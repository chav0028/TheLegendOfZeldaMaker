//
//  ResizeEvent.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-21.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "ResizeEvent.h"


namespace GameDev2D
{
    ResizeEvent::ResizeEvent(vec2 aSize) : Event("ResizeEvent"),
        m_Size(aSize)
    {
        SetEventCode(RESIZE_EVENT);
    }
    
    ResizeEvent::~ResizeEvent()
    {
    
    }
    
    vec2 ResizeEvent::GetSize()
    {
        return m_Size;
    }
    
    void ResizeEvent::LogEvent()
    {
        Log(VerbosityLevel_Events, "%f, %f", m_Size.x, m_Size.y);
    }
}