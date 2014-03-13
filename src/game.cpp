/*
==========
Copyright 2002 Energon Software

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
==========
*/


#include <cstdlib>
#include <iostream>

#include <time.h>
#include <unistd.h>
#include "SDL.h"

#include "game.h"
#include "Video.h"
#include "Audio.h"
#include "Font.h"
#include "Tetrion.h"
#include "Tetramino.h"
#include "Block.h"
#include "scores.h"
#include "state.h"


/*
 *  constants
 *
 */


const unsigned int LEVEL_1 = 200;


/*
 *  prototypes
 *
 */


void handle_state(Video* const video, Audio* const audio, struct State* const game_state);
void event_loop(Video* const video, Audio* const audio, struct State* const game_state);


/*
 *  globals
 *
 */


Uint32 g_step_time = LEVEL_1;
Uint32 g_last_time = 0;


/*
 *  functions
 *
 */


const bool game_init(Video* const video, Audio* const audio, struct State* const game_state, const int window_width, const int window_height, const int desired_bpp)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    std::cout << "Detected big endian" << std::endl;
#else
    std::cout << "Detected little endian" << std::endl;
#endif

    srand(time(NULL));

    if(!video->create_window(window_width, window_height, desired_bpp, "EBlocks"))
        return false;

    if(!audio->start())
        return false;

    Block::calc_size(video->width(), video->height(), Tetrion::NUM_ROWS, Tetrion::NUM_COLS, video->width()/3);

    clear_state(game_state);

    game_state->tetrion = new Tetrion();

    g_step_time = LEVEL_1;

    return true;
}


void game_run(Video* const video, Audio* const audio, struct State* const game_state)
{
    g_last_time = SDL_GetTicks();

    while(game_state->game != Quit) {
        event_loop(video, audio, game_state);
    }
}


void game_shutdown(Audio* const audio, struct State* const game_state)
{
    if(game_state->tetrion) {
        delete game_state->tetrion;
        game_state->tetrion = NULL;
    }

    audio->stop();
    Audio::unload_sounds();
    Font::unload_fonts();

    SDL_Quit();
}


void start_tetramino(Tetramino* const tetramino, Tetrion* const tetrion)
{
    tetramino->move_to(0,  Tetrion::NUM_COLS >> 1, tetrion);
}


Tetramino* new_tetramino()
{
    switch(rand()%7)
    {
    case 0:
        return new T_Tetramino();
    case 1:
        return new I_Tetramino();
    case 2:
        return new L_Tetramino();
    case 3:
        return new J_Tetramino();
    case 4:
        return new O_Tetramino();
    case 5:
        return new S_Tetramino();
    case 6:
        return new Z_Tetramino();
    }
    return NULL;
}


const bool level_up(const int current_score, const int current_level)
{
    if(((current_score / 100) > current_level) && (g_step_time > 1)) {
        if(g_step_time == LEVEL_1) g_step_time -= 75;
        else g_step_time -= 25;
        if(!g_step_time) g_step_time = 1;
        return true;
    }
    return false;
}


void update_score(PlayerState* const player_state, const int amount)
{
    player_state->score += amount/**player_state->level*/;

    if(level_up(player_state->score, player_state->level))
        player_state->level++;
}


