//
//  AccelerometerEvent.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-04-24.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__AccelerometerEvent__
#define __GameDev2D__AccelerometerEvent__

#include "../Event.h"


using namespace glm;

namespace GameDev2D
{
    //The AccelerometerEvent class is a subclass of the event class and is sent by the
    //InputManager when new accelerometer data is recieved.
    class AccelerometerEvent : public Event
    {
    public:
        AccelerometerEvent();
        AccelerometerEvent(double x, double y, double z);
        ~AccelerometerEvent();
        
        //Returns the acceleration for all 3-axis
        dvec3 GetAcceleration();
        
    protected:
        void Set(double x, double y, double z);
        
        void LogEvent();
        
        friend class InputManager;
      
    private:
        dvec3 m_Acceleration;
    };
}

#endif /* defined(__GameDev2D__AccelerometerEvent__) */
