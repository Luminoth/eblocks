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


#include <iostream>

#include "SDL.h"

#include "Video.h"
#include "Audio.h"
#include "Tetrion.h"
#include "Block.h"
#include "game.h"
#include "state.h"


/*
 *  functions
 *
 */


void process_arguments(const int argc, char* const argv[], struct State* const game_state)
{
    for(int i=1; i<argc; i++) {
        if(!strcmp(argv[i], "--nomusic"))
            game_state->music = false;
        else if(!strcmp(argv[i], "--nosound")) {
            game_state->sounds = false;
            game_state->music  = false;
        } else if(!strcmp(argv[i], "-g"))
            game_state->gravity = true;
    }
}


int main(int argc, char* argv[])
{
    const int width=450;
    const int height=560;
    const int bpp=16;

    Video video;
    if(!video.is_good()) {
        std::cerr << "Video initialization failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    Audio audio;
    if(!audio.is_good()) {
        std::cerr << "Audio initialization failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    struct State game_state;
    if(!game_init(&video, &audio, &game_state, width, height, bpp)) {
        std::cerr << "Game initialization failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    process_arguments(argc, argv, &game_state);

#if defined DEBUG
    std::cout << "Block size: " << Block::width << "x" << Block::height << std::endl;
    std::cout << "Tetrion dimensions: " << Tetrion::NUM_ROWS << "x" << Tetrion::NUM_COLS << std::endl;
#endif

    game_run(&video, &audio, &game_state);

    game_shutdown(&audio, &game_state);
    return 0;
}