void handle_state(Video* const video, Audio* const audio, struct State* const game_state)
{
    static Tetramino* current_tetramino = NULL;
    static Tetramino* next_tetramino = NULL;
    static bool good_move = false;

    // cleanup if we're done
    if(Quit == game_state->game || End_Game == game_state->game) {
        if(current_tetramino) {
            delete current_tetramino;
            current_tetramino = NULL;
        }
        if(next_tetramino) {
            delete next_tetramino;
            next_tetramino = NULL;
        }
        return;
    }

    // make new tetraminos if we need them
    if(!current_tetramino) current_tetramino = new_tetramino();
    if(!next_tetramino) {
        next_tetramino = new_tetramino();
        start_tetramino(current_tetramino, game_state->tetrion);
    }

    // handle keystate
    bool good = true;
    if(!game_state->paused) {
        if(game_state->keystate[SDLK_DOWN]) {
            while(true) {
                good = current_tetramino->drop(game_state->tetrion);
                if(good) good_move = true;
                else break;
            }
        } else {
            Uint32 cur_time = SDL_GetTicks();

            if(cur_time > (g_last_time + g_step_time)) {
                if(game_state->cw_rotate) {
                    current_tetramino->rotate_cw(game_state->tetrion);
                    game_state->cw_rotate = false;
                }
                if(game_state->ccw_rotate) {
                    current_tetramino->rotate_ccw(game_state->tetrion);
                    game_state->ccw_rotate = false;
                }

                if(game_state->keystate[SDLK_LEFT])
                    current_tetramino->move_left(game_state->tetrion);
                if(game_state->keystate[SDLK_RIGHT])
                    current_tetramino->move_right(game_state->tetrion);

                good = current_tetramino->drop(game_state->tetrion);
                if(good) good_move = true;
                g_last_time = SDL_GetTicks();
            }
        }

        if(!good && !good_move) {
            game_state->game = End_Game;
            audio->stop_music();
            return;
        }
    }

    // render
    if(!video->render(*next_tetramino, game_state))
        game_state->game = Quit;

    // move to next tetramino if we couldn't move
    if(!game_state->paused) {
        if(!good) {
            update_score(&game_state->player, BLOCK_SCORE);
            if(game_state->sounds) {
                if(!audio->play_sound("data/sounds/block.wav"))
                    game_state->game = Quit;
            }

            delete current_tetramino;
            current_tetramino = next_tetramino;
            next_tetramino = NULL;
            good_move = false;

            // it's at this point we can remove some rows
            bool played_expl = false;
            int line_count = 0;
            while(game_state->tetrion->remove_full_row(game_state)) {
                if(!played_expl) {
                    if(game_state->sounds) {
                        if(!audio->play_sound("data/sounds/line.wav"))
                            game_state->game = Quit;
                    }
                    played_expl = true;
                }
                game_state->player.lines++;
                update_score(&game_state->player, LINE_SCORE);
                line_count++;
            }

            if(line_count == 4) update_score(&game_state->player, TETRIS_SCORE);
            sleep(1);
        }
    }
}


void event_loop(Video* const video, Audio* const audio, struct State* const game_state)
{
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(game_state->game)
        {
        case Splash:
            if(SDL_KEYUP == event.type) {
                if(SDLK_ESCAPE != event.key.keysym.sym) {
                    game_state->game = Running;
                    if(game_state->music) {
                        if(!audio->play_music("data/sounds/music1.wav")) {
                            game_state->game = Quit;
                        }
                    }
                } else {
                    game_state->game = Quit;
                }
            }
            break;
        case Demo:
            if(SDL_KEYUP == event.type) {
                game_state->game = Splash;
            }
            break;
        case Running:
            switch(event.type)
            {
            case SDL_KEYUP:
                switch(event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                case SDLK_q:
                    game_state->game = Quit;
                    break;
                default:
                    game_state->keystate[event.key.keysym.sym] = false;
                }
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                case SDLK_UP:
                    game_state->cw_rotate = true;
                    break;
                case SDLK_g:
                    game_state->render_grid = !game_state->render_grid;
                    break;
                case SDLK_m:
                    if(game_state->music) {
                        audio->stop_music();
                        game_state->music = false;
                    } else {
                        if(!audio->play_music("data/sounds/music1.wav"))
                            game_state->game = Quit;
                        game_state->music = true;
                    }
                    break;
                case SDLK_p:
                    game_state->paused = !game_state->paused;
                    break;
#if defined DEBUG
                case SDLK_r:
                    game_state->tetrion->print();
                    break;
#endif
                case SDLK_s:
                    game_state->sounds = !game_state->sounds;
                    break;
                case SDLK_x:
                    game_state->cw_rotate = true;
                    break;
                case SDLK_z:
                    game_state->ccw_rotate = true;
                    break;
                default:
                    game_state->keystate[event.key.keysym.sym] = true;
                }
                break;
            case SDL_QUIT:
                game_state->game = Quit;
                break;
            default:
                break;
            }
            break;
        case End_Game:
            if(SDL_KEYUP == event.type) {
                if(SDLK_ESCAPE != event.key.keysym.sym) {
                    reset_state(game_state);
                    g_step_time = LEVEL_1;
                    g_last_time = SDL_GetTicks();
                    game_state->tetrion = new Tetrion();

                    if(game_state->music) {
                        if(!audio->play_music("data/sounds/music1.wav"))
                            game_state->game = Quit;
                    }
                }
                else game_state->game = Quit;
            }
            break;
        case Quit: return;
        }
    }

    // do rendering and state handling here
    switch(game_state->game)
    {
    case Splash:
        if(!video->render_splash())
            game_state->game = Quit;
        break;
    case Demo:
        break;
    case Running:
        handle_state(video, audio, game_state);
        break;
    case End_Game:
        handle_state(NULL, NULL, game_state);
        if(!video->render_end_game(game_state))
            game_state->game = Quit;
        break;
    case Quit:
        break;
    }
}
