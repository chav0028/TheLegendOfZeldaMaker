//
//  KeyEvent.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "KeyEvent.h"


namespace GameDev2D
{
    KeyEvent::KeyEvent(KeyEventType aKeyEventType, KeyCode aKeycode, double aDuration) : Event("KeyEvent"),
        m_KeyEventType(aKeyEventType),
        m_KeyCode(aKeycode),
        m_Duration(aDuration)
    {
        SetEventCode(KEYBOARD_EVENT);
    }
    
    KeyEvent::~KeyEvent()
    {
    
    }
    
    KeyEventType KeyEvent::GetKeyEventType()
    {
        return m_KeyEventType;
    }
    
    KeyCode KeyEvent::GetKeyCode()
    {
        return m_KeyCode;
    }
    
    double KeyEvent::GetDuration()
    {
        return m_Duration;
    }
    
    void KeyEvent::LogEvent()
    {
        if(m_KeyEventType == KeyDown)
        {
            Log(VerbosityLevel_Input, "%s(%u) - %s", KeyEventTypeName(m_KeyEventType), m_KeyCode, KeyCodeString(m_KeyCode));
        }
        else
        {
            Log(VerbosityLevel_Input, "%s(%u) - %s - Duration: %f seconds", KeyEventTypeName(m_KeyEventType), m_KeyCode, KeyCodeString(m_KeyCode), m_Duration);
        }
    }
    
    const char* KeyEventTypeName(KeyEventType aKeyEventType)
    {
        switch (aKeyEventType)
        {
            case KeyDown:
                return "KeyDown";
            
            case KeyUp:
                return "KeyUp";
                
            case KeyRepeat:
                return "KeyRepeat";
                
            default:
                break;
        }
        return "Unknown";
    }
}