//
//  OrientationChangedEvent.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-04-19.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "OrientationChangedEvent.h"

namespace GameDev2D
{
    OrientationChangedEvent::OrientationChangedEvent(Orientation aOrientation) : Event("OrientationChangedEvent"),
        m_Orientation(aOrientation)
    {
        SetEventCode(ORIENTATION_CHANGED_EVENT);
    }
    
    OrientationChangedEvent::~OrientationChangedEvent()
    {
    
    }
    
    Orientation OrientationChangedEvent::GetOrientation()
    {
        return m_Orientation;
    }
    
    void OrientationChangedEvent::LogEvent()
    {
        Log(VerbosityLevel_Events, "%s", m_Orientation == OrientationPortrait ? "Portrait" : (m_Orientation == OrientationLandscape ? "Landscape" : "Unknown"));
    }
}