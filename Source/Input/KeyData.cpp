//
//  KeyData.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-22.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//


#include "KeyData.h"


namespace GameDev2D
{
    KeyData::KeyData() : BaseObject("KeyData"),
        m_Duration(0.0),
        m_IsPressed(false)
    {
    
    }
    
    KeyData::~KeyData()
    {
    
    }
    
    void KeyData::SetIsPressed(bool aIsPressed)
    {
        //Set that the key isPressed flag
        m_IsPressed = aIsPressed;
        
        //If the key is no longer pressed, reset the duration
        if(m_IsPressed == false)
        {
            m_Duration = 0.0;
        }
    }
    
    bool KeyData::IsPressed()
    {
        return m_IsPressed;
    }
    
    double KeyData::GetDuration()
    {
        return m_Duration;
    }
    
    void KeyData::Update(double aDelta)
    {
        //If the key is pressed, update the duration
        if(m_IsPressed == true)
        {
            m_Duration += aDelta;
        }
    }
}
