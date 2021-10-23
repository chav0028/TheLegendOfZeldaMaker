//
//  FullscreenEvent.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-12.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__FullscreenEvent__
#define __GameDev2D__FullscreenEvent__

#include "../Event.h"


namespace GameDev2D
{
    //The FullscreenEvent class is a subclass of the Event class and is sent by the
    //PlatformLayer whenever the application enteres or exits fullscreen mode
    class FullscreenEvent : public Event
    {
    public:
        FullscreenEvent(bool isFullscreen);
        ~FullscreenEvent();
        
        //Retusn whether the application is in fullscreen mode or not
        bool IsFullscreen();
      
    protected:
        void LogEvent();
      
    private:
        bool m_IsFullscreen;
    };
}

#endif /* defined(__GameDev2D__FullscreenEvent__) */
