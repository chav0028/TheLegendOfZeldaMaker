//
//  AudioManager.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-04-29.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "AudioManager.h"
#include "../ServiceLocator.h"
#include "../../Audio/Audio.h"
#include "../../Platforms/PlatformLayer.h"


#if TARGET_OS_IPHONE
#include "fmodiphone.h"
#endif


namespace GameDev2D
{
    AudioManager::AudioManager() : GameService("AudioManager"),
        m_System(nullptr)
    {
        //Create the FMOD system
        if(FMOD::System_Create(&m_System) != FMOD_OK)
        {
            Error(true, "The Audio Manager failed to initialize FMOD");
        }
        
        //Initialize the audio channels for the FMOD system
        if(m_System->init(MAX_AUDIO_CHANNELS, FMOD_INIT_NORMAL, NULL) != FMOD_OK)
        {
            Error(true, "The Audio Manager failed to initialize FMOD");
        }
        
        //Add a resume event listener
        ServiceLocator::GetPlatformLayer()->AddEventListener(this, RESUME_EVENT);
    }

    AudioManager::~AudioManager()
    {
        //Remove the resume event listener
        ServiceLocator::GetPlatformLayer()->RemoveEventListener(this, RESUME_EVENT);
        
        //Release any streams that are remaining
        while(m_Streams.empty() == false)
        {
            m_Streams.back()->release();
            m_Streams.pop_back();
        }
        
        //Cycle through and release any existing system sounds
        while(m_SoundMap.size() > 0)
        {
            m_SoundMap.begin()->second.first->release();
            m_SoundMap.erase(m_SoundMap.begin());
        }
        
        //Release the FMOD system
        if(m_System != nullptr)
        {
            m_System->release();
            m_System = nullptr;
        }
    }

    void AudioManager::Update(double aDelta)
    {
        //Update the FMOD system
        if(m_System != nullptr)
        {
            m_System->update();
        }
        
        //Update the audio sources
        for(unsigned int i = 0; i < m_AudioSources.size(); i++)
        {
            m_AudioSources.at(i)->Update(aDelta);
        }
    }
    
    FMOD::Sound* AudioManager::LoadSound(FMOD::System** aSystem, const string& aFilename, const string& aExtension, bool aStreamed, bool aLooped)
    {
        //Safety check the filename
        if(aFilename.length() == 0)
        {
            Error(false, "Failed to load sound, the filename had a length of 0");
            return nullptr;
        }
        
        //Set the FMOD system pointer
        *aSystem = m_System;
    
        //Is it a streamed sound we are loading?
        if(aStreamed == true)
        {
            //Get the path for the texture
            string path = ServiceLocator::GetPlatformLayer()->GetPathForResourceInDirectory(aFilename.c_str(), aExtension.c_str(), "Audio");
            
            //Does the file exist, if it doesn't the assert below will be hit
            bool doesExist = ServiceLocator::GetPlatformLayer()->DoesFileExistAtPath(path);
            assert(doesExist == true);
            
            //If the file doesn't exist, log an error
            if(doesExist == false)
            {
                Error(false, "Failed to load audio stream with filename: %s, it doesn't exist", aFilename.c_str());
                return nullptr;
            }
        
            //Set the mode for the sound
            FMOD::Sound* sound = nullptr;
            FMOD_MODE mode = (aLooped == true ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF) | FMOD_2D | FMOD_HARDWARE;
            
            //Create the sound stream
            if(m_System->createStream(path.c_str(), mode, 0, &sound) != FMOD_OK)
            {
                Error(false, "Failed to load audio stream with filename: %s, an error occured during creation");
                sound = nullptr;
            }

            //Add the sound object to the streams vector
            if(sound != nullptr)
            {
                m_Streams.push_back(sound);
            }

            //Return the sound object
            return sound;
        }
        else
        {
            //Create a key for the filename and extension
            string key = aFilename + "." + aExtension;
        
            //Check to see if the Audio object is already loaded in the sound map
            pair<FMOD::Sound*, unsigned int> soundPair = m_SoundMap[key];
            
            //Is the sound pointer null? That means it doesn't exist yet
            if(soundPair.first == nullptr)
            {
                //Get the path for the texture
                string path = ServiceLocator::GetPlatformLayer()->GetPathForResourceInDirectory(aFilename.c_str(), aExtension.c_str(), "Audio");
                
                //Does the file exist, if it doesn't the assert below will be hit
                bool doesExist = ServiceLocator::GetPlatformLayer()->DoesFileExistAtPath(path);
                assert(doesExist == true);
                
                //If the file doesn't exist, log an error
                if(doesExist == false)
                {
                    Error(false, "Failed to load audio with filename: %s, it doesn't exist", aFilename.c_str());
                    return nullptr;
                }
            
                //Set the mode for the sound
                FMOD::Sound* sound = nullptr;
                FMOD_MODE mode = (aLooped == true ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF) | FMOD_2D | FMOD_HARDWARE;
                
                //Create the sound
                if(m_System->createSound(path.c_str(), mode, 0, &sound) != FMOD_OK)
                {
                    //If it fails log an error
                    Error(false, "Failed to load audio with filename: %s, an error occured during creation");
                    return nullptr;
                }
                else
                {
                    //Set the sound pointer and set the reference count to 1
                    soundPair.first = sound;
                    soundPair.second = 1;
                }
            }
            else
            {
                //The sound is already loaded increment the reference count
                soundPair.second++;
            }
            
            //Set the sound pair
            m_SoundMap[key] = soundPair;
            
            //And return the Audio object
            return soundPair.first;
        }

        //If we got here, we failed to load a sound, return null
        return nullptr;
    }
    
