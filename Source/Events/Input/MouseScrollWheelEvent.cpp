//
//  MouseScrollWheelEvent.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-22.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "MouseScrollWheelEvent.h"


namespace GameDev2D
{
    MouseScrollWheelEvent::MouseScrollWheelEvent(float aDelta) : Event("MouseScrollWheelEvent"),
        m_Delta(aDelta)
    {
        SetEventCode(MOUSE_WHEEL_EVENT);
    }
    
    MouseScrollWheelEvent::~MouseScrollWheelEvent()
    {
    
    }
        
    float MouseScrollWheelEvent::GetDetla()
    {
        return m_Delta;
    }
    
    void MouseScrollWheelEvent::LogEvent()
    {
        Log(VerbosityLevel_Input, "%f", m_Delta);
    }
}