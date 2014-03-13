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

#include "Tetramino.h"
#include "Tetrion.h"
#include "Block.h"
#include "Video.h"


/*
 *  Tetramino methods
 *
 */


Tetramino::Tetramino() : m_center(0)
{
    for(int i=0; i<NUM_BLOCKS; i++)
        m_blocks[i] = new Block();
}


Tetramino::Tetramino(const Tetramino& tetramino) : m_center(tetramino.m_center)
{
    for(int i=0; i<NUM_BLOCKS; i++) {
        if(tetramino.m_blocks[i]) m_blocks[i] = new Block(*tetramino.m_blocks[i]);
        else m_blocks[i] = NULL;
    }
}


const bool Tetramino::drop(Tetrion* const tetrion) const
{
    return tetrion->drop_blocks(NUM_BLOCKS, m_blocks);
}


void Tetramino::move_left(Tetrion* const tetrion) const
{
    tetrion->move_blocks_left(NUM_BLOCKS, m_blocks);
}


void Tetramino::move_right(Tetrion* const tetrion) const
{
    tetrion->move_blocks_right(NUM_BLOCKS, m_blocks);
}


void Tetramino::rotate_cw(Tetrion* const tetrion) const
{
    tetrion->rotate_blocks_cw(NUM_BLOCKS, m_blocks, m_center);
}


void Tetramino::rotate_ccw(Tetrion* const tetrion) const
{
    tetrion->rotate_blocks_ccw(NUM_BLOCKS, m_blocks, m_center);
}


/*
 *  I_Tetramino methods
 *
 */


I_Tetramino::I_Tetramino() : Tetramino()
{
    for(int i=0; i<NUM_BLOCKS; i++) {
        m_blocks[i]->set_red(255);
        m_blocks[i]->set_green(0);
        m_blocks[i]->set_blue(0);
    }
    m_center = 1;
}


void I_Tetramino::move_to(const int row, const int column, Tetrion* const tetrion) const
{
    for(int i=0; i<NUM_BLOCKS; i++) tetrion->remove_block(tetrion->get_location(m_blocks[i]));

    tetrion->set_block(row, column, m_blocks[0]);
    tetrion->set_block(row+1, column, m_blocks[1]);
    tetrion->set_block(row+2, column, m_blocks[2]);
    tetrion->set_block(row+3, column, m_blocks[3]);
}


SDL_Surface* I_Tetramino::generate_surface(const Uint32 video_flags, const int bpp) const
{
    const int height = (Block::height * NUM_BLOCKS) + (Video::GRID_SIZE * 3);

    SDL_Surface* surface = SDL_CreateRGBSurface(video_flags, Block::width, height, bpp, Video::RMASK, Video::GMASK, Video::BMASK, Video::AMASK);
    if(surface) {
        SDL_Rect r;
        r.x = 0; r.y = 0;
        r.w = Block::width; r.h = Block::height;
        for(int i=0; i<NUM_BLOCKS; i++) {
            SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, 255, 0, 0));
            r.y += (Block::height + Video::GRID_SIZE);
        }
    }
    return surface;
}


/*
 *  J_Tetramino methods
 *
 */


J_Tetramino::J_Tetramino() : Tetramino()
{
    for(int i=0; i<NUM_BLOCKS; i++) {
        m_blocks[i]->set_red(255);
        m_blocks[i]->set_green(255);
        m_blocks[i]->set_blue(255);
    }
    m_center = 1;
}


void J_Tetramino::move_to(const int row, const int column, Tetrion* const tetrion) const
{
    for(int i=0; i<NUM_BLOCKS; i++) tetrion->remove_block(tetrion->get_location(m_blocks[i]));

    tetrion->set_block(row, column, m_blocks[0]);
    tetrion->set_block(row+1, column, m_blocks[1]);
    tetrion->set_block(row+2, column, m_blocks[2]);
    tetrion->set_block(row+2, column-1, m_blocks[3]);
}


SDL_Surface* J_Tetramino::generate_surface(const Uint32 video_flags, const int bpp) const
{
    const int width = (Block::width << 1) + Video::GRID_SIZE;
    const int height = (Block::height * (NUM_BLOCKS - 1)) + (Video::GRID_SIZE << 1);

    SDL_Surface* surface = SDL_CreateRGBSurface(video_flags, width, height, bpp, Video::RMASK, Video::GMASK, Video::BMASK, Video::AMASK);
    if(surface) {
        SDL_Rect r;
        r.x = Block::width + Video::GRID_SIZE; r.y = 0;
        r.w = Block::width; r.h = Block::height;

        int i=0;
        for(i=0; i<NUM_BLOCKS >> 1; i++) {
            SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, 255, 255, 255));
            r.y += (Block::height + Video::GRID_SIZE);
        }

        r.x = 0;
        for(i=0; i<NUM_BLOCKS >> 1; i++) {
            SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, 255, 255, 255));
            r.x += (Block::width + Video::GRID_SIZE);
        }
    }
    return surface;
}


