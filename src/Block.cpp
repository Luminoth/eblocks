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


#include "SDL.h"

#include "Block.h"
#include "Video.h"


/*
 *  Block class variables
 *
 */


int Block::width  = Block::DEFAULT_WIDTH;
int Block::height = Block::DEFAULT_HEIGHT;


/*
 *  Block class functions
 *
 */


void Block::calc_size(const int window_width, const int window_height, const int num_rows, const int num_cols, const int hud_width)
{
    const int tetrion_width = window_width - hud_width;
    width = (tetrion_width / num_cols) - Video::GRID_SIZE;
    height = (window_height / num_rows) - Video::GRID_SIZE;
}


/*
 *  Block methods
 *
 */


Block::Block() : m_red(DEFAULT_RED), m_green(DEFAULT_GREEN), m_blue(DEFAULT_BLUE)
{
}


SDL_Surface* Block::generate_surface(const unsigned int video_flags, const int bpp) const
{
    SDL_Surface* surface = SDL_CreateRGBSurface(video_flags, width, height, bpp, Video::RMASK, Video::GMASK, Video::BMASK, Video::AMASK);
    if(surface) SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, m_red, m_green, m_blue));
    return surface;
}
