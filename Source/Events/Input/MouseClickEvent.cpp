//
//  MouseClickEvent.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-22.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "MouseClickEvent.h"


namespace GameDev2D
{
    MouseClickEvent::MouseClickEvent(MouseClickEventType aMouseClickEventType, MouseButton aMouseButton, vec2 aPosition, double aDuration) : Event("MouseClickEvent"),
        m_MouseClickEventType(aMouseClickEventType),
        m_MouseButton(aMouseButton),
        m_Position(aPosition),
        m_Duration(aDuration)
    {
        SetEventCode(MOUSE_CLICK_EVENT);
    }
    
    MouseClickEvent::~MouseClickEvent()
    {
    
    }
        
    MouseClickEventType MouseClickEvent::GetMouseClickEventType()
    {
        return m_MouseClickEventType;
    }
    
    MouseButton MouseClickEvent::GetMouseButton()
    {
        return m_MouseButton;
    }
    
    vec2 MouseClickEvent::GetPosition()
    {
        return m_Position;
    }
    
    double MouseClickEvent::GetDuration()
    {
        return m_Duration;
    }
    
    void MouseClickEvent::LogEvent()
    {
        Log(VerbosityLevel_Input, "%s(%s) - (%f, %f) - Duration: %f seconds", MouseClickEventTypeName(m_MouseClickEventType), MouseButtonName(m_MouseButton), m_Position.x, m_Position.y, m_Duration);
    }
    
    const char* MouseClickEventTypeName(MouseClickEventType aMouseClickEventType)
    {
        switch (aMouseClickEventType)
        {
            case MouseClickDown:
                return "MouseClickDown";
            
            case MouseClickUp:
                return "MouseClickUp";
                
            default:
                break;
        }
        return "Unknown";
    }
}