//
//  AccelerometerEvent.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-04-24.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "AccelerometerEvent.h"


namespace GameDev2D
{
    AccelerometerEvent::AccelerometerEvent() : Event("AccelerometerEvent"),
        m_Acceleration(0.0, 0.0, 0.0)
    {
        SetEventCode(ACCELEROMETER_EVENT);
    }

    AccelerometerEvent::AccelerometerEvent(double aX, double aY, double aZ) : Event("AccelerometerEvent"),
        m_Acceleration(aX, aY, aZ)
    {
        SetEventCode(ACCELEROMETER_EVENT);
    }
    
    AccelerometerEvent::~AccelerometerEvent()
    {
    
    }
        
    dvec3 AccelerometerEvent::GetAcceleration()
    {
        return m_Acceleration;
    }
    
    void AccelerometerEvent::Set(double aX, double aY, double aZ)
    {
        m_Acceleration = dvec3(aX, aY, aZ);
    }
    
    void AccelerometerEvent::LogEvent()
    {
        Log(VerbosityLevel_Input, "(%f, %f, %f)", m_Acceleration.x, m_Acceleration.y, m_Acceleration.z);
    }
}
