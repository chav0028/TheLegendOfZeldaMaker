//
//  DebugUI.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-26.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "DebugUI.h"
#include "../../FrameworkConfig.h"
#include "../ServiceLocator.h"
#include "../InputManager/InputManager.h"
#include "../../Platforms/PlatformLayer.h"
#include "../../UI/Label/Label.h"
#include "../../Events/Platform/ResizeEvent.h"
#include "../../Events/Input/AccelerometerEvent.h"
#include "../../Events/Input/GyroscopeEvent.h"


#ifndef DEBUG_UI_FONT_COLOR
#define DEBUG_UI_FONT_COLOR Color::BlackColor()
#endif


namespace GameDev2D
{
    DebugUI::DebugUI() : GameService("DebugUI"),
        m_FontColor(DEBUG_UI_FONT_COLOR)
    {
        //Add a watch value for the frames per second
        WatchValueUnsignedInt(ServiceLocator::GetPlatformLayer(), CastDebugCallbackUnsignedInt(PlatformLayer::GetFramesPerSecond));
        
        //If the platform has memory tracking enabled, add a watch value
        if(ServiceLocator::GetPlatformLayer()->IsMemoryTrackingEnabled() == true)
        {
            WatchValueString(ServiceLocator::GetPlatformLayer(), CastDebugCallbackString(PlatformLayer::MemoryAllocatedFormatted));
        }
        
        //If the platform has touch input, add a watch value
        if(ServiceLocator::GetPlatformLayer()->HasTouchInput() == true)
        {
            WatchValueUnsignedInt(ServiceLocator::GetInputManager(), CastDebugCallbackUnsignedInt(InputManager::GetNumberOfFingersTouching));
        }
        
        //We need to listen for resize events to ensure the labels are position in the proper place
        ServiceLocator::GetPlatformLayer()->AddEventListener(this, RESIZE_EVENT);
        
        //Add an event listener for enabling/disabling the accelerometer (iOS)
        ServiceLocator::GetPlatformLayer()->AddEventListener(this, ACCELEROMETER_ENABLED_EVENT);
        ServiceLocator::GetPlatformLayer()->AddEventListener(this, ACCELEROMETER_DISABLED_EVENT);
        
        //Add an event listener for enabling/disabling the gyroscope (iOS)
        ServiceLocator::GetPlatformLayer()->AddEventListener(this, GYROSCOPE_ENABLED_EVENT);
        ServiceLocator::GetPlatformLayer()->AddEventListener(this, GYROSCOPE_DISABLED_EVENT);
    }

    DebugUI::~DebugUI()
    {
        //Stop listening for events
        ServiceLocator::GetPlatformLayer()->RemoveEventListener(this, RESIZE_EVENT);
        ServiceLocator::GetPlatformLayer()->RemoveEventListener(this, ACCELEROMETER_ENABLED_EVENT);
        ServiceLocator::GetPlatformLayer()->RemoveEventListener(this, ACCELEROMETER_DISABLED_EVENT);
        ServiceLocator::GetPlatformLayer()->RemoveEventListener(this, GYROSCOPE_ENABLED_EVENT);
        ServiceLocator::GetPlatformLayer()->RemoveEventListener(this, GYROSCOPE_DISABLED_EVENT);
    
        //Cycle through and delete all the label objects
        for(unsigned int i = 0; i < m_Labels.size(); i++)
        {
            SafeDelete(m_Labels.at(i).label);
        }
        m_Labels.clear();
    }

