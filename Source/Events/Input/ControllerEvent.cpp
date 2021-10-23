//
//  ControllerEvent.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "ControllerEvent.h"
#include "../../Input/Controller/ControllerGeneric.h"


namespace GameDev2D
{
    ControllerEvent::ControllerEvent(const string& aControllerEvent, ControllerGeneric* aController, unsigned int aInputBinding) : Event(aControllerEvent),
        m_Controller(aController),
        m_InputBinding(aInputBinding)
    {
    
    }
    
    ControllerEvent::~ControllerEvent()
    {
    
    }
    
    ControllerGeneric* ControllerEvent::GetController()
    {
        return m_Controller;
    }
    
    unsigned int ControllerEvent::GetInputBinding()
    {
        return m_InputBinding;
    }
    
    
    ControllerButtonEvent::ControllerButtonEvent(ControllerGeneric* aController, unsigned int aInputBinding) :  ControllerEvent("ControllerButtonEvent", aController, aInputBinding)
    {
        SetEventCode(CONTROLLER_BUTTON_EVENT);
    }
    
    ControllerButtonEvent::~ControllerButtonEvent()
    {
    
    }
        
    bool ControllerButtonEvent::IsButtonDown()
    {
        if(GetController() != nullptr)
        {
            return GetController()->IsButtonDown(GetInputBinding());
        }
        return false;
    }
    
    double ControllerButtonEvent::GetDuration()
    {
        if(GetController() != nullptr)
        {
            return GetController()->GetControllerData()->GetButtonDuration(GetInputBinding());
        }
        return 0.0;
    }
    
    void ControllerButtonEvent::LogEvent()
    {
        if(IsButtonDown() == false)
        {
            Log(VerbosityLevel_Input, "(%s) Up - Duration: %f", GetController()->StringForInputBinding(GetInputBinding()), GetDuration());
        }
        else
        {
            Log(VerbosityLevel_Input, "(%s) Down", GetController()->StringForInputBinding(GetInputBinding()));
        }
    }
    
    ControllerAnalogEvent::ControllerAnalogEvent(ControllerGeneric* aController, unsigned int aInputBinding) :  ControllerEvent("ControllerAnalogEvent", aController, aInputBinding)
    {
        SetEventCode(CONTROLLER_ANALOG_EVENT);
    }
    
    ControllerAnalogEvent::~ControllerAnalogEvent()
    {
    
    }
        
    float ControllerAnalogEvent::GetAnalogValue()
    {
        if(GetController() != nullptr)
        {
            return GetController()->GetAnalogValue(GetInputBinding());
        }
        return 0.0f;
    }
    
    void ControllerAnalogEvent::LogEvent()
    {
        Log(VerbosityLevel_Input, "%s - %f", GetController()->StringForInputBinding(GetInputBinding()), GetAnalogValue());
    }
    
    ControllerAnalogStickEvent::ControllerAnalogStickEvent(ControllerGeneric* aController, unsigned int aInputBinding[2]) :  ControllerEvent("ControllerAnalogStickEvent", aController, aInputBinding[0]),
        m_InputBindingY(aInputBinding[1])
    {
        SetEventCode(CONTROLLER_ANALOG_STICK_EVENT);
    }
    
    ControllerAnalogStickEvent::~ControllerAnalogStickEvent()
    {
    
    }
        
    unsigned int ControllerAnalogStickEvent::GetInputBindingX()
    {
        return GetInputBinding();
    }
    
    unsigned int ControllerAnalogStickEvent::GetInputBindingY()
    {
        return m_InputBindingY;
    }
        
    float ControllerAnalogStickEvent::GetAnalogValueX()
    {
        if(GetController() != nullptr)
        {
            return GetController()->GetAnalogValue(GetInputBindingX());
        }
        return 0.0f;
    }
    
    float ControllerAnalogStickEvent::GetAnalogValueY()
    {
        if(GetController() != nullptr)
        {
            return GetController()->GetAnalogValue(GetInputBindingY());
        }
        return 0.0f;
    }
    
    vec2 ControllerAnalogStickEvent::GetAnalogValue()
    {
        return vec2(GetAnalogValueX(), GetAnalogValueY());
    }
    
    void ControllerAnalogStickEvent::LogEvent()
    {
        Log(VerbosityLevel_Input, "%s - (%f, %f)", GetController()->StringForInputBinding(GetInputBinding(), false), GetAnalogValueX(), GetAnalogValueY());
    }
}