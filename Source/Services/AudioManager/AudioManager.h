//
//  AudioManager.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-04-29.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__AudioManager__
#define __GameDev2D__AudioManager__

#include "../GameService.h"
#include <fmod.hpp>


using namespace std;

namespace GameDev2D
{
    //Forward declarations
    class Audio;

    //Local constants
    const unsigned int MAX_AUDIO_CHANNELS = 32;

    //The AudioManager leverages the FMOD library to load and play audio files. It can load
    //complete sounds (for SFX) or stream larger audio files (for background music). You can
    //load as many 'sounds' as you want (up to 32 at a time), but the AudioManager limits
    //streams to one at time and uses reference counting to ensures only one will be loaded.
    class AudioManager : public GameService
    {
    public:
        AudioManager();
        ~AudioManager();
        
        //Used to update the FMOD system
        void Update(double delta);
        
        //Returns the max number of channels that are available
        unsigned int GetMaxChannels();
        
        //Used to determine if the AudioManager should be updated and drawn
        bool CanUpdate();
        bool CanDraw();
        
        //Handles events, we need to handle a resume event
        void HandleEvent(Event* event);
    
    protected:
        //Used to load and unload sound, access by the Audio class
        FMOD::Sound* LoadSound(FMOD::System** system, const string& filename, const string& extension, bool streamed, bool looped);
        bool UnloadSound(FMOD::Sound* sound, const string& key);
        
        //Conveniance method to register and unregister Audio objects
        void RegisterAudio(Audio* audio);
        void UnregisterAudio(Audio* audio);
        
        //Checks to see if an audio object is already loaded
        bool ContainsAudio(Audio* audio);
        
        //The Audio class needs to access the LoadSound() and UnloadSound() methods
        friend class Audio;
    
    private:
        //Member variables
        FMOD::System* m_System;
        map<string, pair<FMOD::Sound*, unsigned int>> m_SoundMap;
        vector<FMOD::Sound*> m_Streams;
        vector<Audio*> m_AudioSources;
    };
}


#endif /* defined(__GameDev2D__AudioManager__) */
