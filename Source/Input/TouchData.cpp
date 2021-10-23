//
//  TouchData.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-04.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//


#include "TouchData.h"
#include "../Services/ServiceLocator.h"
#include "../Platforms/PlatformLayer.h"
#include "../Graphics/Primitives/Circle.h"
#include "../Graphics/Primitives/Line.h"


namespace GameDev2D
{
    TouchData::TouchData(vec2 aLocation, vec2 aPrevious) : BaseObject("TouchData"),
        m_Duration(0.0),
        m_FadeInTimer(FADE_IN_DURATION),
        m_FadeOutTimer(),
        m_TouchRadius(0.0f),
        m_TouchAlpha(0.0f),
        m_HasEnded(false)
    {
        m_TouchLocations.push_back(make_pair(aLocation, aPrevious));
        m_FadeInTimer.Start();
    }
    
    TouchData::~TouchData()
    {
        m_TouchLocations.clear();
    }
        
    void TouchData::AddTouchLocation(vec2 aLocation, vec2 aPrevious, bool aEnded)
    {
        //Add the touch locations as a pair to the locations vector
        m_TouchLocations.push_back(make_pair(aLocation, aPrevious));
        
        //Has the touch ended
        if(aEnded == true)
        {
            m_HasEnded = true;
            m_FadeOutTimer.SetDuration(FADE_OUT_DURATION);
            m_FadeOutTimer.Start();
        }
    }
    
    bool TouchData::ContainsPrevious(vec2 aPrevious)
    {
        return m_TouchLocations.back().first == aPrevious || m_TouchLocations.back().second == aPrevious;
    }
    
    double TouchData::GetDuration()
    {
        return m_Duration;
    }
    
    bool TouchData::HasEnded()
    {
        return m_HasEnded;
    }
    
    bool TouchData::CanErase()
    {
        if(HasEnded() == true)
        {
            return m_FadeOutTimer.GetPercentageComplete() == 1.0f;
        }
        return false;
    }
        
    void TouchData::Update(double aDelta)
    {
        if(m_HasEnded == false)
        {
            //Increment the duration
            m_Duration += aDelta;
            
            //
            #if DEBUG && DRAW_TOUCH_DEBUG_DATA
            m_FadeInTimer.Update(aDelta);
            m_TouchRadius = DEFAULT_TOUCH_RADIUS + FADE_IN_TOUCH_RADIUS - (FADE_IN_TOUCH_RADIUS * m_FadeInTimer.GetPercentageComplete());
            m_TouchAlpha = MAX_TOUCH_ALPHA * m_FadeInTimer.GetPercentageComplete();
            #endif
        }
        else
        {
            #if DEBUG && DRAW_TOUCH_DEBUG_DATA
            m_FadeOutTimer.Update(aDelta);
            m_TouchRadius *= 1.0f - m_FadeOutTimer.GetPercentageComplete();
            m_TouchAlpha *= 1.0f - m_FadeOutTimer.GetPercentageComplete();
            #endif
        }
    }
    
    void TouchData::Draw()
    {
        //If the touch radius is greater than zero, draw the circle
        #ifdef DRAW_TOUCH_DEBUG_COLOR
        if(m_TouchRadius > 0.0f)
        {
            Circle circle(m_TouchLocations.back().first, m_TouchRadius);
            circle.SetColor(DRAW_TOUCH_DEBUG_COLOR(m_TouchAlpha));
            circle.SetIsFilled(false);
            circle.Draw();
        }
        #endif
        
        #if DRAW_TOUCH_DEBUG_MOVED_PATH
        //If there is a touch event, draw the debug touch data
        if(HasEnded() == false)
        {
            //Set the width and color of the line
            Line line;
            line.SetColor(DRAW_TOUCH_DEBUG_COLOR(m_TouchAlpha));
            
            //Set the line's vertices and draw
            for(unsigned int i = 0; i < m_TouchLocations.size(); i++)
            {
                line.SetLocalPosition(m_TouchLocations.at(i).first);
                line.SetPositionB(m_TouchLocations.at(i).second);
                line.Draw();
            }
        }
        #endif
    }
}