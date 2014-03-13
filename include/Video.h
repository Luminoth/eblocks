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


#if !defined VIDEO_H
#define VIDEO_H


#include <string>

#include "Font.h"


class Tetramino;
struct State;
struct SDL_Surface;


/*
==========
Video class

used for video related stuff and things
NOTE: the hud size is 1/3 the width of the screen by the full height
==========
*/


class Video
{
public:
    // color constants (determined by endianness)
    static const Uint32 RMASK;
    static const Uint32 GMASK;
    static const Uint32 BMASK;
    static const Uint32 AMASK;

    // the size of the grid lines
    static const int GRID_SIZE;

public:
    // returns a surface with the bitmap on it
    // returns NULL on error
    static SDL_Surface* load_bitmap(const std::string& filename);

public:
    Video();
    ~Video();

public:
    // creates the main window
    const bool create_window(const int width, const int height, const int desired_bpp, const std::string& title);

    // gets the main surface flags
    const Uint32 get_flags();

    // renders the current game
    const bool render(const Tetramino& next_tetramino, struct State* const game_state);

    // renders the splash screen
    const bool render_splash();

    // renders the end-game screen
    const bool render_end_game(struct State* const game_state);

public:
    const bool is_good() const { return m_good; }
    const int width() const { return m_window->w; }
    const int height() const { return m_window->h; }

private:
    const bool render_hud();

private:
    SDL_Surface* m_window;
    SDL_Surface* m_splash;
    SDL_Surface* m_hud;
    SDL_Surface* m_end;

    Font m_font;

    Uint32 m_flags;
    bool m_good;

private:
    Video(const Video& video) {}
};


#endif
