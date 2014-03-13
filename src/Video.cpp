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
#include "Tetrion.h"
#include "Tetramino.h"
#include "Block.h"
#include "state.h"


/*
 *  Video constants
 *
 */


/* FIXME: this should be right, but all my colors are backwards... */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const Uint32 Video::RMASK = 0xff000000;
    const Uint32 Video::GMASK = 0x00ff0000;
    const Uint32 Video::BMASK = 0x0000ff00;
//    const Uint32 Video::AMASK = 0x000000ff;
    const Uint32 Video::AMASK = 0x00000000;
#else
    const Uint32 Video::RMASK = 0x000000ff;
    const Uint32 Video::GMASK = 0x0000ff00;
    const Uint32 Video::BMASK = 0x00ff0000;
//    const Uint32 Video::AMASK = 0xff000000;
    const Uint32 Video::AMASK = 0x00000000;
#endif

const int Video::GRID_SIZE = 2;


/*
 *  Video class functions
 *
 */


SDL_Surface* Video::load_bitmap(const std::string& filename)
{
/* FIXME: do some path searching to find the image */
    SDL_Surface* r = SDL_LoadBMP(filename.c_str());

    return r;
}


/*
 *  Video methods
 *
 */


Video::Video() : m_window(NULL), m_splash(NULL), m_hud(NULL), m_end(NULL), m_flags(0), m_good(true)
{
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) m_good = false;
    else {
        char buf[32];
        std::cout << "Found video driver: " << SDL_VideoDriverName(buf, 32) << std::endl;
        get_flags();
    }

    if(!m_font.load_font("data/images/font.bmp")) m_good = false;
}


Video::~Video()
{
    if(m_splash) SDL_FreeSurface(m_splash);
    if(m_hud) SDL_FreeSurface(m_hud);
    if(m_end) SDL_FreeSurface(m_end);

    if(SDL_WasInit(SDL_INIT_VIDEO))
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
}


const bool Video::create_window(const int width, const int height, const int desired_bpp, const std::string& title)
{
    const int bpp = SDL_VideoModeOK(width, height, desired_bpp, m_flags);
    if(!bpp) {
        std::cerr << width << "x" << height << "x" << desired_bpp << " is not a valid video mode." << std::endl;
        return false;
    }

    std::cout << "Creating window '" << title << "': " << width << "x" << height << "x" << bpp << std::endl;
    m_window = SDL_SetVideoMode(width, height, bpp, m_flags);
    SDL_WM_SetCaption(title.c_str(), NULL);

    return true;
}


const Uint32 Video::get_flags()
{
    if(!m_flags) {
        const SDL_VideoInfo* info = SDL_GetVideoInfo();

        if(info->hw_available) m_flags |= (SDL_HWSURFACE | SDL_DOUBLEBUF);
        else m_flags |= SDL_SWSURFACE;
    }
    return m_flags;
}


void render_grid(SDL_Surface* const window, const int hud_width)
{
/*
FIXME: this isn't scaling good for some resolutions...
Some calculation is off when figuring the size of things (not being very precise with things)
*/
    const int tetrion_width = window->w - hud_width;

    int i=0;
    int x = Block::width;
    for(i=0; i<Tetrion::NUM_COLS-1; i++) {
        SDL_Surface* surface = SDL_CreateRGBSurface(window->flags, Video::GRID_SIZE, window->h, window->format->BitsPerPixel, Video::RMASK, Video::GMASK, Video::BMASK, Video::AMASK);
        if(surface) {
            SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));

            SDL_Rect pos;
            pos.x = x; pos.y = 0;
            SDL_BlitSurface(surface, NULL, window, &pos);

            SDL_FreeSurface(surface);
        }
        x += (Block::width + Video::GRID_SIZE);
    }

    int y = Block::height;
    for(i=0; i<Tetrion::NUM_ROWS-1; i++) {
        SDL_Surface* surface = SDL_CreateRGBSurface(window->flags, tetrion_width, Video::GRID_SIZE, window->format->BitsPerPixel, Video::RMASK, Video::GMASK, Video::BMASK, Video::AMASK);
        if(surface) {
            SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));

            SDL_Rect pos;
            pos.x = 0; pos.y = y;
            SDL_BlitSurface(surface, NULL, window, &pos);

            SDL_FreeSurface(surface);
        }
        y += (Block::height + Video::GRID_SIZE);
    }
}


void print_score(SDL_Surface* const window, Font* const font, const int score)
{
    char scratch[32];
    snprintf(scratch, 32, "%i", score);
    font->render_text(scratch, window, window->w - ((font->char_width() << 3) + 6), (3*font->char_height()) + 10);
}


void print_lines(SDL_Surface* const window, Font* const font, const int lines)
{
    char scratch[32];
    snprintf(scratch, 32, "%i", lines);
    font->render_text(scratch, window, window->w - ((font->char_width() << 3) + 6), (13*font->char_height()) + 9);
}


