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

#include <string.h>

#include "Tetrion.h"
#include "Block.h"
#include "state.h"


/*
 *  constants
 *
 */


// this is just to make things nicer...
const int MAX_LOCATIONS = Tetrion::NUM_ROWS*Tetrion::NUM_COLS;


/*
 *  functions
 *
 */


// returns the actual location from the row and column positions
inline const int calc_location(const int row, const int column)
{
    return (row * Tetrion::NUM_COLS) + column;
}


// returns the row from the location
inline const int calc_row(const int location)
{
    return location / Tetrion::NUM_COLS;
}


// returns the column from the location
inline const int calc_column(const int location)
{
    return location % Tetrion::NUM_COLS;
}



/*
 *  Tetrion class functions
 *
 */


const bool Tetrion::is_bottom_row(const int location)
{
    return ((NUM_ROWS -1) == calc_row(location));
}


const bool Tetrion::is_left_column(const int location)
{
    return (0 == calc_column(location));
}


const bool Tetrion::is_right_column(const int location)
{
    return ((NUM_COLS - 1) == calc_column(location));
}


/*
 *  Tetrion methods
 *
 */


Tetrion::Tetrion()
{
    memset(m_grid, 0, MAX_LOCATIONS*sizeof(Block*));
}


Tetrion::Tetrion(const Tetrion& tetrion)
{
    for(int i=0; i<MAX_LOCATIONS; i++) {
        if(tetrion.m_grid[i]) m_grid[i] = new Block(*tetrion.m_grid[i]);
        else m_grid[i] = NULL;
    }
}


Tetrion::~Tetrion()
{
    for(int i=0; i<MAX_LOCATIONS; i++) {
        if(m_grid[i]) delete m_grid[i];
    }
}


