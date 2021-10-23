//
//  SpriteExample.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-08-11.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__SpriteExample__
#define __GameDev2D__SpriteExample__

#include "../../Source/Core/Scene.h"


namespace GameDev2D
{
    class Sprite;

    class SpriteExample : public Scene
    {
    public:
        SpriteExample();
        ~SpriteExample();
        
        //Load all Game content in this method (including audio)
        float LoadContent();

        //Handles input events used to trigger Sprite animations
        void HandleEvent(Event* event);
        
        //Resets everything back to its original state
        void Reset();
        
    private:
        //Member variables
        Sprite* m_AlgonquinLogo1;
        Sprite* m_AlgonquinLogo2;
        Sprite* m_AlgonquinAnimatedLogo;
    };
}

#endif /* defined(__GameDev2D__SpriteExample__) */