void print_level(SDL_Surface* const window, Font* const font, const int level)
{
    char scratch[32];
    snprintf(scratch, 32, "%i", level);
    font->render_text(scratch, window, window->w - ((font->char_width() << 3) + 6), (font->char_height() << 3) + 9);
}


const bool render_background(SDL_Surface* const window)
{
/* this can be used to put a background on the tetrion */
    return true;
}


const bool render_blocks(SDL_Surface* const window, const Tetrion& tetrion)
{
    int x = 0;
    int y = 0;

    for(int i=0; i<Tetrion::NUM_ROWS; i++) {
        for(int j=0; j<Tetrion::NUM_COLS; j++) {
            Block* block = tetrion.get_block(i, j);
            if(block) {
                SDL_Surface* surface = block->generate_surface(window->flags, window->format->BitsPerPixel);
                if(surface) {
                    SDL_Rect pos;
                    pos.x = x; pos.y = y;
                    SDL_BlitSurface(surface, NULL, window, &pos);

                    SDL_FreeSurface(surface);
                } else return false;
            }
            x += (Block::width + Video::GRID_SIZE);
        }
        y += (Block::height + Video::GRID_SIZE);
        x = 0;
    }
    return true;
}


const bool render_next_tetramino(SDL_Surface* const window, const Tetramino& next_tetramino)
{
    SDL_Surface* surface = next_tetramino.generate_surface(window->flags, window->format->BitsPerPixel);
    if(surface) {
        SDL_SetColorKey(surface, SDL_SRCCOLORKEY, SDL_MapRGB(surface->format, 0, 0, 0));

        SDL_Rect pos;

        const int hud_width = window->w/3;
        pos.x = window->w - (hud_width >> 1) - (surface->w >> 1); pos.y = 334 - (surface->h >> 1);
        SDL_BlitSurface(surface, NULL, window, &pos);

        SDL_FreeSurface(surface);
    } else return false;
    return true;
}


const bool Video::render(const Tetramino& next_tetramino, struct State* const game_state)
{
    SDL_FillRect(m_window, NULL, SDL_MapRGB(m_window->format, 0, 0, 0));

    if(!render_hud()) {
        std::cerr << "There was an error rendering the hud: " << SDL_GetError() << std::endl;
        return false;
    }

    if(!render_next_tetramino(m_window, next_tetramino)) {
        std::cerr << "There was an error rendering the next tetramino: " << SDL_GetError() << std::endl;
        return false;
    }

    print_score(m_window, &m_font, game_state->player.score);
    print_lines(m_window, &m_font, game_state->player.lines);
    print_level(m_window, &m_font, game_state->player.level);

    if(!render_background(m_window)) {
        std::cerr << "There was an error rendering the background: " << SDL_GetError() << std::endl;
        return false;
    }

    if(game_state->render_grid) render_grid(m_window, width()/3);

    if(!render_blocks(m_window, *(game_state->tetrion))) {
        std::cerr << "There was an error rendering the blocks: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_Flip(m_window);
    return true;
}


const bool Video::render_splash()
{
    if(!m_splash) {
        m_splash = load_bitmap("data/images/splash.bmp");
        if(!m_splash) {
            std::cerr << "Could not load splash bitmap: " << SDL_GetError() << std::endl;
            return false;
        }
    }

    SDL_FillRect(m_window, NULL, SDL_MapRGB(m_window->format, 0, 0, 0));

    SDL_BlitSurface(m_splash, NULL, m_window, NULL);

    SDL_Flip(m_window);
    return true;
}


const bool Video::render_end_game(struct State* const game_state)
{
    if(!m_end) {
        m_end = load_bitmap("data/images/end.bmp");
        if(!m_end) {
            std::cerr << "Could not load end bitmap: " << SDL_GetError() << std::endl;
            return false;
        }
    }

    SDL_FillRect(m_window, NULL, SDL_MapRGB(m_window->format, 0, 0, 0));

    if(!render_hud())
        return false;

    print_score(m_window, &m_font, game_state->player.score);
    print_lines(m_window, &m_font, game_state->player.lines);
    print_level(m_window, &m_font, game_state->player.level);

    SDL_Rect pos;
    pos.x = 0; pos.y = 0;
    SDL_BlitSurface(m_end, NULL, m_window, &pos);

    SDL_Flip(m_window);
    return true;
}


const bool Video::render_hud()
{
    if(!m_hud) {
        m_hud = load_bitmap("data/images/hud.bmp");
        if(!m_hud) {
            std::cout << "Could not load hud bitmap: " << SDL_GetError() << std::endl;
            return false;
        }
    }

    SDL_Rect pos;
    pos.x = m_window->w - m_hud->w; pos.y = 0;
    SDL_BlitSurface(m_hud, NULL, m_window, &pos);

    return true;
}