/*
 *  L_Tetramino methods
 *
 */


L_Tetramino::L_Tetramino() : Tetramino()
{
    for(int i=0; i<NUM_BLOCKS; i++) {
        m_blocks[i]->set_red(255);
        m_blocks[i]->set_green(0);
        m_blocks[i]->set_blue(255);
    }
    m_center = 1;
}


void L_Tetramino::move_to(const int row, const int column, Tetrion* const tetrion) const
{
    for(int i=0; i<NUM_BLOCKS; i++) tetrion->remove_block(tetrion->get_location(m_blocks[i]));

    tetrion->set_block(row, column, m_blocks[0]);
    tetrion->set_block(row+1, column, m_blocks[1]);
    tetrion->set_block(row+2, column, m_blocks[2]);
    tetrion->set_block(row+2, column+1, m_blocks[3]);
}


SDL_Surface* L_Tetramino::generate_surface(const Uint32 video_flags, const int bpp) const
{
    const int width = (Block::width << 1) + Video::GRID_SIZE;
    const int height = (Block::height * (NUM_BLOCKS - 1)) + (Video::GRID_SIZE << 1);

    SDL_Surface* surface = SDL_CreateRGBSurface(video_flags, width, height, bpp, Video::RMASK, Video::GMASK, Video::BMASK, Video::AMASK);
    if(surface) {
        SDL_Rect r;
        r.x = 0; r.y = 0;
        r.w = Block::width; r.h = Block::height;

        int i=0;
        for(i=0; i<NUM_BLOCKS >> 1; i++) {
            SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, 255, 0, 255));
            r.y += (Block::height + Video::GRID_SIZE);
        }

        for(i=0; i<NUM_BLOCKS >> 1; i++) {
            SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, 255, 0, 255));
            r.x += (Block::width + Video::GRID_SIZE);
        }
    }
    return surface;
}


/*
 *  O_Tetramino methods
 *
 */


O_Tetramino::O_Tetramino() : Tetramino()
{
    for(int i=0; i<NUM_BLOCKS; i++) {
        m_blocks[i]->set_red(0);
        m_blocks[i]->set_green(0);
        m_blocks[i]->set_blue(255);
    }
    m_center = -1;
}


void O_Tetramino::move_to(const int row, const int column, Tetrion* const tetrion) const
{
    for(int i=0; i<NUM_BLOCKS; i++) tetrion->remove_block(tetrion->get_location(m_blocks[i]));

    tetrion->set_block(row, column, m_blocks[0]);
    tetrion->set_block(row, column+1, m_blocks[1]);
    tetrion->set_block(row+1, column, m_blocks[2]);
    tetrion->set_block(row+1, column+1, m_blocks[3]);
}


SDL_Surface* O_Tetramino::generate_surface(const Uint32 video_flags, const int bpp) const
{
    const int width = (Block::width << 1) + Video::GRID_SIZE;
    const int height = (Block::height << 1) + Video::GRID_SIZE;

    SDL_Surface* surface = SDL_CreateRGBSurface(video_flags, width, height, bpp, Video::RMASK, Video::GMASK, Video::BMASK, Video::AMASK);
    if(surface) {
        SDL_Rect r;
        r.x = 0; r.y = 0;
        r.w = Block::width; r.h = Block::height;

        for(int i=0; i<NUM_BLOCKS >> 1; i++) {
            for(int j=0; j<NUM_BLOCKS >> 1; j++) {
                SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, 0, 0, 255));
                r.x += (Block::width + Video::GRID_SIZE);
            }
            r.x = 0;
            r.y += (Block::height + Video::GRID_SIZE);
        }
    }
    return surface;
}


/*
 *  S_Tetramino methods
 *
 */


S_Tetramino::S_Tetramino() : Tetramino()
{
    for(int i=0; i<NUM_BLOCKS; i++) {
        m_blocks[i]->set_red(0);
        m_blocks[i]->set_green(255);
        m_blocks[i]->set_blue(0);
    }
    m_center = 2;
}


void S_Tetramino::move_to(const int row, const int column, Tetrion* const tetrion) const
{
    for(int i=0; i<NUM_BLOCKS; i++) tetrion->remove_block(tetrion->get_location(m_blocks[i]));

    tetrion->set_block(row, column+1, m_blocks[0]);
    tetrion->set_block(row, column, m_blocks[1]);
    tetrion->set_block(row+1, column, m_blocks[2]);
    tetrion->set_block(row+1, column-1, m_blocks[3]);
}


