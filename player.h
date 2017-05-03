//
//  player.hpp
//  final_graficas
//
//  Created by Ruben Cuadra on 5/2/17.
//  Copyright © 2017 Ruben Cuadra. All rights reserved.
//

#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#undef main

#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>

class Player
{
private:
    static const std::string SOUNDE;
    static const std::string SOUNDT;
    static int currentE , currentT;
    static bool forcedStop;
    
protected:
    std::vector<Mix_Music *> songs;
    std::vector<Mix_Chunk *> effects;
    std::vector<std::string> songs_names;
    
    static Player * instance;
    Player()
    {
        if( SDL_Init(SDL_INIT_AUDIO) < 0 ) exit(1);
        // Setup audio mode
        Mix_OpenAudio(22050,AUDIO_S16SYS,2,640);
        Mix_HookMusicFinished( &Player::onStop );
        Mix_VolumeMusic(MIX_MAX_VOLUME);
        Mix_Music *temp_music;
        Mix_Chunk *temp_chunk;
        
        for (auto i = boost::filesystem::directory_iterator(SOUNDT); i != boost::filesystem::directory_iterator(); i++)
        {
            if (!is_directory(i->path())) //we eliminate directories
            {
                temp_music = Mix_LoadMUS( i->path().c_str() );
                if(temp_music) //(.wav, .mod .s3m .it .xm)
                {
                    songs.push_back( temp_music ) ;
                    songs_names.push_back( i->path().filename().string() );
                }
            }
        }
        
        for (auto i = boost::filesystem::directory_iterator(SOUNDE); i != boost::filesystem::directory_iterator(); i++)
        {
            if (!is_directory(i->path())) //we eliminate directories
            {
                temp_chunk = Mix_LoadWAV( i->path().c_str() );
                if(temp_chunk)
                    effects.push_back( temp_chunk );
            }
        }
        if (songs.empty() || effects.empty())
        {
            std::cout<<"Debe existir por lo menos 1 effecto y 1 soundtrack con el formato '.wav'\n";
            exit(0);
        }
    }
    
public:
    static Player * getInstance()
    {
        if(instance == nullptr)
            instance = new Player();
        return instance;
    }
    void FreeAudio()
    {
        for (int i = 0; i<songs.size(); ++i)
            Mix_FreeMusic( songs[i] );
        
        for (int i = 0; i<effects.size(); ++i)
            Mix_FreeChunk( effects[i] );
       
    }
    
    static void onStop() //Callback nos pide static
    {
        if(forcedStop) //Aqui es un caso de FadeOut o Stop manual, ignorar esta funcion
        {
            forcedStop = false; //Regresar a caso normal
        }
        else          //Aqui es que se acabo la rola
        {
            getInstance()->playNext();
        }
    }
    void togglePlay()
    {
        if (Mix_PlayingMusic() == 1)
        {
            if (Mix_PausedMusic() == 1)
            {
                Mix_ResumeMusic();
            }
            else
            {
                forcedStop = true;
                Mix_PauseMusic();
            }
        }
    }
    
    void playSong(int index)
    {
        if (Mix_PlayingMusic() == 1 && Mix_PausedMusic() == 0)
        {
            forcedStop = true;
            Mix_FadeOutMusic(1000); //Instead of Pause
        }
        currentT = index%songs.size();
        Mix_PlayMusic( songs[ currentT ] , 0);
    }
    
    void playNext()
    {
        if (Mix_PlayingMusic() == 1 && Mix_PausedMusic() == 0)
        {
            forcedStop = true;
            Mix_FadeOutMusic(1000); //Instead of Pause
        }
        currentT = (currentT+1)%songs.size();
        Mix_PlayMusic( songs[ currentT ] , 0);
    }
    
    void playPrev()
    {
        if (Mix_PlayingMusic() == 1 && Mix_PausedMusic() == 0)
        {
            forcedStop = true;
            Mix_PauseMusic();
        }
        currentT = (currentT-1>0)?(currentT-1):(songs.size()-1);
        Mix_PlayMusic( songs[ currentT ] , 0);
    }
    
    void playEffect(int index)
    {
        Mix_PlayChannel(-1, effects[index%effects.size()], 0); //hara un sonido, repetir 0 veces en canal disponible(-1)
    }
    
    ~Player()
    {
        FreeAudio();
    }
    
    std::string getCurrentPlaying()
    {
        if( Mix_PlayingMusic() == 1 && Mix_PausedMusic() == 0)
            return  songs_names[currentT] ;
        return " ";
    }
};

//Necesario inicializar statics
Player * Player::instance = 0;
bool Player::forcedStop = false;
int Player::currentE = -1;
int Player::currentT = -1;
const std::string Player::SOUNDE = "assets/audio/effects";
const std::string Player::SOUNDT = "assets/audio/soundtrack";




