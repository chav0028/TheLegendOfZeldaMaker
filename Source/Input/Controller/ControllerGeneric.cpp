//
//  ControllerGeneric.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-26.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//


#include "ControllerGeneric.h"
#include "../../Events/Input/ControllerEvent.h"



namespace GameDev2D
{
    ControllerGeneric::ControllerGeneric(const string& aControllerType, ControllerData* aControllerData, unsigned int aControllerId) : BaseObject(aControllerType),
        m_ControllerData(aControllerData),
        m_ControllerId(aControllerId)
    {
    
    }
    
    ControllerGeneric::ControllerGeneric(ControllerData* aControllerData, unsigned int aControllerId) : BaseObject("ControllerGeneric"),
        m_ControllerData(aControllerData),
        m_ControllerId(aControllerId)
    {
        Log(VerbosityLevel_Input | VerbosityLevel_Debug, "Generic controller detected - %p", aControllerData->GetDevice());
    }
    
    ControllerGeneric::~ControllerGeneric()
    {
        SafeDelete(m_ControllerData);
    }
    
    const char* ControllerGeneric::StringForInputBinding(unsigned int aBinding, bool aSpecifyAnalogStickAxis)
    {
        std::stringstream ss;
        ss << aBinding;
        return ss.str().c_str();
    }
    
    bool ControllerGeneric::IsBindingValid(unsigned int aBinding)
    {
        return true;
    }
    
    const void* ControllerGeneric::GetDevice()
    {
        return m_ControllerData != nullptr ? m_ControllerData->GetDevice() : nullptr;
    }

    void ControllerGeneric::SetAnalogStickInverted(unsigned int aBinding, bool aIsInverted)
    {
        if(IsAnalogStick(aBinding) == true && m_ControllerData != nullptr)
        {
            m_ControllerData->SetAnalogInputUserInverted(aBinding, aIsInverted);
        }
    }

    bool ControllerGeneric::IsAnalogStickIsInverted(unsigned int aBinding)
    {
        if(IsAnalogStick(aBinding) == true)
        {
            return m_ControllerData != nullptr ? m_ControllerData->IsAnalogInputUserInverted(aBinding) : false;
        }
        return false;
    }

    unsigned int ControllerGeneric::GetId()
    {
        return m_ControllerId;
    }
    
    bool ControllerGeneric::IsButtonDown(unsigned int aBinding)
    {
        return m_ControllerData != nullptr ? m_ControllerData->IsButtonDownForBinding(aBinding) : false;
    }
    
    float ControllerGeneric::GetAnalogValue(unsigned int aBinding)
    {
        if(m_ControllerData != nullptr)
        {
            if(IsAnalogStick(aBinding) == true)
            {
                long longValue = m_ControllerData->GetAnalogValueForBinding(aBinding);
                long longMin = m_ControllerData->GetMinAnalogValue(aBinding);
                long longMax = m_ControllerData->GetMaxAnalogValue(aBinding);
                bool isPlatformInverted = m_ControllerData->IsAnalogInputPlatformInverted(aBinding);
                bool isUserInverted = m_ControllerData->IsAnalogInputUserInverted(aBinding);
                float value = ((float)(longValue - longMin) / (float)(longMax - longMin) * 2.0f - 1.0f);
                return value * (isPlatformInverted == true ? -1.0f : 1.0f) * (isUserInverted == true ? -1.0f : 1.0f);
            }
            else
            {
                long longValue = m_ControllerData->GetAnalogValueForBinding(aBinding);
                long longMin = m_ControllerData->GetMinAnalogValue(aBinding);
                long longMax = m_ControllerData->GetMaxAnalogValue(aBinding);
                return ((float)(longValue - longMin) / (float)(longMax - longMin));
            }
        }

        return 0.0f;
    }
    
    ControllerData* ControllerGeneric::GetControllerData()
    {
        return m_ControllerData;
    }
    
    bool ControllerGeneric::IsAnalogStick(unsigned int aBinding)
    {
        return false;
    }
    
    void ControllerGeneric::GetAnalogStickBindings(unsigned int aInBinding, unsigned int (&aOutBindings)[2])
    {
        aOutBindings[0] = aInBinding;
        aOutBindings[1] = aInBinding;
    }
}