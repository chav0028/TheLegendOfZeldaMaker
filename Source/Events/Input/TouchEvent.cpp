//
//  TouchEvent.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "TouchEvent.h"


namespace GameDev2D
{
    TouchEvent::TouchEvent(TouchType aTouchType, unsigned int aIndex, vec2 aTouchLocation, vec2 aPreviousLocation, double aDuration) : Event("TouchEvent"),
        m_TouchType(aTouchType),
        m_Index(aIndex),
        m_TouchLocation(vec2(aTouchLocation.x, aTouchLocation.y)),
        m_PreviousLocation(vec2(aPreviousLocation.x, aPreviousLocation.y)),
        m_Duration(aDuration)
    {
        SetEventCode(TOUCH_EVENT);
    }
    
    TouchEvent::~TouchEvent()
    {
    
    }
    
    TouchType TouchEvent::GetTouchType()
    {
        return m_TouchType;
    }
    
    unsigned int TouchEvent::GetIndex()
    {
        return m_Index;
    }
    
    vec2 TouchEvent::GetTouchLocation()
    {
        return m_TouchLocation;
    }
    
    vec2 TouchEvent::GetPreviousLocation()
    {
        return m_PreviousLocation;
    }
    
    double TouchEvent::GetDuration()
    {
        return m_Duration;
    }
    
    void TouchEvent::LogEvent()
    {
        if(m_TouchType == TouchBegan)
        {
            Log(VerbosityLevel_Input, "%s [%i] - (%f, %f)", TouchTypeName(m_TouchType), m_Index, m_TouchLocation.x, m_TouchLocation.y);
        }
        else
        {
            Log(VerbosityLevel_Input, "%s [%i] - (%f, %f) - (%f, %f) - Duration: %f seconds", TouchTypeName(m_TouchType), m_Index, m_TouchLocation.x, m_TouchLocation.y, m_PreviousLocation.x, m_PreviousLocation.y, m_Duration);
        }
    }
    
    const char* TouchTypeName(TouchType aTouchType)
    {
        switch (aTouchType)
        {
            case TouchBegan:
                return "Touch Began";
            
            case TouchMoved:
                return "Touch Moved";
            
            case TouchEnded:
                return "Touch Ended";
            
            case TouchCancelled:
                return "Touch Cancelled";

            default:
                break;
        }
        return "";
    }
}