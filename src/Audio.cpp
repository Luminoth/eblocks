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

#include "SDL.h"

#include "Audio.h"


/*
==========
Sound structure

holds the information for a sound
==========
*/


struct Sound {
    Uint8* data;
    Uint32 pos;
    Uint32 len;

    bool music;
};


/*
 *  Audio class variables
 *
 */


std::map<std::string, Sound*> Audio::sound_map;


/*
 *  Audio class functions
 *
 */


void Audio::unload_sounds()
{
    for(std::map<std::string, Sound*>::iterator it = sound_map.begin(); it != sound_map.end(); ++it) {
        delete[] it->second->data;
        delete it->second;
        sound_map.erase(it);
    }
}


/*
 *  Audio methods
 *
 */


Audio::Audio() : m_format(NULL), m_good(true)
{
    if(-1 == SDL_Init(SDL_INIT_AUDIO)) m_good = false;
}


Audio::~Audio()
{
    if(SDL_WasInit(SDL_INIT_AUDIO)) {
        stop();
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }

    if(m_format) delete m_format;
}


void mixaudio(void *unused, Uint8 *stream, int len)
{
    // this is pretty much straight out of the SDL tutorials
    for(std::map<std::string, Sound*>::iterator it = Audio::sound_map.begin(); it != Audio::sound_map.end(); ++it) {
        int amount = it->second->len - it->second->pos;
        if(amount > len) amount = len;

/* FIXME: have a setable volume somehow */
        SDL_MixAudio(stream, &it->second->data[it->second->pos], amount, SDL_MIX_MAXVOLUME);
        it->second->pos += amount;

        // loop music sounds
        if(it->second->music && it->second->pos >= it->second->len)
            it->second->pos = 0;
    }
}


const bool Audio::start()
{
    // 16-bit stereo at 22Khz
/* FIXME: this should be a setable thing */
    SDL_AudioSpec desired;
    memset(&desired, 0, sizeof(SDL_AudioSpec));
    desired.freq     = 22050;
    desired.format   = AUDIO_S16;
    desired.channels = 2;
    desired.samples  = 512;         // good value for games (but is it the best?)
    desired.callback = mixaudio;
    desired.userdata = NULL;

    m_format = new SDL_AudioSpec;
    memset(m_format, 0, sizeof(SDL_AudioSpec));
    if(SDL_OpenAudio(&desired, m_format) < 0)
        return false;
    std::cout << "Audio starting with " << m_format->freq << " frequency and " << static_cast<int>(m_format->channels) << " channels" << std::endl;

    SDL_PauseAudio(0);
    return true;
}


void Audio::stop()
{
    if(m_format) {
        delete m_format;
        m_format = NULL;
    }
    SDL_CloseAudio();
}


Sound* const load_file(const std::string& filename, const Uint16 format, const Uint8 channels, const int rate)
{
    // load the wave data
    SDL_AudioSpec wave;
    Uint8* data = NULL;
    Uint32 len = 0;
    if(!SDL_LoadWAV(filename.c_str(), &wave, &data, &len))
        return NULL;

    // convert the wave data
    SDL_AudioCVT cvt;
    SDL_BuildAudioCVT(&cvt, wave.format, wave.channels, wave.freq, format, channels, rate);
    cvt.buf = new Uint8[len * cvt.len_mult];
    memcpy(cvt.buf, data, len);
    cvt.len = len;
    SDL_ConvertAudio(&cvt);

    SDL_FreeWAV(data);

    // create the new sounds
    Sound* sound = new Sound;
    sound->music = false;
    SDL_LockAudio();
        sound->data = cvt.buf;
        sound->len  = cvt.len_cvt;
        sound->pos  = 0;
    SDL_UnlockAudio();
    return sound;
}


const bool Audio::load_sound(const std::string& filename)
{
    // only load it if we haven't already
    std::map<std::string, Sound*>::iterator it = sound_map.find(filename);
    if(it == sound_map.end()) {
        Sound* sound = load_file(filename, m_format->format, m_format->channels, m_format->freq);
        if(!sound) {
            std::cerr << "There was an error loading sound: " << SDL_GetError() << std::endl;
            return false;
        }
        sound_map[filename] = sound;
    } else it->second->music = false;
    return true;
}


const bool Audio::load_music(const std::string& filename)
{
    // only load it if we haven't already
    std::map<std::string, Sound*>::iterator it = sound_map.find(filename);
    if(it == sound_map.end()) {
        Sound* music = load_file(filename, m_format->format, m_format->channels, m_format->freq);
        if(!music) {
            std::cerr << "There was an error loading music: " << SDL_GetError() << std::endl;
            return false;
        }
        music->music = true;
        sound_map[filename] = music;
    } else it->second->music = true;
    return true;
}


const bool Audio::play_sound(const std::string& filename)
{
    // if it exists and is finished, reset it
    std::map<std::string, Sound*>::iterator it = sound_map.find(filename);
    if(it != sound_map.end()) {
        if(it->second->pos >= it->second->len)
            it->second->pos = 0;
        return true;
    }

    // load the sound
    Sound* sound = load_file(filename, m_format->format, m_format->channels, m_format->freq);
    if(!sound) {
        std::cerr << "There was an error loading sound: " << SDL_GetError() << std::endl;
        return false;
    }
    sound_map[filename] = sound;
    return true;
}


const bool Audio::play_music(const std::string& filename)
{
    // if it exists and is finished, reset it (and turn it on)
    std::map<std::string, Sound*>::iterator it = sound_map.find(filename);
    if(it != sound_map.end()) {
        if(it->second->pos >= it->second->len) it->second->pos = 0;
        if(!it->second->music) it->second->music = true;
        return true;
    }

    // load the music
    Sound* music = load_file(filename, m_format->format, m_format->channels, m_format->freq);
    if(!music) {
        std::cerr << "There was an error loading music: " << SDL_GetError() << std::endl;
        return false;
    }
    music->music = true;
    sound_map[filename] = music;
    return true;
}


void Audio::stop_music()
{
    for(std::map<std::string, Sound*>::iterator it = sound_map.begin(); it != sound_map.end(); ++it) {
        if(it->second->music) {
            it->second->music = false;
            it->second->pos = it->second->len;
        }
    }
}
