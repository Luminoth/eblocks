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


#if !defined AUDIO_H
#define AUDIO_H


#include <string>
#include <map>


struct SDL_AudioSpec;
struct Sound;


/*
==========
Audio class

used for audio related stuff and things
==========
*/


class Audio
{
private:
    static std::map<std::string, Sound*> sound_map;

public:
    /* NOTE: if you load sounds, this must be called to unload all of them */
    static void unload_sounds();

public:
    Audio();
    ~Audio();

public:
    // starts audio
    const bool start();

    // stops audio
    void stop();

    // loads a sound/music, but doesn't play it
    const bool load_sound(const std::string& filename);
    const bool load_music(const std::string& filename);

    // plays the sound at file
    // this loads the sound if necessary
    const bool play_sound(const std::string& filename);

    // starts/stops a looping sound (music)
    // more than one looping sounds can happen, but stop stops them all
    const bool play_music(const std::string& filename);
    void stop_music();

public:
    const bool is_good() const { return m_good; }

private:
    friend void mixaudio(void* unused, Uint8* stream, int len);

private:
    SDL_AudioSpec* m_format;
    bool m_good;

private:
    Audio(const Audio& audio) {}
};


#endif
