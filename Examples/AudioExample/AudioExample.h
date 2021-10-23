//
//  AudioExample.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-08-11.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__AudioExample__
#define __GameDev2D__AudioExample__

#include "../../Source/Core/Scene.h"


namespace GameDev2D
{
    class Audio;

    class AudioExample : public Scene
    {
    public:
        AudioExample();
        ~AudioExample();
        
        //Load all Game content in this method (including audio)
        float LoadContent();

        //Handles input events used to trigger Audio playback
        void HandleEvent(Event* event);
        
    private:
        //Member variables
        Audio* m_Music;
        Audio* m_SoundEffect;
    };
}

#endif /* defined(__GameDev2D__AudioExample__) */
