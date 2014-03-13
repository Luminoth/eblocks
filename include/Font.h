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

#if !defined FONT_H
#define FONT_H


#include <string>
#include <map>


struct SDL_Surface;


/*
==========
Font class

represents a single (bitmapped) font to print text with
==========
*/


class Font
{
private:
    static std::map<std::string, SDL_Surface*> font_map;

public:
    /* NOTE: if you load fonts, this must be called to unload all of them */
    static void unload_fonts();

public:
    Font();

public:
    // loads a font from a bitmap
    const bool load_font(const std::string& filename);

    // renders the text on the destination surface at position
    void render_text(const std::string& text, SDL_Surface* const destination, const int x, const int y);

public:
    const int char_width() const { return m_char_width; }
    const int char_height() const { return m_char_height; }

private:
    SDL_Surface* m_surface;
    int m_char_width;
    int m_char_height;
};


#endif