    void DebugUI::Update(double aDelta)
    {
        for(unsigned int i = 0; i < m_Labels.size(); i++)
        {
            if(m_Labels.at(i).callbackType == DebugCallbackTypeUnsignedInt)
            {
                UpdateLabel(aDelta, m_Labels.at(i).label, m_Labels.at(i).callbackObject, m_Labels.at(i).callbackMethodUnsignedInt);
            }
            else if(m_Labels.at(i).callbackType == DebugCallbackTypeInt)
            {
                UpdateLabel(aDelta, m_Labels.at(i).label, m_Labels.at(i).callbackObject, m_Labels.at(i).callbackMethodInt);
            }
            else if(m_Labels.at(i).callbackType == DebugCallbackTypeFloat)
            {
                UpdateLabel(aDelta, m_Labels.at(i).label, m_Labels.at(i).callbackObject, m_Labels.at(i).callbackMethodFloat);
            }
            else if(m_Labels.at(i).callbackType == DebugCallbackTypeDouble)
            {
                UpdateLabel(aDelta, m_Labels.at(i).label, m_Labels.at(i).callbackObject, m_Labels.at(i).callbackMethodDouble);
            }
            else if(m_Labels.at(i).callbackType == DebugCallbackTypeBool)
            {
                UpdateLabel(aDelta, m_Labels.at(i).label, m_Labels.at(i).callbackObject, m_Labels.at(i).callbackMethodBool);
            }
            else if(m_Labels.at(i).callbackType == DebugCallbackTypeString)
            {
                UpdateLabel(aDelta, m_Labels.at(i).label, m_Labels.at(i).callbackObject, m_Labels.at(i).callbackMethodString);
            }
            else if(m_Labels.at(i).callbackType == DebugCallbackTypeVec2)
            {
                UpdateLabel(aDelta, m_Labels.at(i).label, m_Labels.at(i).callbackObject, m_Labels.at(i).callbackMethodVec2);
            }
            else if(m_Labels.at(i).callbackType == DebugCallbackTypeDVec2)
            {
                UpdateLabel(aDelta, m_Labels.at(i).label, m_Labels.at(i).callbackObject, m_Labels.at(i).callbackMethodDVec2);
            }
            else if(m_Labels.at(i).callbackType == DebugCallbackTypeVec3)
            {
                UpdateLabel(aDelta, m_Labels.at(i).label, m_Labels.at(i).callbackObject, m_Labels.at(i).callbackMethodVec3);
            }
            else if(m_Labels.at(i).callbackType == DebugCallbackTypeDVec3)
            {
                UpdateLabel(aDelta, m_Labels.at(i).label, m_Labels.at(i).callbackObject, m_Labels.at(i).callbackMethodDVec3);
            }
        }
    }
    
    void DebugUI::Draw()
    {
        for(unsigned int i = 0; i < m_Labels.size(); i++)
        {
            m_Labels.at(i).label->Draw();
        }
    }

    bool DebugUI::CanUpdate()
    {
    #if DRAW_DEBUG_UI
        return true;
    #else
        return false;
    #endif
    }
    
    bool DebugUI::CanDraw()
    {
    #if DRAW_DEBUG_UI
        return true;
    #else
        return false;
    #endif
    }
    
    void DebugUI::HandleEvent(Event* aEvent)
    {
        switch (aEvent->GetEventCode())
        {
            case RESIZE_EVENT:
            {
                UpdateLayout();
            }
            break;
            
            case ACCELEROMETER_ENABLED_EVENT:
            {
                WatchValueDVec3(ServiceLocator::GetInputManager(), CastDebugCallbackDVec3(InputManager::GetAccelerometerData));
            }
            break;
            
            case ACCELEROMETER_DISABLED_EVENT:
            {
                StopWatchingValueDVec3(ServiceLocator::GetInputManager(), CastDebugCallbackDVec3(InputManager::GetAccelerometerData));
            }
            break;
            
            case GYROSCOPE_ENABLED_EVENT:
            {
                WatchValueDVec3(ServiceLocator::GetInputManager(), CastDebugCallbackDVec3(InputManager::GetGyroscopeData));
            }
            break;
            
            case GYROSCOPE_DISABLED_EVENT:
            {
                StopWatchingValueDVec3(ServiceLocator::GetInputManager(), CastDebugCallbackDVec3(InputManager::GetGyroscopeData));
            }
            break;
                
            default:
                break;
        }
    }
    
    void DebugUI::WatchValueUnsignedInt(BaseObject* aCallbackObject, DebugCallbackUnsignedInt aCallbackMethodUnsignedInt)
    {
        //Add the label and the callback object and method to the vector
        m_Labels.push_back(DebugData(CreateDebugLabel(), aCallbackObject, aCallbackMethodUnsignedInt));
        
        //Update the layout
        UpdateLayout();
    }
    
