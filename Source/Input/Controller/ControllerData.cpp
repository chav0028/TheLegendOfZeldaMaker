//
//  ControllerData.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-28.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//


#include "ControllerData.h"


namespace GameDev2D
{
    ControllerData::ControllerData(void* aDevice, int aVendorId, int aProductId) : BaseObject("ControllerData"),
        m_Device(aDevice),
        m_VendorId(aVendorId),
        m_ProductId(aProductId),
        m_DeadZone(0.0f)
    {
#if _WIN32
        ZeroMemory(&m_StateInfo, sizeof(XINPUT_STATE));
#endif
    }
    
    ControllerData::~ControllerData()
    {
    
    }
    
    void ControllerData::Update(double aDelta)
    {
        for(unsigned int i = 0; i < m_ButtonBindings.size(); i++)
        {
            if(m_ButtonBindings.at(i).isDown == true)
            {
                m_ButtonBindings.at(i).duration += aDelta;
            }
        }
    }
    
    const void* ControllerData::GetDevice()
    {
        return m_Device;
    }
    
    int ControllerData::GetVendorId()
    {
        return m_VendorId;
    }
    
    int ControllerData::GetProductId()
    {
        return m_ProductId;
    }
    
    void ControllerData::AddButton(unsigned int aBinding)
    {
        //Log that a button was added
        Log(VerbosityLevel_Input, "Button(%u) added for device: %p", aBinding, m_Device);
        
        //Add the ButtonData to the button bindings vector
        m_ButtonBindings.push_back(ButtonData(aBinding));
    }
    
    void ControllerData::AddAnalog(unsigned int aBinding, long aMin, long aMax, long aValue, bool aPlatformInverted)
    {
        //Log that an analog input was added
        Log(VerbosityLevel_Input, "Analog(%u) added for device: %p - (%li, %li)", aBinding, m_Device, aMin, aMax);
        
        //Add the AnalogData to the analog bindings vector
        m_AnalogBindings.push_back(AnalogData(aBinding, aMin, aMax, aValue, aPlatformInverted));
    }

    unsigned long ControllerData::GetNumberOfButtonBindings()
    {
        return m_ButtonBindings.size();
    }

    unsigned long ControllerData::GetNumberOfAnalogBindings()
    {
        return m_AnalogBindings.size();
    }

    unsigned int ControllerData::GetButtonBindingForIndex(unsigned int aIndex)
    {
        if(aIndex < m_ButtonBindings.size())
        {
            return m_ButtonBindings.at(aIndex).binding;
        }
        return 0;
    }
    
    int ControllerData::IsButtonBinding(unsigned int aBinding)
    {
        for(unsigned int i = 0; i < m_ButtonBindings.size(); i++)
        {
            //We found the binding, return the index
            if(aBinding == m_ButtonBindings.at(i).binding)
            {
                return i;
            }
        }
        
        //If we got here, it means the binding was NOT a button binding, return -1
        return -1;
    }
    
    int ControllerData::IsAnalogBinding(unsigned int aBinding)
    {
        for(unsigned int i = 0; i < m_AnalogBindings.size(); i++)
        {
            //We found the binding, return the index
            if(aBinding == m_AnalogBindings.at(i).binding)
            {
                return i;
            }
        }
        
        //If we got here, it means the binding was NOT an analog binding, return -1
        return -1;
    }
    
    long ControllerData::GetMinAnalogValue(unsigned int aBinding)
    {
        int index = IsAnalogBinding(aBinding);
        if(index != -1)
        {
            return m_AnalogBindings.at(index).min;
        }
        return 0;
    }
    
    long ControllerData::GetMaxAnalogValue(unsigned int aBinding)
    {
        int index = IsAnalogBinding(aBinding);
        if(index != -1)
        {
            return m_AnalogBindings.at(index).max;
        }
        return 0;
    }
    
    void ControllerData::SetButtonValueAtIndex(unsigned int aIndex, bool aIsDown)
    {
        //Safety check the index
        if(aIndex < m_ButtonBindings.size())
        {
            //Set the button state
            m_ButtonBindings.at(aIndex).isDown = aIsDown;
            
            //If it is down, reset the duration
            if(aIsDown == true)
            {
                m_ButtonBindings.at(aIndex).duration = 0.0;
            }
        }
    }
    
