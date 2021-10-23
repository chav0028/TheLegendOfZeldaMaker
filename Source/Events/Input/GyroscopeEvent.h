//
//  GyroscopeEvent.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-04-24.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__GyroscopeEvent__
#define __GameDev2D__GyroscopeEvent__

#include "../Event.h"


using namespace glm;

namespace GameDev2D
{
    //The GyroscopeEvent class is a subclass of the event class and is sent by the
    //InputManager when new gyroscope data is recieved.
    class GyroscopeEvent : public Event
    {
    public:
        GyroscopeEvent();
        GyroscopeEvent(double x, double y, double z);
        ~GyroscopeEvent();
        
        //Returns the rotation of the device around all 3-axis
        dvec3 GetRotation();
        
    protected:
        void Set(double x, double y, double z);
        
        void LogEvent();
        
        friend class InputManager;
      
    private:
        dvec3 m_Rotation;
    };
}

#endif /* defined(__GameDev2D__GyroscopeEvent__) */