    void DebugUI::WatchValueInt(BaseObject* aCallbackObject, DebugCallbackInt aCallbackMethodInt)
    {
        //Add the label and the callback object and method to the vector
        m_Labels.push_back(DebugData(CreateDebugLabel(), aCallbackObject, aCallbackMethodInt));
        
        //Update the layout
        UpdateLayout();
    }
    
    void DebugUI::WatchValueFloat(BaseObject* aCallbackObject, DebugCallbackFloat aCallbackMethodFloat)
    {
        //Add the label and the callback object and method to the vector
        m_Labels.push_back(DebugData(CreateDebugLabel(), aCallbackObject, aCallbackMethodFloat));
        
        //Update the layout
        UpdateLayout();
    }
    
    void DebugUI::WatchValueDouble(BaseObject* aCallbackObject, DebugCallbackDouble aCallbackMethodDouble)
    {
        //Add the label and the callback object and method to the vector
        m_Labels.push_back(DebugData(CreateDebugLabel(), aCallbackObject, aCallbackMethodDouble));
        
        //Update the layout
        UpdateLayout();
    }
    
    void DebugUI::WatchValueBool(BaseObject* aCallbackObject, DebugCallbackBool aCallbackMethodBool)
    {
        //Add the label and the callback object and method to the vector
        m_Labels.push_back(DebugData(CreateDebugLabel(), aCallbackObject, aCallbackMethodBool));
        
        //Update the layout
        UpdateLayout();
    }
    
    void DebugUI::WatchValueString(BaseObject* aCallbackObject, DebugCallbackString aCallbackMethodString)
    {
        //Add the label and the callback object and method to the vector
        m_Labels.push_back(DebugData(CreateDebugLabel(), aCallbackObject, aCallbackMethodString));
        
        //Update the layout
        UpdateLayout();
    }
    
    void DebugUI::WatchValueVec2(BaseObject* aCallbackObject, DebugCallbackVec2 aCallbackMethodVec2)
    {
        //Add the label and the callback object and method to the vector
        m_Labels.push_back(DebugData(CreateDebugLabel(), aCallbackObject, aCallbackMethodVec2));
        
        //Update the layout
        UpdateLayout();
    }
    
    void DebugUI::WatchValueDVec2(BaseObject* aCallbackObject, DebugCallbackDVec2 aCallbackMethodDVec2)
    {
        //Add the label and the callback object and method to the vector
        m_Labels.push_back(DebugData(CreateDebugLabel(), aCallbackObject, aCallbackMethodDVec2));
        
        //Update the layout
        UpdateLayout();
    }
    
    void DebugUI::WatchValueVec3(BaseObject* aCallbackObject, DebugCallbackVec3 aCallbackMethodVec3)
    {
        //Add the label and the callback object and method to the vector
        m_Labels.push_back(DebugData(CreateDebugLabel(), aCallbackObject, aCallbackMethodVec3));
        
        //Update the layout
        UpdateLayout();
    }
    
    void DebugUI::WatchValueDVec3(BaseObject* aCallbackObject, DebugCallbackDVec3 aCallbackMethodDVec3)
    {
        //Add the label and the callback object and method to the vector
        m_Labels.push_back(DebugData(CreateDebugLabel(), aCallbackObject, aCallbackMethodDVec3));
        
        //Update the layout
        UpdateLayout();
    }
    
    void DebugUI::StopWatchingValueUnsignedInt(BaseObject* aCallbackObject, DebugCallbackUnsignedInt aCallbackMethodUnsignedInt)
    {
        //Cycle through and erase the entry
        for(unsigned int i = 0; i < m_Labels.size(); i++)
        {
            if(m_Labels.at(i).callbackObject == aCallbackObject && m_Labels.at(i).callbackMethodUnsignedInt == aCallbackMethodUnsignedInt)
            {
                //Erase the label
                EraseLabelAtIndex(i);
                break;
            }
        }
    
        //Update the layout
        UpdateLayout();
    }
    
