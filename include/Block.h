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


#if !defined BLOCK_H
#define BLOCK_H


struct SDL_Surface;


/*
==========
Block class

represents a single block of a tetramino
==========
*/


class Block
{
private:
    enum
    {
        DEFAULT_WIDTH  = 15,
        DEFAULT_HEIGHT = 15,
        DEFAULT_RED = 0,
        DEFAULT_GREEN = 0,
        DEFAULT_BLUE = 0
    };

public:
    // block width/height
    // calling calc_size() figures the right size of a block
    // based on various factors
    static int width;
    static int height;

public:
    // calculates the block width/height from the window width/height, number of rows and columns, and the hud width
    static void calc_size(const int window_width, const int window_height, const int num_rows, const int num_cols, const int hud_width);

public:
    Block();

public:
    // returns an SDL surface with the block drawn on it
    // surface is the size of a block
    // use set_*() functions to set the color of the block
    SDL_Surface* generate_surface(const unsigned int video_flags, const int bpp) const;

public:
    // sets the color of the block
    void set_red(const unsigned int red) { m_red = red; }
    void set_green(const unsigned int green) { m_green = green; }
    void set_blue(const unsigned int blue) { m_blue = blue; }

private:
    unsigned int m_red;
    unsigned int m_green;
    unsigned int m_blue;
};


#endif
