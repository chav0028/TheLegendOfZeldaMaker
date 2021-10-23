//
//  ControllerPS3.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-26.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//


#include "ControllerPS3.h"


namespace GameDev2D
{
    ControllerPS3::ControllerPS3(ControllerData* aControllerData, unsigned int aControllerId) : ControllerGeneric("ControllerPS3", aControllerData, aControllerId)
    {
        //Log that a Playstation 3 controller has been detected
        Log(VerbosityLevel_Input | VerbosityLevel_Debug, "Sony Playstation 3 controller detected - %p", aControllerData->GetDevice());
        
        //Set the PS3 analog stick dead zone
        GetControllerData()->SetAnalogStickDeadZone(PS3_CONTROLLER_DEFAULT_ANALOG_STICK_DEAD_ZONE);
        
        //Set the valid input bindings
        m_ValidBindings[0] = ControllerPS3::ButtonX();
        m_ValidBindings[1] = ControllerPS3::ButtonCircle();
        m_ValidBindings[2] = ControllerPS3::ButtonSquare();
        m_ValidBindings[3] = ControllerPS3::ButtonTriangle();
        m_ValidBindings[4] = ControllerPS3::DirectionalPadLeft();
        m_ValidBindings[5] = ControllerPS3::DirectionalPadRight();
        m_ValidBindings[6] = ControllerPS3::DirectionalPadUp();
        m_ValidBindings[7] = ControllerPS3::DirectionalPadDown();
        m_ValidBindings[8] = ControllerPS3::ButtonPlaystation();
        m_ValidBindings[9] = ControllerPS3::ButtonStart();
        m_ValidBindings[10] = ControllerPS3::ButtonSelect();
        m_ValidBindings[11] = ControllerPS3::ButtonLeftShoulder();
        m_ValidBindings[12] = ControllerPS3::ButtonRightShoulder();
        m_ValidBindings[13] = ControllerPS3::ButtonLeftStick();
        m_ValidBindings[14] = ControllerPS3::ButtonRightStick();
        m_ValidBindings[15] = ControllerPS3::TriggerLeft();
        m_ValidBindings[16] = ControllerPS3::TriggerRight();
        m_ValidBindings[17] = ControllerPS3::AnalogStickLeftHorizontal();
        m_ValidBindings[18] = ControllerPS3::AnalogStickLeftVertical();
        m_ValidBindings[19] = ControllerPS3::AnalogStickRightHorizontal();
        m_ValidBindings[20] = ControllerPS3::AnalogStickRightVertical();
    }
    
    ControllerPS3::~ControllerPS3()
    {
    
    }
    
    const char* ControllerPS3::StringForInputBinding(unsigned int aBinding, bool aSpecifyAnalogStickAxis)
    {
        if(aBinding == ControllerPS3::ButtonX())
        {
            return "X";
        }
        else if(aBinding == ControllerPS3::ButtonCircle())
        {
            return "Circle";
        }
        else if(aBinding == ControllerPS3::ButtonSquare())
        {
            return "Square";
        }
        else if(aBinding == ControllerPS3::ButtonTriangle())
        {
            return "Triangle";
        }
        else if(aBinding == ControllerPS3::ButtonStart())
        {
            return "Start";
        }
        else if(aBinding == ControllerPS3::ButtonSelect())
        {
            return "Select";
        }
        else if(aBinding == ControllerPS3::ButtonPlaystation())
        {
            return "Playstation Button";
        }
        else if(aBinding == ControllerPS3::ButtonRightShoulder())
        {
            return "Right Shoulder";
        }
        else if(aBinding == ControllerPS3::ButtonLeftShoulder())
        {
            return "Left Shoulder";
        }
        else if(aBinding == ControllerPS3::ButtonRightStick())
        {
            return "Right Stick";
        }
        else if(aBinding == ControllerPS3::ButtonLeftStick())
        {
            return "Left Stick";
        }
        else if(aBinding == ControllerPS3::DirectionalPadDown())
        {
            return "Dpad Down";
        }
        else if(aBinding == ControllerPS3::DirectionalPadUp())
        {
            return "Dpad Up";
        }
        else if(aBinding == ControllerPS3::DirectionalPadLeft())
        {
            return "Dpad Left";
        }
        else if(aBinding == ControllerPS3::DirectionalPadRight())
        {
            return "Dpad Right";
        }
        else if(aBinding == ControllerPS3::TriggerRight())
        {
            return "Right Trigger";
        }
        else if(aBinding == ControllerPS3::TriggerLeft())
        {
            return "Left Trigger";
        }
        else if(aBinding == ControllerPS3::AnalogStickRightHorizontal())
        {
            return aSpecifyAnalogStickAxis == true ? "Analog Stick (Right) - Horizontal" : "Analog Stick (Right)";
        }
        else if(aBinding == ControllerPS3::AnalogStickRightVertical())
        {
            return aSpecifyAnalogStickAxis == true ? "Analog Stick (Right) - Vertical" : "Analog Stick (Right)";
        }
        else if(aBinding == ControllerPS3::AnalogStickLeftHorizontal())
        {
            return aSpecifyAnalogStickAxis == true ? "Analog Stick (Left) - Horizontal" : "Analog Stick (Left)";
        }
        else if(aBinding == ControllerPS3::AnalogStickLeftVertical())
        {
            return aSpecifyAnalogStickAxis == true ? "Analog Stick (Left) - Vertical" : "Analog Stick (Left)";
        }
        
        return ControllerGeneric::StringForInputBinding(aBinding);
    }
    