    void DebugUI::StopWatchingValueInt(BaseObject* aCallbackObject, DebugCallbackInt aCallbackMethodInt)
    {
        //Cycle through and erase the entry
        for(unsigned int i = 0; i < m_Labels.size(); i++)
        {
            if(m_Labels.at(i).callbackObject == aCallbackObject && m_Labels.at(i).callbackMethodInt == aCallbackMethodInt)
            {
                //Erase the label
                EraseLabelAtIndex(i);
                break;
            }
        }
    
        //Update the layout
        UpdateLayout();
    }
    
    void DebugUI::StopWatchingValueFloat(BaseObject* aCallbackObject, DebugCallbackFloat aCallbackMethodFloat)
    {
        //Cycle through and erase the entry
        for(unsigned int i = 0; i < m_Labels.size(); i++)
        {
            if(m_Labels.at(i).callbackObject == aCallbackObject && m_Labels.at(i).callbackMethodFloat == aCallbackMethodFloat)
            {
                //Erase the label
                EraseLabelAtIndex(i);
                break;
            }
        }
    
        //Update the layout
        UpdateLayout();
    }
    
    void DebugUI::StopWatchingValueDouble(BaseObject* aCallbackObject, DebugCallbackDouble aCallbackMethodDouble)
    {
        //Cycle through and erase the entry
        for(unsigned int i = 0; i < m_Labels.size(); i++)
        {
            if(m_Labels.at(i).callbackObject == aCallbackObject && m_Labels.at(i).callbackMethodDouble == aCallbackMethodDouble)
            {
                //Erase the label
                EraseLabelAtIndex(i);
                break;
            }
        }
    
        //Update the layout
        UpdateLayout();
    }
    
    void DebugUI::StopWatchingValueBool(BaseObject* aCallbackObject, DebugCallbackBool aCallbackMethodBool)
    {
        //Cycle through and erase the entry
        for(unsigned int i = 0; i < m_Labels.size(); i++)
        {
            if(m_Labels.at(i).callbackObject == aCallbackObject && m_Labels.at(i).callbackMethodBool == aCallbackMethodBool)
            {
                //Erase the label
                EraseLabelAtIndex(i);
                break;
            }
        }
    
        //Update the layout
        UpdateLayout();
    }
    
    void DebugUI::StopWatchingValueString(BaseObject* aCallbackObject, DebugCallbackString aCallbackMethodString)
    {
        //Cycle through and erase the entry
        for(unsigned int i = 0; i < m_Labels.size(); i++)
        {
            if(m_Labels.at(i).callbackObject == aCallbackObject && m_Labels.at(i).callbackMethodString == aCallbackMethodString)
            {
                //Erase the label
                EraseLabelAtIndex(i);
                break;
            }
        }
    
        //Update the layout
        UpdateLayout();
    }
    
    void DebugUI::StopWatchingValueVec2(BaseObject* aCallbackObject, DebugCallbackVec2 aCallbackMethodVec2)
    {
        //Cycle through and erase the entry
        for(unsigned int i = 0; i < m_Labels.size(); i++)
        {
            if(m_Labels.at(i).callbackObject == aCallbackObject && m_Labels.at(i).callbackMethodVec2 == aCallbackMethodVec2)
            {
                //Erase the label
                EraseLabelAtIndex(i);
                break;
            }
        }
    
        //Update the layout
        UpdateLayout();
    }
    
    void DebugUI::StopWatchingValueDVec2(BaseObject* aCallbackObject, DebugCallbackDVec2 aCallbackMethodDVec2)
    {
        //Cycle through and erase the entry
        for(unsigned int i = 0; i < m_Labels.size(); i++)
        {
            if(m_Labels.at(i).callbackObject == aCallbackObject && m_Labels.at(i).callbackMethodDVec2 == aCallbackMethodDVec2)
            {
                //Erase the label
                EraseLabelAtIndex(i);
                break;
            }
        }
    
        //Update the layout
        UpdateLayout();
    }
    
