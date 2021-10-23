//
//  OrientationChangedEvent.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-04-19.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__OrientationChangedEvent__
#define __GameDev2D__OrientationChangedEvent__

#include "../Event.h"
#include "../../Platforms/PlatformLayer.h"


namespace GameDev2D
{
    //The OrientationChangedEvent class is a subclass of the Event class and is sent by the
    //PlatformLayer whenever orientation of the device changes, only gets sent on iOS
    class OrientationChangedEvent : public Event
    {
    public:
        OrientationChangedEvent(Orientation orientation);
        ~OrientationChangedEvent();
        
        //Returns the new orientation of the device
        Orientation GetOrientation();
      
    protected:
        void LogEvent();  
    
    private:
        Orientation m_Orientation;
    };
}

#endif /* defined(__GameDev2D__OrientationChangedEvent__) */
