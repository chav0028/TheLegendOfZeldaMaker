//
//  ControllerXbox360.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-26.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//


#include "ControllerXbox360.h"


namespace GameDev2D
{
    ControllerXbox360::ControllerXbox360(ControllerData* aControllerData, unsigned int aControllerId) : ControllerGeneric("ControllerXbox360", aControllerData, aControllerId)
    {
        //Log that a XBOX 360 controller has been detected
        Log(VerbosityLevel_Input | VerbosityLevel_Debug, "Microsoft Xbox 360 controller detected - %p", aControllerData->GetDevice());
        
        //Set the 360 analog stick dead zone
        aControllerData->SetAnalogStickDeadZone(XBOX_360_CONTROLLER_DEFAULT_ANALOG_STICK_DEAD_ZONE);
        
        //Set the valid input bindings
        m_ValidBindings[0] = ControllerXbox360::ButtonA();
        m_ValidBindings[1] = ControllerXbox360::ButtonB();
        m_ValidBindings[2] = ControllerXbox360::ButtonX();
        m_ValidBindings[3] = ControllerXbox360::ButtonY();
        m_ValidBindings[4] = ControllerXbox360::DirectionalPadLeft();
        m_ValidBindings[5] = ControllerXbox360::DirectionalPadRight();
        m_ValidBindings[6] = ControllerXbox360::DirectionalPadUp();
        m_ValidBindings[7] = ControllerXbox360::DirectionalPadDown();
        m_ValidBindings[8] = ControllerXbox360::ButtonXbox();
        m_ValidBindings[9] = ControllerXbox360::ButtonStart();
        m_ValidBindings[10] = ControllerXbox360::ButtonBack();
        m_ValidBindings[11] = ControllerXbox360::ButtonLeftShoulder();
        m_ValidBindings[12] = ControllerXbox360::ButtonRightShoulder();
        m_ValidBindings[13] = ControllerXbox360::ButtonLeftStick();
        m_ValidBindings[14] = ControllerXbox360::ButtonRightStick();
        m_ValidBindings[15] = ControllerXbox360::TriggerLeft();
        m_ValidBindings[16] = ControllerXbox360::TriggerRight();
        m_ValidBindings[17] = ControllerXbox360::AnalogStickLeftHorizontal();
        m_ValidBindings[18] = ControllerXbox360::AnalogStickLeftVertical();
        m_ValidBindings[19] = ControllerXbox360::AnalogStickRightHorizontal();
        m_ValidBindings[20] = ControllerXbox360::AnalogStickRightVertical();
    }
    
    ControllerXbox360::~ControllerXbox360()
    {
    
    }
    
    const char* ControllerXbox360::StringForInputBinding(unsigned int aBinding, bool aSpecifyAnalogStickAxis)
    {
        if(aBinding == ControllerXbox360::ButtonA())
        {
            return "A";
        }
        else if(aBinding == ControllerXbox360::ButtonB())
        {
            return "B";
        }
        else if(aBinding == ControllerXbox360::ButtonX())
        {
            return "X";
        }
        else if(aBinding == ControllerXbox360::ButtonY())
        {
            return "Y";
        }
        else if(aBinding == ControllerXbox360::ButtonStart())
        {
            return "Start";
        }
        else if(aBinding == ControllerXbox360::ButtonBack())
        {
            return "Back";
        }
        else if(aBinding == ControllerXbox360::ButtonXbox())
        {
            return "Xbox Button";
        }
        else if(aBinding == ControllerXbox360::ButtonRightShoulder())
        {
            return "Right Shoulder";
        }
        else if(aBinding == ControllerXbox360::ButtonLeftShoulder())
        {
            return "Left Shoulder";
        }
        else if(aBinding == ControllerXbox360::ButtonRightStick())
        {
            return "Right Stick";
        }
        else if(aBinding == ControllerXbox360::ButtonLeftStick())
        {
            return "Left Stick";
        }
        else if(aBinding == ControllerXbox360::DirectionalPadDown())
        {
            return "Dpad Down";
        }
        else if(aBinding == ControllerXbox360::DirectionalPadUp())
        {
            return "Dpad Up";
        }
        else if(aBinding == ControllerXbox360::DirectionalPadLeft())
        {
            return "Dpad Left";
        }
        else if(aBinding == ControllerXbox360::DirectionalPadRight())
        {
            return "Dpad Right";
        }
        else if(aBinding == ControllerXbox360::TriggerRight())
        {
            return "Right Trigger";
        }
        else if(aBinding == ControllerXbox360::TriggerLeft())
        {
            return "Left Trigger";
        }
        else if(aBinding == ControllerXbox360::AnalogStickRightHorizontal())
        {
            return aSpecifyAnalogStickAxis == true ? "Analog Stick (Right) - Horizontal" : "Analog Stick (Right)";
        }
        else if(aBinding == ControllerXbox360::AnalogStickRightVertical())
        {
            return aSpecifyAnalogStickAxis == true ? "Analog Stick (Right) - Vertical" : "Analog Stick (Right)";
        }
        else if(aBinding == ControllerXbox360::AnalogStickLeftHorizontal())
        {
            return aSpecifyAnalogStickAxis == true ? "Analog Stick (Left) - Horizontal" : "Analog Stick (Left)";
        }
        else if(aBinding == ControllerXbox360::AnalogStickLeftVertical())
        {
            return aSpecifyAnalogStickAxis == true ? "Analog Stick (Left) - Vertical" : "Analog Stick (Left)";
        }
        
        //
        return ControllerGeneric::StringForInputBinding(aBinding);
    }
    
