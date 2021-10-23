//
//  FullscreenEvent.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-12.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "FullscreenEvent.h"


namespace GameDev2D
{
    FullscreenEvent::FullscreenEvent(bool aIsFullscreen) : Event("FullscreenEvent"),
        m_IsFullscreen(aIsFullscreen)
    {
        SetEventCode(FULLSCREEN_EVENT);
    }
    
    FullscreenEvent::~FullscreenEvent()
    {
    
    }
    
    bool FullscreenEvent::IsFullscreen()
    {
        return m_IsFullscreen;
    }
    
    void FullscreenEvent::LogEvent()
    {
        Log(VerbosityLevel_Events, "Fullscreen: %s", m_IsFullscreen == true ? "true" : "false");
    }
}