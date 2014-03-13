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


#if !defined TETRAMINO_H
#define TETRAMINO_H


class Block;
class Tetrion;
struct SDL_Surface;


/*
==========
Tetramino class

represents a tetramino (combination of 4 blocks)
==========
*/


class Tetramino
{
protected:
    enum { NUM_BLOCKS = 4 };

public:
    Tetramino();
    Tetramino(const Tetramino& tetramino);  /* NOTE: this doesn't put the blocks into a tetrion */
    virtual ~Tetramino() { }

public:
    // drops the tetramino's row
    // returns false if the move would cause a collision
    // if there was a collision, coordinates remain unaffected
    const bool drop(Tetrion* const tetrion) const;

    // moves the tetramino left/right
    // no movement is performed if the move would cause a collision
    void move_left(Tetrion* const tetrion) const;
    void move_right(Tetrion* const tetrion) const;

    // rotates the tetramino clock-wise/counter clock-wise around the tetramino center
    // no rotation is performed if the rotation would cause a collision
    void rotate_cw(Tetrion* const tetrion) const;
    void rotate_ccw(Tetrion* const tetrion) const;

public:
    // moves the top of the tetramino to the position ('face up')
    virtual void move_to(const int row, const int column, Tetrion* const tetrion) const = 0;

    // returns a surface with the tetramino rendered on it 'face up'
    virtual SDL_Surface* generate_surface(const Uint32 video_flags, const int bpp) const = 0;

protected:
    Block* m_blocks[NUM_BLOCKS];
    int m_center;
};


/*
==========
I shaped tetramino
==========
*/


class I_Tetramino : public Tetramino
{
public:
    I_Tetramino();

public:
    virtual void move_to(const int row, const int column, Tetrion* const tetrion) const;
    virtual SDL_Surface* generate_surface(const Uint32 video_flags, const int bpp) const;
};


/*
==========
J shaped tetramino
==========
*/


class J_Tetramino : public Tetramino
{
public:
    J_Tetramino();

public:
    virtual void move_to(const int row, const int column, Tetrion* const tetrion) const;
    virtual SDL_Surface* generate_surface(const Uint32 video_flags, const int bpp) const;
};


/*
==========
L shaped tetramino
==========
*/


class L_Tetramino : public Tetramino
{
public:
    L_Tetramino();

public:
    virtual void move_to(const int row, const int column, Tetrion* const tetrion) const;
    virtual SDL_Surface* generate_surface(const Uint32 video_flags, const int bpp) const;
};


/*
==========
O shaped tetramino
==========
*/


class O_Tetramino : public Tetramino
{
public:
    O_Tetramino();

public:
    virtual void move_to(const int row, const int column, Tetrion* const tetrion) const;
    virtual SDL_Surface* generate_surface(const Uint32 video_flags, const int bpp) const;
};


/*
==========
S shaped tetramino
==========
*/


class S_Tetramino : public Tetramino
{
public:
    S_Tetramino();

public:
    virtual void move_to(const int row, const int column, Tetrion* const tetrion) const;
    virtual SDL_Surface* generate_surface(const Uint32 video_flags, const int bpp) const;
};


/*
==========
T shaped tetramino
==========
*/


class T_Tetramino : public Tetramino
{
public:
    T_Tetramino();

public:
    virtual void move_to(const int row, const int column, Tetrion* const tetrion) const;
    virtual SDL_Surface* generate_surface(const Uint32 video_flags, const int bpp) const;
};


/*
==========
Z shaped tetramino
==========
*/


class Z_Tetramino : public Tetramino
{
public:
    Z_Tetramino();

public:
    virtual void move_to(const int row, const int column, Tetrion* const tetrion) const;
    virtual SDL_Surface* generate_surface(const Uint32 video_flags, const int bpp) const;
};


#endif
