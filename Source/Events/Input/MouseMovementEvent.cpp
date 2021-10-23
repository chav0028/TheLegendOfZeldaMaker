//
//  MouseMovementEvent.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "MouseMovementEvent.h"


namespace GameDev2D
{
    MouseMovementEvent::MouseMovementEvent() : Event("MouseMovementEvent"),
        m_Position(0.0f, 0.0f),
        m_DeltaPosition(0.0f, 0.0f)
    {
        SetEventCode(MOUSE_MOVEMENT_EVENT);
    }
    
    MouseMovementEvent::MouseMovementEvent(float aX, float aY, float aDeltaX, float aDeltaY) : Event("MouseMovementEvent"),
        m_Position(aX, aY),
        m_DeltaPosition(aDeltaX, aDeltaY)
    {
        SetEventCode(MOUSE_MOVEMENT_EVENT);
    }

    MouseMovementEvent::~MouseMovementEvent()
    {
    
    }
        
    vec2 MouseMovementEvent::GetPosition()
    {
        return m_Position;
    }
    
    vec2 MouseMovementEvent::GetDeltaPosition()
    {
        return m_DeltaPosition;
    }

    void MouseMovementEvent::Set(float aX, float aY, float aDeltaX, float aDeltaY)
    {
        m_Position = vec2(aX, aY);
        m_DeltaPosition = vec2(aDeltaX, aDeltaY);
    }
    
    void MouseMovementEvent::LogEvent()
    {
        Log(VerbosityLevel_Input, "(%f, %f) - Delta(%f, %f)", m_Position.x, m_Position.y, m_DeltaPosition.x, m_DeltaPosition.y);
    }
}