    void DebugUI::StopWatchingValueVec3(BaseObject* aCallbackObject, DebugCallbackVec3 aCallbackMethodVec3)
    {
        //Cycle through and erase the entry
        for(unsigned int i = 0; i < m_Labels.size(); i++)
        {
            if(m_Labels.at(i).callbackObject == aCallbackObject && m_Labels.at(i).callbackMethodVec3 == aCallbackMethodVec3)
            {
                //Erase the label
                EraseLabelAtIndex(i);
                break;
            }
        }
    
        //Update the layout
        UpdateLayout();
    }
    
    void DebugUI::StopWatchingValueDVec3(BaseObject* aCallbackObject, DebugCallbackDVec3 aCallbackMethodDVec3)
    {
        //Cycle through and erase the entry
        for(unsigned int i = 0; i < m_Labels.size(); i++)
        {
            if(m_Labels.at(i).callbackObject == aCallbackObject && m_Labels.at(i).callbackMethodDVec3 == aCallbackMethodDVec3)
            {
                //Erase the label
                EraseLabelAtIndex(i);
                break;
            }
        }
    
        //Update the layout
        UpdateLayout();
    }
    
    void DebugUI::SetLabelColor(Color aColor)
    {
        //Set the font color
        m_FontColor = aColor;
        
        //Update all the label's to the new font color
        for(unsigned int i = 0; i < m_Labels.size(); i++)
        {
            m_Labels.at(i).label->SetColor(m_FontColor);
        }
    }
    
    void DebugUI::UpdateLabel(double aDelta, Label* aLabel, BaseObject* aCallbackObject, DebugCallbackUnsignedInt aCallbackMethodUnsignedInt)
    {
        if(aLabel != nullptr)
        {
            stringstream labelStream;
            labelStream << (((BaseObject*)aCallbackObject)->*aCallbackMethodUnsignedInt)();
            aLabel->SetText(labelStream.str());
            aLabel->Update(aDelta);
        }
    }
    
    void DebugUI::UpdateLabel(double aDelta, Label* aLabel, BaseObject* aCallbackObject, DebugCallbackInt aCallbackMethodInt)
    {
        if(aLabel != nullptr)
        {
            stringstream labelStream;
            labelStream << (((BaseObject*)aCallbackObject)->*aCallbackMethodInt)();
            aLabel->SetText(labelStream.str());
            aLabel->Update(aDelta);
        }
    }
    
    void DebugUI::UpdateLabel(double aDelta, Label* aLabel, BaseObject* aCallbackObject, DebugCallbackFloat aCallbackMethodFloat)
    {
        if(aLabel != nullptr)
        {
            stringstream labelStream;
            labelStream << setprecision(DEBUG_FLOAT_DOUBLE_PRECISION) << (((BaseObject*)aCallbackObject)->*aCallbackMethodFloat)();
            aLabel->SetText(labelStream.str());
            aLabel->Update(aDelta);
        }
    }
    
    void DebugUI::UpdateLabel(double aDelta, Label* aLabel, BaseObject* aCallbackObject, DebugCallbackDouble aCallbackMethodDouble)
    {
        if(aLabel != nullptr)
        {
            stringstream labelStream;
            labelStream << setprecision(DEBUG_FLOAT_DOUBLE_PRECISION) << (((BaseObject*)aCallbackObject)->*aCallbackMethodDouble)();
            aLabel->SetText(labelStream.str());
            aLabel->Update(aDelta);
        }
    }
    
    void DebugUI::UpdateLabel(double aDelta, Label* aLabel, BaseObject* aCallbackObject, DebugCallbackBool aCallbackMethodBool)
    {
        if(aLabel != nullptr)
        {
            stringstream labelStream;
            labelStream << ((((BaseObject*)aCallbackObject)->*aCallbackMethodBool)() == true ? "true" : "false");
            aLabel->SetText(labelStream.str());
            aLabel->Update(aDelta);
        }
    }
    
    void DebugUI::UpdateLabel(double aDelta, Label* aLabel, BaseObject* aCallbackObject, DebugCallbackString aCallbackMethodString)
    {
        if(aLabel != nullptr)
        {
            //
            string labelText;
            (((BaseObject*)aCallbackObject)->*aCallbackMethodString)(labelText);

            //
            stringstream labelStream;
            labelStream << labelText;

            //
            aLabel->SetText(labelStream.str());
            aLabel->Update(aDelta);
        }
    }
    
