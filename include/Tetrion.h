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


#if !defined TETRION_H
#define TETRION_H


class Block;
struct State;


/*
==========
Tetrion class

represents the game field
holds a grid of block pointers that
    can be manipulated and rendered
==========
*/


class Tetrion
{
public:
    enum
    {
        NUM_ROWS = 20,
        NUM_COLS = 10
    };

public:
    // returns true if the location is an extremety
    static const bool is_bottom_row(const int location);
    static const bool is_left_column(const int location);
    static const bool is_right_column(const int location);

public:
    Tetrion();
    Tetrion(const Tetrion& tetrion);

    // calls delete on all blocks in the tetrion
    ~Tetrion();

public:
    // prints the tetrion to stdout
    // a 0 means no block, a 1 means there's a block
    void print() const;

    // moves a set of blocks
    // prevents wrapping
    // drop_blocks returns false if we collided with something
    // no move happens if there was a collision
    const bool drop_blocks(const int num_blocks, Block* const blocks[]);    /* FIXME: this doesn't work at all if the array has one block */
    void move_blocks_right(const int num_blocks, Block* const blocks[]);
    void move_blocks_left(const int num_blocks, Block* const blocks[]);

    // rotates a set of cubes clockwise/counter-clockwise around the center (index of center block)
    void rotate_blocks_cw(const int num_blocks, Block* const blocks[], const int center);
    void rotate_blocks_ccw(const int num_blocks, Block* const blocks[], const int center);

    // returns the block at the given location
    // returns NULL if there is no block there
    Block* const get_block(const int row, const int column) const;
    Block* const get_block(const int location) const;

    // sets the block at the given location
    // returns NULL if no blocks were overwritten
    // returns a pointer to the block that was overwritten if one was
    // no memory is freed if a block is overwritten
    Block* const set_block(const int row, const int column, Block* const block);
    Block* const set_block(const int location, Block* const block);

    // returns the location of the block
    // returns -1 if the block isn't in the tetrion
    const int get_location(Block* const block) const;

    // removes the block at location if one exists there and returns it
    // returns NULL if there was no block at that location
    // no memory is freed by this
    Block* const remove_block(const int row, const int column);
    Block* const remove_block(const int location);

    // removes the bottom-most full row and drops all blocks above the removed row
    // returns true if a row was removed, false otherwise
    // this also frees the memory from all removed blocks
    // either does naive drop or flood-fill depending on state
    const bool remove_full_row(struct State* const game_state);

private:
    const bool move_blocks(const int num_blocks, Block* const blocks[], const int amount, const bool (*comparison)(const int));
    void remove_row(const int row);
    const bool drop_block(const int location);
    void drop_rows_above(const int row);

private:
    Block* m_grid[NUM_ROWS*NUM_COLS];
};


#endif