    bool AudioManager::UnloadSound(FMOD::Sound* aSound, const string& aKey)
    {
        //If the key is empty, then it is a streamed sound
        if(aKey == "")
        {
            //Cycle through the streams vector
            int index = -1;
            for(unsigned int i = 0; i < m_Streams.size(); i++)
            {
                if(m_Streams.at(i) == aSound)
                {
                    index = i;
                    break;
                }
            }
            
            //If the assert is hit, that means the sound you are trying to unload hasn't been loaded
            assert(index != -1);
            
            //Safety check that the index isnt -1
            if(index != -1)
            {
                //Erase the the stream
                m_Streams.erase(m_Streams.begin() + index);
                
                //Release the sound
                aSound->release();
                
                //Return success!
                return true;
            }
        }
        else
        {
            //Get the sound pair for the key
            pair<FMOD::Sound*, unsigned int> soundPair = m_SoundMap[aKey];
            
            //Safety check the sound pair
            if(soundPair.first == aSound)
            {
                //Subract from the retain counter
                soundPair.second--;
                
                //Has the counter reacher zero?
                if(soundPair.second == 0)
                {
                    //Release the sound object
                    aSound->release();

                    //Erase the name from the map
                    m_SoundMap.erase(aKey);
                }
                else
                {
                    //Set the sound map pair for the name key
                    m_SoundMap[aKey] = soundPair;
                }
                
                //Return success!
                return true;
            }
        }

        //If we got here, we failed to unload the sound
        return false;
    }
    
    void AudioManager::RegisterAudio(Audio* aAudio)
    {
        if(aAudio != nullptr)
        {
            m_AudioSources.push_back(aAudio);
        }
    }
    
    void AudioManager::UnregisterAudio(Audio* aAudio)
    {
        int index = -1;
        for(unsigned int i = 0; i < m_AudioSources.size(); i++)
        {
            if(m_AudioSources.at(i) == aAudio)
            {
                index = i;
                break;
            }
        }
        
        //If the assert is hit, that means the Audio object you are trying to unregister hasn't been registered
        assert(index != -1);
        
        //Safety check the index and erase the audio source
        if(index != -1)
        {
            m_AudioSources.erase(m_AudioSources.begin() + index);
        }
    }
    
    unsigned int AudioManager::GetMaxChannels()
    {
        return MAX_AUDIO_CHANNELS;
    }

    bool AudioManager::CanUpdate()
    {
        return true;
    }
    
    bool AudioManager::CanDraw()
    {
      return false;
    }
    
    void AudioManager::HandleEvent(Event* aEvent)
    {
        switch (aEvent->GetEventCode())
        {
          case RESUME_EVENT:
            #if TARGET_OS_IPHONE
            FMOD_IPhone_RestoreAudioSession();
            #endif
            break;

          default:
            break;
        }
    }
}