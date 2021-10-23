//
//  GyroscopeEvent.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-04-24.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "GyroscopeEvent.h"


namespace GameDev2D
{
    GyroscopeEvent::GyroscopeEvent() : Event("GyroscopeEvent"),
        m_Rotation(0.0, 0.0, 0.0)
    {
        SetEventCode(GYROSCOPE_EVENT);
    }

    GyroscopeEvent::GyroscopeEvent(double aX, double aY, double aZ) : Event("GyroscopeEvent"),
        m_Rotation(aX, aY, aZ)
    {
        SetEventCode(GYROSCOPE_EVENT);
    }
    
    GyroscopeEvent::~GyroscopeEvent()
    {
    
    }
        
    dvec3 GyroscopeEvent::GetRotation()
    {
        return m_Rotation;
    }
    
    void GyroscopeEvent::Set(double aX, double aY, double aZ)
    {
        m_Rotation = dvec3(aX, aY, aZ);
    }
    
    void GyroscopeEvent::LogEvent()
    {
        Log(VerbosityLevel_Input, "(%f, %f, %f)", m_Rotation.x, m_Rotation.y, m_Rotation.z);
    }
}