    void ControllerData::SetAnalogValueAtIndex(unsigned int aIndex, long aValue)
    {
        //Safety check the index
        if(aIndex < m_AnalogBindings.size())
        {
            //Bounds check against the min value
            if(aValue < m_AnalogBindings.at(aIndex).min)
            {
                aValue = m_AnalogBindings.at(aIndex).min;
            }
            
            //Bounds check against the ,ax value
            if(aValue > m_AnalogBindings.at(aIndex).max)
            {
                aValue = m_AnalogBindings.at(aIndex).max;
            }

            //Set the anlog value for the binding
            m_AnalogBindings.at(aIndex).value = aValue;
        }
    }
    
    void ControllerData::SetButtonValueForBinding(unsigned int aBinding, bool aIsDown)
    {
        for(unsigned int i = 0; i < m_ButtonBindings.size(); i++)
        {
            if(aBinding == m_ButtonBindings.at(i).binding)
            {
                SetButtonValueAtIndex(i, aIsDown);
                break;
            }
        }
    }
    
    void ControllerData::SetAnalogValueForBinding(unsigned int aBinding, long aValue)
    {
        for(unsigned int i = 0; i < m_AnalogBindings.size(); i++)
        {
            if(aBinding == m_AnalogBindings.at(i).binding)
            {
                SetAnalogValueAtIndex(i, aValue);
                break;
            }
        }
    }
    
    bool ControllerData::IsButtonDownForIndex(unsigned int aIndex)
    {
        if(aIndex < m_ButtonBindings.size())
        {
            return m_ButtonBindings.at(aIndex).isDown;
        }
        return false;
    }
    
    long ControllerData::GetAnalogValueForIndex(unsigned int aIndex)
    {
        if(aIndex < m_AnalogBindings.size())
        {
            return m_AnalogBindings.at(aIndex).value;
        }
        return 0;
    }
    
    bool ControllerData::IsButtonDownForBinding(unsigned int aBinding)
    {
        int index = IsButtonBinding(aBinding);
        if(index != -1)
        {
            return IsButtonDownForIndex(index);
        }
        return false;
    }
    
    long ControllerData::GetAnalogValueForBinding(unsigned int aBinding)
    {
        int index = IsAnalogBinding(aBinding);
        if(index != -1)
        {
            return GetAnalogValueForIndex(index);
        }
        
        return 0;
    }
    
    void ControllerData::SetAnalogStickDeadZone(float aDeadZone)
    {
        m_DeadZone = aDeadZone;
    }
    
    float ControllerData::GetAnalogStickDeadZone()
    {
        return m_DeadZone;
    }

    void ControllerData::SetAnalogInputUserInverted(unsigned int aBinding, bool aUserInverted)
    {
        int index = IsAnalogBinding(aBinding);
        if(index != -1)
        {
            m_AnalogBindings.at(index).isUserInverted = aUserInverted;
        }
    }

    bool ControllerData::IsAnalogInputUserInverted(unsigned int aBinding)
    {
        int index = IsAnalogBinding(aBinding);
        if(index != -1)
        {
            return m_AnalogBindings.at(index).isUserInverted;
        }

        return false;
    }

    bool ControllerData::IsAnalogInputPlatformInverted(unsigned int aBinding)
    {
        int index = IsAnalogBinding(aBinding);
        if(index != -1)
        {
            return m_AnalogBindings.at(index).isPlatformInverted;
        }

        return false;
    }
    
    double ControllerData::GetButtonDuration(unsigned int aBinding)
    {
        for(unsigned int i = 0; i < m_ButtonBindings.size(); i++)
        {
            if(aBinding == m_ButtonBindings.at(i).binding)
            {
                return m_ButtonBindings.at(i).duration;
            }
        }
        return 0.0;
    }

    #if _WIN32
    void ControllerData::SetStateInfo(XINPUT_STATE aStateInfo)
    {
        m_StateInfo = aStateInfo;
    }

    XINPUT_STATE ControllerData::GetStateInfo()
    {
        return m_StateInfo;
    }
    #endif
}