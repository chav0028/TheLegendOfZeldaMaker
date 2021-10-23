//
//  ResizeEvent.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-21.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__PlatformResizeEvent__
#define __GameDev2D__PlatformResizeEvent__

#include "../Event.h"


using namespace glm;

namespace GameDev2D
{
    //The ResizeEvent class is a subclass of the Event class and is sent by the
    //PlatformLayer whenever screen is resized
    class ResizeEvent : public Event
    {
    public:
        ResizeEvent(vec2 size);
        ~ResizeEvent();
        
        //Returns the new size of the screen
        vec2 GetSize();
      
    protected:
        void LogEvent();
      
    private:
        vec2 m_Size;
    };
}

#endif /* defined(__GameDev2D__PlatformResizeEvent__) */
