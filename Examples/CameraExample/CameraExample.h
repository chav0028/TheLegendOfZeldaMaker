//
//  CameraExample.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-08-12.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__CameraExample__
#define __GameDev2D__CameraExample__

#include "../../Source/Core/Scene.h"


namespace GameDev2D
{
    class Sprite;
    class Rect;

    class CameraExample : public Scene
    {
    public:
        CameraExample();
        ~CameraExample();
        
        //Load all Game content in this method
        float LoadContent();

        //Handles input events used to trigger Sprite animations
        void HandleEvent(Event* event);
        
        //Resets everything back to its original state
        void Reset();
        
    private:
        //Member variables
        Sprite* m_AlgonquinLogo;
        Rect* m_BackgroundRect;
    };
}

#endif /* defined(__GameDev2D__CameraExample__) */