    bool ControllerXbox360::IsBindingValid(unsigned int aBinding)
    {
        for(int i = 0; i < XBOX_360_CONTROLLER_INPUT_COUNT; i++)
        {
            if(m_ValidBindings[i] == aBinding)
            {
                return true;
            }
        }
    
        return false;
    }
    
    bool ControllerXbox360::IsAnalogStick(unsigned int aBinding)
    {
        return aBinding == AnalogStickLeftHorizontal() || aBinding == AnalogStickLeftVertical() || aBinding == AnalogStickRightHorizontal() || aBinding == AnalogStickRightVertical();
    }
    
    void ControllerXbox360::GetAnalogStickBindings(unsigned int aInBinding, unsigned int (&aOutBindings)[2])
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
    
    float ControllerXbox360::GetAnalogValue(unsigned int aBinding)
    {
        float value = ControllerGeneric::GetAnalogValue(aBinding);
        if(fabsf(value) > XBOX_360_CONTROLLER_ANALOG_STICK_DEAD_ZONE_EPSILON)
        {
            return value;
        }

        return 0.0f;
    }

    unsigned int ControllerXbox360::AnalogStickLeftHorizontal()
    {
#if __APPLE__
        return 0x1b;
#elif _WIN32
        return VK_PAD_LTHUMB_LEFT;
#endif
    }
    
    unsigned int ControllerXbox360::AnalogStickLeftVertical()
    {
#if __APPLE__
        return 0x1c;
#elif _WIN32
        return VK_PAD_LTHUMB_UP;
#endif
    }
    
    unsigned int ControllerXbox360::AnalogStickRightHorizontal()
    {
#if __APPLE__
        return 0x1d;
#elif _WIN32
        return VK_PAD_RTHUMB_LEFT;
#endif
    }
    
    unsigned int ControllerXbox360::AnalogStickRightVertical()
    {
#if __APPLE__
        return 0x1e;
#elif _WIN32
        return VK_PAD_RTHUMB_UP;
#endif
    }
    
    unsigned int ControllerXbox360::DirectionalPadLeft()
    {
#if __APPLE__
        return 0xc;
#elif _WIN32
        return XINPUT_GAMEPAD_DPAD_LEFT;
#endif
    }
    
    unsigned int ControllerXbox360::DirectionalPadRight()
    {
#if __APPLE__
        return 0xd;
#elif _WIN32
        return XINPUT_GAMEPAD_DPAD_RIGHT;
#endif
    }
    
    unsigned int ControllerXbox360::DirectionalPadUp()
    {
#if __APPLE__
        return 0xa;
#elif _WIN32
        return XINPUT_GAMEPAD_DPAD_UP;
#endif
    }
    
    unsigned int ControllerXbox360::DirectionalPadDown()
    {
#if __APPLE__
        return 0xb;
#elif _WIN32
        return XINPUT_GAMEPAD_DPAD_DOWN;
#endif
    }
    
    unsigned int ControllerXbox360::ButtonA()
    {
#if __APPLE__
        return 0x15;
#elif _WIN32
        return XINPUT_GAMEPAD_A;
#endif
    }
    
    unsigned int ControllerXbox360::ButtonB()
    {
#if __APPLE__
        return 0x16;
#elif _WIN32
        return XINPUT_GAMEPAD_B;
#endif
    }
    
    unsigned int ControllerXbox360::ButtonX()
    {
#if __APPLE__
        return 0x17;
#elif _WIN32
        return XINPUT_GAMEPAD_X;
#endif
    }
    
    unsigned int ControllerXbox360::ButtonY()
    {
#if __APPLE__
        return 0x18;
#elif _WIN32
        return XINPUT_GAMEPAD_Y;
#endif
    }
    
    unsigned int ControllerXbox360::ButtonXbox()
    {
#if __APPLE__
        return 0x14;
#elif _WIN32
        return 0; //Doesn't send a message on windows
#endif
    }
    
    unsigned int ControllerXbox360::ButtonStart()
    {
#if __APPLE__
        return 0xe;
#elif _WIN32
        return XINPUT_GAMEPAD_START;
#endif
    }
    
    unsigned int ControllerXbox360::ButtonBack()
    {
#if __APPLE__
        return 0xf;
#elif _WIN32
        return XINPUT_GAMEPAD_BACK;
#endif
    }
    
    unsigned int ControllerXbox360::ButtonLeftStick()
    {
#if __APPLE__
        return 0x10;
#elif _WIN32
        return XINPUT_GAMEPAD_LEFT_THUMB;
#endif
    }
    
    unsigned int ControllerXbox360::ButtonRightStick()
    {
#if __APPLE__
        return 0x11;
#elif _WIN32
        return XINPUT_GAMEPAD_RIGHT_THUMB;
#endif
    }
    
    unsigned int ControllerXbox360::ButtonLeftShoulder()
    {
#if __APPLE__
        return 0x12;
#elif _WIN32
        return XINPUT_GAMEPAD_LEFT_SHOULDER;
#endif
    }
    
    unsigned int ControllerXbox360::ButtonRightShoulder()
    {
#if __APPLE__
        return 0x13;
#elif _WIN32
        return XINPUT_GAMEPAD_RIGHT_SHOULDER;
#endif
    }
    
    unsigned int ControllerXbox360::TriggerLeft()
    {
#if __APPLE__
        return 0x19;
#elif _WIN32
        return VK_PAD_LTRIGGER;
#endif
    }
    
    unsigned int ControllerXbox360::TriggerRight()
    {
#if __APPLE__
        return 0x1a;
#elif _WIN32
        return VK_PAD_RTRIGGER;
#endif
    }
}