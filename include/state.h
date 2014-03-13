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

#if !defined STATE_H
#define STATE_H


#include "SDL.h"


class Tetrion;


/*
 *  state constants
 *
 */


enum GameState
{
    Splash,
    Demo,
    Running,
    End_Game,
    Quit
};


/*
 *  state structures
 *
 */


struct PlayerState
{
    long score;
    int lines;
    int level;
};


struct State
{
    GameState game;
    PlayerState player;

    Tetrion* tetrion;

    bool gravity;

    bool sounds;
    bool music;

    bool render_grid;
    bool paused;

    bool keystate[SDLK_LAST];
    bool cw_rotate;
    bool ccw_rotate;
};


/*
 *  functions
 *
 */


inline void clear_state(State* const state)
{
    state->game = Splash;
    memset(&state->player, 0, sizeof(struct PlayerState));
    state->player.level = 1;

    state->gravity = false;

    state->tetrion = NULL;

    state->sounds = true;
    state->music = true;

    state->render_grid = false;
    state->paused = false;

    memset(state->keystate, 0, SDLK_LAST*sizeof(bool));
    state->cw_rotate = false;
    state->ccw_rotate = false;
}


inline void reset_state(State* const state)
{
    state->game = Running;
    memset(&state->player, 0, sizeof(struct PlayerState));
    state->player.level = 1;

    if(state->tetrion) delete state->tetrion;
    state->tetrion = NULL;

    memset(state->keystate, 0, SDLK_LAST*sizeof(bool));
    state->cw_rotate = false;
    state->ccw_rotate = false;
}


#endif