SDL_Surface* S_Tetramino::generate_surface(const Uint32 video_flags, const int bpp) const
{
    const int width = (Block::width * 3) + (Video::GRID_SIZE << 1);
    const int height = (Block::height << 1) + Video::GRID_SIZE;

    SDL_Surface* surface = SDL_CreateRGBSurface(video_flags, width, height, bpp, Video::RMASK, Video::GMASK, Video::BMASK, Video::AMASK);
    if(surface) {
        SDL_Rect r;
        r.x = (Block::width + Video::GRID_SIZE); r.y = 0;
        r.w = Block::width; r.h = Block::height;

        for(int i=0; i<NUM_BLOCKS >> 1; i++) {
            for(int j=0; j<NUM_BLOCKS >> 1; j++) {
                SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, 0, 255, 0));
                r.x += (Block::width + Video::GRID_SIZE);
            }
            r.x = 0;
            r.y += (Block::height + Video::GRID_SIZE);
        }
    }
    return surface;
}


/*
 *  T_Tetramino methods
 *
 */


T_Tetramino::T_Tetramino() : Tetramino()
{
    for(int i=0; i<NUM_BLOCKS; i++) {
        m_blocks[i]->set_red(100);
        m_blocks[i]->set_green(50);
        m_blocks[i]->set_blue(0);
    }
    m_center = 2;
}


void T_Tetramino::move_to(const int row, const int column, Tetrion* const tetrion) const
{
    for(int i=0; i<NUM_BLOCKS; i++) tetrion->remove_block(tetrion->get_location(m_blocks[i]));

    tetrion->set_block(row, column, m_blocks[0]);
    tetrion->set_block(row+1, column-1, m_blocks[1]);
    tetrion->set_block(row+1, column, m_blocks[2]);
    tetrion->set_block(row+1, column+1, m_blocks[3]);
}


SDL_Surface* T_Tetramino::generate_surface(const Uint32 video_flags, const int bpp) const
{
    const int width = (Block::width * 3) + (Video::GRID_SIZE << 1);
    const int height = (Block::height << 1) + Video::GRID_SIZE;

    SDL_Surface* surface = SDL_CreateRGBSurface(video_flags, width, height, bpp, Video::RMASK, Video::GMASK, Video::BMASK, Video::AMASK);
    if(surface) {
        SDL_Rect r;
        r.x = (Block::width + Video::GRID_SIZE); r.y = 0;
        r.w = Block::width; r.h = Block::height;

        SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, 100, 50, 0));
        r.y += (Block::height + Video::GRID_SIZE);
        r.x = 0;
        for(int i=0; i<NUM_BLOCKS-1; i++) {
            SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, 100, 50, 0));
            r.x += (Block::width + Video::GRID_SIZE);
        }
    }
    return surface;
}


/*
 *  Z_Tetramino methods
 *
 */


Z_Tetramino::Z_Tetramino() : Tetramino()
{
    for(int i=0; i<NUM_BLOCKS; i++) {
        m_blocks[i]->set_red(0);
        m_blocks[i]->set_green(255);
        m_blocks[i]->set_blue(255);
    }
    m_center = 2;
}


void Z_Tetramino::move_to(const int row, const int column, Tetrion* const tetrion) const
{
    for(int i=0; i<NUM_BLOCKS; i++) tetrion->remove_block(tetrion->get_location(m_blocks[i]));

    tetrion->set_block(row, column-1, m_blocks[0]);
    tetrion->set_block(row, column, m_blocks[1]);
    tetrion->set_block(row+1, column, m_blocks[2]);
    tetrion->set_block(row+1, column+1, m_blocks[3]);
}


SDL_Surface* Z_Tetramino::generate_surface(const Uint32 video_flags, const int bpp) const
{
    const int width = (Block::width * 3) + (Video::GRID_SIZE << 1);
    const int height = (Block::height << 1) + Video::GRID_SIZE;

    SDL_Surface* surface = SDL_CreateRGBSurface(video_flags, width, height, bpp, Video::RMASK, Video::GMASK, Video::BMASK, Video::AMASK);
    if(surface) {
        SDL_Rect r;
        r.x = 0; r.y = 0;
        r.w = Block::width; r.h = Block::height;

        for(int i=0; i<NUM_BLOCKS >> 1; i++) {
            for(int j=0; j<NUM_BLOCKS >> 1; j++) {
                SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, 0, 255, 255));
                r.x += (Block::width + Video::GRID_SIZE);
            }
            r.x -= (Block::width + Video::GRID_SIZE);
            r.y += (Block::height + Video::GRID_SIZE);
        }
    }
    return surface;
}