void Tetrion::print() const
{
    for(int row=0; row<NUM_ROWS; row++) {
        for(int col=0; col<NUM_COLS; col++) {
            if(get_block(row, col)) std::cout << "1 ";
            else std::cout << "0 ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


const bool Tetrion::drop_blocks(const int num_blocks, Block* const blocks[])
{
    return move_blocks(num_blocks, blocks, NUM_COLS, is_bottom_row);
}


void Tetrion::move_blocks_right(const int num_blocks, Block* const blocks[])
{
    move_blocks(num_blocks, blocks, 1, is_right_column);
}


void Tetrion::move_blocks_left(const int num_blocks, Block* const blocks[])
{
    move_blocks(num_blocks, blocks, -1, is_left_column);
}


void Tetrion::rotate_blocks_cw(const int num_blocks, Block* const blocks[], const int center)
{
    if(center < 0) return;

    const int row = 0;
    const int col = 1;
    int i=0;

    // get the column and row locations for each block
    int locations[num_blocks][2];
    for(i=0; i<num_blocks; i++) {
        const int location = get_location(blocks[i]);
        locations[i][row] = calc_row(location);
        locations[i][col] = calc_column(location);
    }

    // this only works if (0, 0) in true coords is in the upper-left corner of the grid

    int to_locations[num_blocks][2];
    for(i=0; i<num_blocks; i++) {
        // get the relative location (to the center cube)
        const int relative_row = locations[center][row] - locations[i][row];
        const int relative_col = locations[i][col] - locations[center][col];

        // use 2d rotation formula to figure new location
        // uses the fact that sin(-90) = -1 and cos(-90) = 0
        const int relative_to_row = -relative_col;
        const int relative_to_col = relative_row;

        // calculate the displacement in real coordinates
        // x`/y` = x/y + dx/dy
        to_locations[i][row] = locations[i][row] + (relative_row - relative_to_row);
        to_locations[i][col] = locations[i][col] + (relative_to_col - relative_col);

        // check for wall collisions
        const int location = calc_location(locations[i][row], locations[i][col]);
        const int to_location = calc_location(to_locations[i][row], to_locations[i][col]);
        if((is_right_column(location) || is_right_column(location+1)) && is_left_column(to_location))
            return;
        if((is_left_column(location) || is_left_column(location-1)) && is_right_column(to_location))
            return;
    }

    // remove each block now so block collision detection works
    for(i=0; i<num_blocks; i++) remove_block(locations[i][row], locations[i][col]);

    // make sure we can do the move to the new location
    for(i=0; i<num_blocks; i++) {
        if(get_block(to_locations[i][row], to_locations[i][col])) {
            for(i=0; i<num_blocks; i++) set_block(locations[i][row], locations[i][col], blocks[i]);
            return;
        }
    }

    // make the move
    for(i=0; i<num_blocks; i++) set_block(to_locations[i][row], to_locations[i][col], blocks[i]);
}


void Tetrion::rotate_blocks_ccw(const int num_blocks, Block* const blocks[], const int center)
{
    if(center < 0) return;

    const int row = 0;
    const int col = 1;
    int i=0;

    // get the column and row locations for each block
    int locations[num_blocks][2];
    for(i=0; i<num_blocks; i++) {
        const int location = get_location(blocks[i]);
        locations[i][row] = calc_row(location);
        locations[i][col] = calc_column(location);
    }

    // this only works if (0, 0) in true coords is in the upper-left corner of the grid

    int to_locations[num_blocks][2];
    for(i=0; i<num_blocks; i++) {
        // get the relative location (to the center cube)
        const int relative_row = locations[center][row] - locations[i][row];
        const int relative_col = locations[i][col] - locations[center][col];

        // use 2d rotation formula to figure new location
        // uses the fact that sin(90) = 1 and cos(90) = 0
        const int relative_to_row = relative_col;
        const int relative_to_col = -relative_row;

        // calculate the displacement in real coordinates
        // x`/y` = x/y + dx/dy
        to_locations[i][row] = locations[i][row] + (relative_row - relative_to_row);
        to_locations[i][col] = locations[i][col] + (relative_to_col - relative_col);

        // check for wall collisions
        const int location = calc_location(locations[i][row], locations[i][col]);
        const int to_location = calc_location(to_locations[i][row], to_locations[i][col]);
        if((is_right_column(location) || is_right_column(location+1)) && is_left_column(to_location))
            return;
        if((is_left_column(location) || is_left_column(location-1)) && is_right_column(to_location))
            return;
    }

    // remove each block now so block collision detection works
    for(i=0; i<num_blocks; i++) remove_block(locations[i][row], locations[i][col]);

    // make sure we can do the move to the new location
    for(i=0; i<num_blocks; i++) {
        if(get_block(to_locations[i][row], to_locations[i][col])) {
            for(i=0; i<num_blocks; i++) set_block(locations[i][row], locations[i][col], blocks[i]);
            return;
        }
    }

    // make the move
    for(i=0; i<num_blocks; i++) set_block(to_locations[i][row], to_locations[i][col], blocks[i]);
}


Block* const Tetrion::get_block(const int row, const int column) const
{
    return get_block(calc_location(row, column));
}


Block* const Tetrion::get_block(const int location) const
{
    if(location >= MAX_LOCATIONS || location < 0) return NULL;
    return m_grid[location];
}


Block* const Tetrion::set_block(const int row, const int column, Block* const block)
{
    return set_block(calc_location(row, column), block);
}


Block* const Tetrion::set_block(const int location, Block* const block)
{
    if(location >= MAX_LOCATIONS || location < 0) return NULL;

#if defined DEBUG
    if(m_grid[location])
        std::cout << "You are overwriting a block at " << location << std::endl;
#endif

    Block* r = m_grid[location];
    m_grid[location] = block;
    return r;
}


const int Tetrion::get_location(Block* const block) const
{
    if(!block) return -1;
    for(int i=0; i<MAX_LOCATIONS; i++) {
        if(m_grid[i] == block) return i;
    }
    return -1;
}


Block* const Tetrion::remove_block(const int row, const int column)
{
    return remove_block(calc_location(row, column));
}


Block* const Tetrion::remove_block(const int location)
{
    if(location >= MAX_LOCATIONS || location < 0) return NULL;

    Block* r = m_grid[location];
    m_grid[location] = NULL;
    return r;
}


const bool row_is_full(const int start, Block* const grid[], const int num_cols)
{
    // a row is full if we can go from one end to another without
    // finding a NULL grid location

    const int end = start + num_cols;
    for(int i=start; i<end; i++) {
        if(!grid[i]) return false;
    }
    return true;
}


const bool Tetrion::remove_full_row(struct State* const game_state)
{
/* TODO: use either naive or flood-fill algorithm based on state */

    for(int i=NUM_ROWS-1; i>=0; i--) {
        const int start_location = i * NUM_COLS;
        if(row_is_full(start_location, m_grid, NUM_COLS)) {
            remove_row(i);
            drop_rows_above(i);
            return true;
        }
    }
    return false;
}


const bool Tetrion::move_blocks(const int num_blocks, Block* const blocks[], const int amount, const bool (*comparison)(const int))
{
    if(num_blocks <= 0 || !blocks) return false;

    int i=0;
    int locations[num_blocks];
    int to_locations[num_blocks];

    // check to make sure each block is in the tetrion
    for(i=0; i<num_blocks; i++) {
        locations[i] = get_location(blocks[i]);
        if(-1 == locations[i] || comparison(locations[i])) return false;

        // calculate where the block is going to
        to_locations[i] = locations[i] + amount;
    }

    // remove each block now so collision detection works
    for(i=0; i<num_blocks; i++) remove_block(locations[i]);

    // make sure we can do the move to the new location
    for(i=0; i<num_blocks; i++) {
        if(get_block(to_locations[i])) {
            for(i=0; i<num_blocks; i++) set_block(locations[i], blocks[i]);
            return false;
        }
    }

    // make the move
    for(i=0; i<num_blocks; i++) set_block(to_locations[i], blocks[i]);
    return true;
}


void Tetrion::remove_row(const int row)
{
    if(row < 0 || row >= NUM_ROWS) return;

    // go from (row, 0) to (row, num_cols) removing blocks
    for(int i=0; i<NUM_COLS; i++) {
        Block* block = remove_block(row, i);
        if(block) delete block;
    }
}


const bool Tetrion::drop_block(const int location)
{
    Block* block = get_block(location);
    if(!block || is_bottom_row(location)) return false;

    const int to_location = location + NUM_COLS;
    if(get_block(to_location)) return false;

    remove_block(location);
    set_block(to_location, block);
    return true;
}


void Tetrion::drop_rows_above(const int row)
{
    if(row <= 0 || row >= NUM_ROWS) return;

    // start at the far right of the row above the one we're dropping,
    // work toward the beginning dropping each block as we go
    const int cur_location = (row * NUM_COLS) - 1;
    for(int i=cur_location; i>=0; i--) {
        if(m_grid[i]) drop_block(i);
    }
}