    bool ControllerPS3::IsBindingValid(unsigned int aBinding)
    {
        for(int i = 0; i < PS3_CONTROLLER_INPUT_COUNT; i++)
        {
            if(m_ValidBindings[i] == aBinding)
            {
                return true;
            }
        }
    
        return false;
    }
    
    bool ControllerPS3::IsAnalogStick(unsigned int aBinding)
    {
        return aBinding == AnalogStickLeftHorizontal() || aBinding == AnalogStickLeftVertical() || aBinding == AnalogStickRightHorizontal() || aBinding == AnalogStickRightVertical();
    }
    
    void ControllerPS3::GetAnalogStickBindings(unsigned int aInBinding, unsigned int (&aOutBindings)[2])
    {
        if(aInBinding == AnalogStickLeftHorizontal() || aInBinding == AnalogStickLeftVertical())
        {
            aOutBindings[0] = AnalogStickLeftHorizontal();
            aOutBindings[1] = AnalogStickLeftVertical();
        }
        else if(aInBinding == AnalogStickRightHorizontal() || aInBinding == AnalogStickRightVertical())
        {
            aOutBindings[0] = AnalogStickRightHorizontal();
            aOutBindings[1] = AnalogStickRightVertical();
        }
    }
    
    float ControllerPS3::GetAnalogValue(unsigned int aBinding)
    {
        float value = ControllerGeneric::GetAnalogValue(aBinding);
        if(fabsf(value) > PS3_CONTROLLER_ANALOG_STICK_DEAD_ZONE_EPSILON)
        {
            return value;
        }

        return 0.0f;
    }
    
    unsigned int ControllerPS3::AnalogStickLeftHorizontal()
    {
        return 0x1a;
    }
    
    unsigned int ControllerPS3::AnalogStickLeftVertical()
    {
        return 0x1b;
    }
    
    unsigned int ControllerPS3::AnalogStickRightHorizontal()
    {
        return 0x1c;
    }
    
    unsigned int ControllerPS3::AnalogStickRightVertical()
    {
        return 0x1d;
    }
    
    unsigned int ControllerPS3::DirectionalPadLeft()
    {
        return 0xe;
    }
    
    unsigned int ControllerPS3::DirectionalPadRight()
    {
        return 0xc;
    }
    
    unsigned int ControllerPS3::DirectionalPadUp()
    {
        return 0xb;
    }
    
    unsigned int ControllerPS3::DirectionalPadDown()
    {
        return 0xd;
    }
    
    unsigned int ControllerPS3::ButtonX()
    {
        return 0x15;
    }
    
    unsigned int ControllerPS3::ButtonCircle()
    {
        return 0x14;
    }
    
    unsigned int ControllerPS3::ButtonSquare()
    {
        return 0x16;
    }
    
    unsigned int ControllerPS3::ButtonTriangle()
    {
        return 0x13;
    }
    
    unsigned int ControllerPS3::ButtonPlaystation()
    {
        return 0x17;
    }
    
    unsigned int ControllerPS3::ButtonStart()
    {
        return 0xa;
    }
    
    unsigned int ControllerPS3::ButtonSelect()
    {
        return 0x7;
    }
    
    unsigned int ControllerPS3::ButtonLeftStick()
    {
        return 0x8;
    }
    
    unsigned int ControllerPS3::ButtonRightStick()
    {
        return 0x9;
    }
    
    unsigned int ControllerPS3::ButtonLeftShoulder()
    {
        return 0x11;
    }
    
    unsigned int ControllerPS3::ButtonRightShoulder()
    {
        return 0x12;
    }
    
    unsigned int ControllerPS3::TriggerLeft()
    {
        return 0x27;
    }
    
    unsigned int ControllerPS3::TriggerRight()
    {
        return 0x28;
    }
}