    void DebugUI::UpdateLabel(double aDelta, Label* aLabel, BaseObject* aCallbackObject, DebugCallbackVec2 aCallbackMethodVec2)
    {
        if(aLabel != nullptr)
        {
            vec2 vector2 = (((BaseObject*)aCallbackObject)->*aCallbackMethodVec2)();
            stringstream labelStream;
            labelStream << setprecision(DEBUG_FLOAT_DOUBLE_PRECISION) << vector2.x << ", " << vector2.y;
            aLabel->SetText(labelStream.str());
            aLabel->Update(aDelta);
        }
    }
    
    void DebugUI::UpdateLabel(double aDelta, Label* aLabel, BaseObject* aCallbackObject, DebugCallbackDVec2 aCallbackMethodDVec2)
    {
        if(aLabel != nullptr)
        {
            dvec2 dVector2 = (((BaseObject*)aCallbackObject)->*aCallbackMethodDVec2)();
            stringstream labelStream;
            labelStream << setprecision(DEBUG_FLOAT_DOUBLE_PRECISION) << dVector2.x << ", " << dVector2.y;
            aLabel->SetText(labelStream.str());
            aLabel->Update(aDelta);
        }
    }
    
    void DebugUI::UpdateLabel(double aDelta, Label* aLabel, BaseObject* aCallbackObject, DebugCallbackVec3 aCallbackMethodVec3)
    {
        if(aLabel != nullptr)
        {
            vec3 vector3 = (((BaseObject*)aCallbackObject)->*aCallbackMethodVec3)();
            stringstream labelStream;
            labelStream << setprecision(DEBUG_FLOAT_DOUBLE_PRECISION) << vector3.x << ", " << vector3.y << ", " << vector3.z;
            aLabel->SetText(labelStream.str());
            aLabel->Update(aDelta);
        }
    }
    
    void DebugUI::UpdateLabel(double aDelta, Label* aLabel, BaseObject* aCallbackObject, DebugCallbackDVec3 aCallbackMethodDVec3)
    {
        if(aLabel != nullptr)
        {
            dvec3 dVector3 = (((BaseObject*)aCallbackObject)->*aCallbackMethodDVec3)();
            stringstream labelStream;
            labelStream << setprecision(DEBUG_FLOAT_DOUBLE_PRECISION) << dVector3.x << ", " << dVector3.y << ", " << dVector3.z;
            aLabel->SetText(labelStream.str());
            aLabel->Update(aDelta);
        }
    }
    
    void DebugUI::UpdateLayout()
    {
        //Calculate the initial position of the top-most label
        float padding = DEBUG_LABEL_SPACING * ServiceLocator::GetPlatformLayer()->GetScale();
        vec2 position = vec2(padding, ServiceLocator::GetPlatformLayer()->GetHeight() - padding);
        
        //Cycle through and position the labels properly
        for(unsigned int i = 0; i < m_Labels.size(); i++)
        {
            m_Labels.at(i).label->SetLocalPosition(position);
            m_Labels.at(i).label->ModelMatrixIsDirty(true);
            position.y -= (m_Labels.at(i).label->GetHeight() + padding);
        }
    }
    
    Label* DebugUI::CreateDebugLabel()
    {
        //Calculate the Label's font size
        unsigned int fontSize = (unsigned int)(DEBUG_LABEL_FONT_SIZE * ServiceLocator::GetPlatformLayer()->GetScale());
        
        //Create a label object for the watch value
        Label* label = new Label("OpenSans-CondBold", "ttf", fontSize);
        label->SetColor(m_FontColor);
        label->SetAnchorPoint(0.0f, 1.0f);
        label->SetJustification(JustifyLeft);
        return label;
    }
    
    void DebugUI::EraseLabelAtIndex(int aIndex)
    {
        //Delete the label object
        SafeDelete(m_Labels.at(aIndex).label);
        
        //Remove the label from the label from the vector
        m_Labels.erase(m_Labels.begin() + aIndex);
    }
}