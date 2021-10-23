//
//  MouseData.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-22.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//


#include "MouseData.h"


namespace GameDev2D
{
    MouseData::MouseData() : BaseObject("MouseData"),
        m_Position(0.0f, 0.0f),
        m_PreviousPosition(0.0f, 0.0f)
    {
        for(int i = 0; i < MouseButtonCount; i++)
        {
            m_Duration[i] = 0.0;
            m_IsPressed[i] = false;
        }
    }
    
    MouseData::~MouseData()
    {
    
    }
    
    void MouseData::SetPosition(vec2 aPosition)
    {
        //Set the previous position
        m_PreviousPosition = m_Position;
        
        //Set the new position
        m_Position = aPosition;
    }
    
    vec2 MouseData::GetPosition()
    {
        return m_Position;
    }
    
    vec2 MouseData::GetPreviousPosition()
    {
        return m_PreviousPosition;
    }
    
    void MouseData::SetIsPressed(MouseButton aMouseButton, bool aIsPressed)
    {
        //Safety check
        if(aMouseButton >= MouseButtonLeft && aMouseButton < MouseButtonCount)
        {
            //Set the mouse button has been pressed flag
            m_IsPressed[aMouseButton] = aIsPressed;
            
            //If the mouse button is no longer pressed, reset the duration
            if(m_IsPressed[aMouseButton] == false)
            {
                m_Duration[aMouseButton] = 0.0;
            }
        }
    }
    
    bool MouseData::IsPressed(MouseButton aMouseButton)
    {
        //Safety check
        if(aMouseButton >= MouseButtonLeft && aMouseButton < MouseButtonCount)
        {
            return m_IsPressed[aMouseButton];
        }
        return false;
    }
    
    double MouseData::GetDuration(MouseButton aMouseButton)
    {
        //Safety check
        if(aMouseButton >= MouseButtonLeft && aMouseButton < MouseButtonCount)
        {
            return m_Duration[aMouseButton];
        }
        return false;
    }
        
    void MouseData::Update(double aDelta)
    {
        //Cycle through all the mouse button, and if any of them are pressed, update the duration
        for(int i = 0; i < MouseButtonCount; i++)
        {
            if(m_IsPressed[i] == true)
            {
                m_Duration[i] += aDelta;
            }
        }
    }
    
    const char* MouseButtonName(MouseButton aMouseButton)
    {
        switch (aMouseButton)
        {
            case MouseButtonLeft:
                return "Left";
            
            case MouseButtonCenter:
                return "Center";
                
            case MouseButtonRight:
                return "Right";
                
            default:
                break;
        }
        return "Unknown";
    }
}