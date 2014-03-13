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


#include <string>
#include <map>

#include "SDL.h"

#include "Font.h"
#include "Video.h"


/*
 *  Font class variables
 *
 */


std::map<std::string, SDL_Surface*> Font::font_map;


/*
 *  Font class functions
 *
 */


void Font::unload_fonts()
{
    for(std::map<std::string, SDL_Surface*>::iterator it = font_map.begin(); it != font_map.end(); ++it) {
        SDL_FreeSurface(it->second);
        font_map.erase(it);
    }
}


/*
 *  Font methods
 *
 */


Font::Font() : m_surface(NULL), m_char_width(0), m_char_height(0)
{
}


const bool Font::load_font(const std::string& filename)
{
    std::map<std::string, SDL_Surface*>::const_iterator it = font_map.find(filename);
    if(it != font_map.end())
        m_surface = it->second;
    else {
        // gotta load the bitmap
        SDL_Surface* surface = Video::load_bitmap(filename);
        if(!surface) return false;
        font_map[filename] = surface;

        m_surface = surface;
    }

    SDL_SetColorKey(m_surface, SDL_SRCCOLORKEY, SDL_MapRGB(m_surface->format, 0, 0, 0));

    // calculate the character width/height
    m_char_width = m_surface->w >> 4;
    m_char_height = m_surface->h >> 4;
    return true;
}


void Font::render_text(const std::string& text, SDL_Surface* const destination, const int x, const int y)
{
    if(!destination) return;

    SDL_Rect pos;
    pos.x = x; pos.y = y;

    for(unsigned int i=0; i<text.length(); i++) {
        if((pos.x + m_char_width) > destination->w) break;
        if((pos.y + m_char_height) > destination->h) break;

        const char cur_char = text[i];

        // handle some special characters
        switch(cur_char)
        {
        case '\n':
            pos.x = x;
            pos.y += m_char_height;
            continue;
        case '\r':
            pos.x = x;
            continue;
        case '\t':
            pos.x += (m_char_width << 2);
            continue;
        }

        SDL_Rect src;
        src.w = m_char_width; src.h = m_char_height;

        const int sx = static_cast<int>(cur_char) % 16;
        const int sy = static_cast<int>(cur_char) >> 4;

        src.x = (sx * m_char_width) + sx;
        src.y = (sy * m_char_height) + sy;

        SDL_BlitSurface(m_surface, &src, destination, &pos);
        pos.x += m_char_width;
    }
}
