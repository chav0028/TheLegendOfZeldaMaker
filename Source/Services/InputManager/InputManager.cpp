 //
//  InputManager.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "InputManager.h"
#include "../ServiceLocator.h"
#include "../../Platforms/PlatformLayer.h"
#include "../../Graphics/Primitives/Line.h"
#include "../../Input/Controller/ControllerData.h"
#include "../../Input/Controller/ControllerGeneric.h"
#include "../../Input/Controller/ControllerPS3.h"
#include "../../Input/Controller/ControllerXbox360.h"
#include "../../Events/Input/KeyEvent.h"
#include "../../Events/Input/TouchEvent.h"
#include "../../Events/Input/ControllerEvent.h"
#include "../../Events/Input/MouseMovementEvent.h"
#include "../../Events/Input/MouseClickEvent.h"
#include "../../Events/Input/MouseScrollWheelEvent.h"
#include "../../Events/Input/AccelerometerEvent.h"
#include "../../Events/Input/GyroscopeEvent.h"


namespace GameDev2D
{
    InputManager::InputManager() : GameService("InputManager"),
        m_ControllerId(0),
        m_AccelerometerData(dvec3(0.0, 0.0, 0.0)),
        m_GyroscopeData(dvec3(0.0, 0.0, 0.0)),
        m_MouseMovementEvent(nullptr),
        m_AccelerometerEvent(nullptr),
        m_GyroscopeEvent(nullptr)
    {
        //Since accelerometer and gyroscope events are consistent,
        //we can re-use the same event objects instead of creating
        //a new object every n seconds (using 0.01 seconds)
        m_MouseMovementEvent = new MouseMovementEvent();
        m_AccelerometerEvent = new AccelerometerEvent();
        m_GyroscopeEvent = new GyroscopeEvent();
    }
    
    InputManager::~InputManager()
    {
        //Clear the touches vector
        for(unsigned int i = 0; i < m_Touches.size(); i++)
        {
            SafeDelete(m_Touches.at(i));
        }
        m_Touches.clear();
        
        //Delete the accelerometer and gryoscope event objects
        SafeDelete(m_MouseMovementEvent);
        SafeDelete(m_AccelerometerEvent);
        SafeDelete(m_GyroscopeEvent);
    }

    void InputManager::Update(double aDelta)
    {
        //Cycle through the touch data and update them
        unsigned int i = 0;
        for(i = 0; i < m_Touches.size(); i++)
        {
            m_Touches.at(i)->Update(aDelta);
        }
        
        //Cycle through the key data and update them
        for(i = 0; i < KEY_CODE_COUNT; i++)
        {
            m_Keys[i].Update(aDelta);
        }
        
        //Update the mouse data
        m_MouseData.Update(aDelta);
        
        //Cycle through the connected controllers and update them
        for(i = 0; i < m_Controllers.size(); i++)
        {
            m_Controllers.at(i)->GetControllerData()->Update(aDelta);
        }
        
        //Iterate through the touch data and erase any that are flagged from removal
        #if DEBUG && DRAW_TOUCH_DEBUG_DATA
        vector<TouchData>::iterator iter = m_Touches.begin();
        while (iter != m_Touches.end())
        {
            if(iter->CanErase() == true)
            {
                //Erase returns the new iterator
                iter = m_Touches.erase(iter);
            }
            else
            {
                ++iter;
            }
        }
        #endif
    }

    void InputManager::Draw()
    {
        //Cycle through the touch data and draw any 
        for(unsigned int i = 0; i < m_Touches.size(); i++)
        {
            m_Touches.at(i)->Draw();
        }
        
        //Are there any connected controllers? If there is, draw the debug 
        //controller data for the controller connected on port 0
        if(m_Controllers.size() > 0)
        {
            //Cache the width and height
            float width = (float)ServiceLocator::GetPlatformLayer()->GetWidth();
            float height = (float)ServiceLocator::GetPlatformLayer()->GetHeight();
            
            //Calculate the position and radius
            vec2 outerPosition1 = vec2(width * 0.25f, height * 0.15f);
            vec2 outerPosition2 = vec2(width * 0.75f, height * 0.15f);
            float outerRadius = 40.0f * ServiceLocator::GetPlatformLayer()->GetScale();
            float innerRadius = 2.0f * ServiceLocator::GetPlatformLayer()->GetScale();
        
            //Draw the left outer circle
            Circle outerCircle(outerPosition1, outerRadius);
            outerCircle.SetIsFilled(false);
            outerCircle.SetColor(Color::BlackColor());
            outerCircle.Draw();
        
            //Draw the right outer circle
            outerCircle.SetLocalPosition(outerPosition2);
            outerCircle.Draw();
        
            if(m_Controllers.at(0)->GetType() == "ControllerXbox360" || m_Controllers.at(0)->GetType() == "ControllerPS3")
            {
                //Draw left inner circler
                bool isPS3 = m_Controllers.at(0)->GetType() == "ControllerPS3";
                unsigned int leftBinding[] = {isPS3 ? ControllerPS3::AnalogStickLeftHorizontal() : ControllerXbox360::AnalogStickLeftHorizontal(), isPS3 ? ControllerPS3::AnalogStickLeftVertical() : ControllerXbox360::AnalogStickLeftVertical()};
                vec2 left = vec2(m_Controllers.at(0)->GetAnalogValue(leftBinding[0]), m_Controllers.at(0)->GetAnalogValue(leftBinding[1]));
                vec2 innerPosition1 = outerPosition1 + (left * outerRadius);
                Circle innerCircle(innerPosition1, innerRadius);
                innerCircle.SetIsFilled(true);
                innerCircle.SetColor(Color::BlackColor());
                innerCircle.Draw();
                
                //Draw right inner circler
                unsigned int rightBinding[] = {isPS3 ? ControllerPS3::AnalogStickRightHorizontal() : ControllerXbox360::AnalogStickRightHorizontal(), isPS3 ? ControllerPS3::AnalogStickRightVertical() : ControllerXbox360::AnalogStickRightVertical()};
                vec2 right = vec2(m_Controllers.at(0)->GetAnalogValue(rightBinding[0]), m_Controllers.at(0)->GetAnalogValue(rightBinding[1]));
                vec2 innerPosition2 = outerPosition2 + (right * outerRadius);
                innerCircle.SetLocalPosition(innerPosition2);
                innerCircle.Draw();
                
            }
        }
    }
    
    vec2 InputManager::GetMousePosition()
    {
        return m_MouseData.GetPosition();
    }
    
    bool InputManager::IsMouseButtonPressed(MouseButton aMouseButton)
    {
        return m_MouseData.IsPressed(aMouseButton);
    }
    
    bool InputManager::IsKeyPressed(KeyCode aKeyCode)
    {
        return m_Keys[aKeyCode].IsPressed();
    }

    ControllerGeneric* InputManager::GetControllerForIndex(unsigned int aIndex)
    {
        if(aIndex < GetNumberOfControllers())
        {
            return m_Controllers.at(aIndex);
        }
        return nullptr;
    }
    
    unsigned long InputManager::GetNumberOfControllers()
    {
        return m_Controllers.size();
    }
    
    bool InputManager::IsTouching()
    {
        return GetNumberOfFingersTouching() > 0;
    }
    
    unsigned int InputManager::GetNumberOfFingersTouching()
    {
        unsigned int count = 0;
        
        #if DEBUG && DRAW_TOUCH_DEBUG_DATA
        //We only want to count the number of fingers that are still touching,
        //in Debug draw mode they are kept alive a little longer than the
        //last touch ended or cancelled event
        for(unsigned int i = 0; i < m_Touches.size(); i++)
        {
            if(m_Touches.at(i).HasEnded() == false)
            {
                count++;
            }
        }
        #else
        count = (unsigned int)m_Touches.size();
        #endif
        
        //Return the fingers touching count
        return count;
    }
    
    dvec3 InputManager::GetAccelerometerData()
    {
        return m_AccelerometerData;
    }
    
    dvec3 InputManager::GetGyroscopeData()
    {
        return m_GyroscopeData;
    }
    
    void InputManager::HandleKeyDown(KeyCode aKeyCode)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }
    
        //If the key is pressed, dispatch a KeyRepeat event
        if(m_Keys[aKeyCode].IsPressed() == true)
        {
            DispatchEvent(new KeyEvent(KeyRepeat, aKeyCode, m_Keys[aKeyCode].GetDuration()));
        }
        else
        {
            //Other wise, set the key as pressed
            m_Keys[aKeyCode].SetIsPressed(true);
            
            //And dispatch and KeyDown event
            DispatchEvent(new KeyEvent(KeyDown, aKeyCode, 0.0));
        }
    }
    
    void InputManager::HandleKeyUp(KeyCode aKeyCode)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }
    
        //Dispatch a KeyUp event
        DispatchEvent(new KeyEvent(KeyUp, aKeyCode, m_Keys[aKeyCode].GetDuration()));
    
        //And set that the key is no longer pressed
        m_Keys[aKeyCode].SetIsPressed(false);
    }
    
    void InputManager::HandleLeftMouseDown(float aX, float aY)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }
    
        //Set that the left mouse button is pressed
        m_MouseData.SetIsPressed(MouseButtonLeft, true);
    
        //If the simulate touch event preprocessor is enabled handle this as a touch event 
        //otherwise dispatch a MouseClickEvent
        #if SIMULATE_TOUCH_EVENTS_WITH_MOUSE
        HandleTouchBegan(aX, aY, aX, aY);
        #else
        DispatchEvent(new MouseClickEvent(MouseClickDown, MouseButtonLeft, vec2(aX, aY), 0.0));
        #endif
    }
    
    void InputManager::HandleLeftMouseUp(float aX, float aY)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }
    
        //If the simulate touch event preprocessor is enabled handle this as a touch event 
        //otherwise dispatch a MouseClickEvent
        #if SIMULATE_TOUCH_EVENTS_WITH_MOUSE
        HandleTouchEnded(aX, aY, m_MouseData.GetPreviousPosition().x, m_MouseData.GetPreviousPosition().y);
        #else
        DispatchEvent(new MouseClickEvent(MouseClickUp, MouseButtonLeft, vec2(aX, aY), m_MouseData.GetDuration(MouseButtonLeft)));
        #endif
        
        //Set that the left mouse button is no longer pressed
        m_MouseData.SetIsPressed(MouseButtonLeft, false);
    }
    
    void InputManager::HandleCenterMouseDown(float aX, float aY)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }
    
        //Set that the center mouse button is pressed
        m_MouseData.SetIsPressed(MouseButtonCenter, true);
    
        //Lastly, dispatch a MouseClickDown event for the center button
        DispatchEvent(new MouseClickEvent(MouseClickDown, MouseButtonCenter, vec2(aX, aY), 0.0));
    }
    
    void InputManager::HandleCenterMouseUp(float aX, float aY)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }
    
        //Dispatch a MouseClickUp event for the center button
        DispatchEvent(new MouseClickEvent(MouseClickUp, MouseButtonCenter, vec2(aX, aY), m_MouseData.GetDuration(MouseButtonCenter)));
        
        //Set that the center mouse button is no longer pressed
        m_MouseData.SetIsPressed(MouseButtonCenter, false);
    }
    
    void InputManager::HandleRightMouseDown(float aX, float aY)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }
    
        //Set that the right mouse button is pressed
        m_MouseData.SetIsPressed(MouseButtonRight, true);
    
        //Lastly, dispatch a MouseClickDown event for the right button
        DispatchEvent(new MouseClickEvent(MouseClickDown, MouseButtonRight, vec2(aX, aY), 0.0));
    }
    
    void InputManager::HandleRightMouseUp(float aX, float aY)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }
    
        //Dispatch a MouseClickUp event for the right button
        DispatchEvent(new MouseClickEvent(MouseClickUp, MouseButtonRight, vec2(aX, aY), m_MouseData.GetDuration(MouseButtonRight)));
        
        //Set that the right mouse button is no longer pressed
        m_MouseData.SetIsPressed(MouseButtonRight, false);
    }
    
    void InputManager::HandleMouseMoved(float aX, float aY, float aDeltaX, float aDeltaY)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }
        
        //Set the mouse data's position
        m_MouseData.SetPosition(vec2(aX, aY));
    
        //If the simulate touch event preprocessor is enabled handle this as a touch event 
        //otherwise dispatch a MouseClickEvent
        #if SIMULATE_TOUCH_EVENTS_WITH_MOUSE
        if(IsMouseButtonPressed(MouseButtonLeft) == true)
        {
            if(aX >= 0.0f && aX <= ServiceLocator::GetPlatformLayer()->GetWidth() &&
               aY >= 0.0f && aY <= ServiceLocator::GetPlatformLayer()->GetHeight())
            {
                HandleTouchMoved(aX, aY, aX - aDeltaX, aY + aDeltaY);
            }
            else
            {
                HandleTouchCancelled(aX, aY, aX - aDeltaX, aY + aDeltaY);
            }
        }
        #else
        //Do we have a cached mouse movement event object? If we do set the mouse data and dispatch it, 
        //otherwise make a new mouse movement event object and dispatch it
        if(m_MouseMovementEvent != nullptr)
        {
            m_MouseMovementEvent->Set(aX, aY, aDeltaX, aDeltaY);
            DispatchEvent(m_MouseMovementEvent, false);
        }
        else
        {
            DispatchEvent(new MouseMovementEvent(aX, aY, aDeltaX, aDeltaY));
        }
        #endif
    }
    
    void InputManager::HandleScrollWheel(float aDelta)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }
    
        //Disptach a scroll wheel event
        DispatchEvent(new MouseScrollWheelEvent(aDelta));
    }
    
    ControllerGeneric* InputManager::HandleMatchedController(ControllerData* aControllerData)
    {
        ControllerGeneric* controller = nullptr;
    
        //Safety check the controller data
        if(aControllerData != nullptr)
        {
            //Increment the controller id
            m_ControllerId++;

            //Determine the type of controller this is, currently only the XBOX 360 and Playstation 3 controllers are fully supported
            if(aControllerData->GetVendorId() == HID_VENDOR_SONY && aControllerData->GetProductId() == HID_PRODUCT_PS3_CONTROLLER)
            {
                controller = new ControllerPS3(aControllerData, m_ControllerId);
            }
            else if(aControllerData->GetVendorId() == HID_VENDOR_MICROSOFT && aControllerData->GetProductId() == HID_PRODUCT_XBOX_360_CONTROLLER)
            {
                controller = new ControllerXbox360(aControllerData, m_ControllerId);
            }
            else
            {
                controller = new ControllerGeneric(aControllerData, m_ControllerId);
            }
        
            //Dispatch plugged in controller event
            DispatchEvent(new Event(CONTROLLER_PLUGGED_IN, controller));
        
            //Add the controller to the controllers vector
            m_Controllers.push_back(controller);
        }
        
        //Lastly return the controller
        return controller;
    }
    
    void InputManager::HandleRemovedController(void* aDevice)
    {
        for(unsigned int i = 0; i < m_Controllers.size(); i++)
        {
            if(m_Controllers.at(i)->GetDevice() == aDevice)
            {
                //Dispatch uplugged event
                DispatchEvent(new Event(CONTROLLER_UNPLUGGED, m_Controllers.at(i)));
                
                //Delete the controller object
                SafeDelete(m_Controllers.at(i));
                
                //Erase the controller pointer from the controller vector
                m_Controllers.erase(m_Controllers.begin() + i);
                
                //Lastly, break out of the for loop
                break;
            }
        }
    }
    
    void InputManager::HandleControllerInput(ControllerGeneric* aController, unsigned int aBinding, long aLogicalValue)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }
    
        //Safety check the controller pointer
        if(aController != nullptr)
        {
            //Get the button index for the binding, safety check it
            int index = aController->GetControllerData()->IsButtonBinding(aBinding);
            if(index != -1)
            {
                //Set the logical value (pressed or not) for the button
                aController->GetControllerData()->SetButtonValueAtIndex(index, aLogicalValue == 1);

                //And dispatch a ControllerButtonEvent
                DispatchEvent(new ControllerButtonEvent(aController, aBinding));
                
                //Return out of the method, we are down now
                return;
            }
            
            //Is the input an analog binding (analog stick or trigger)
            index = aController->GetControllerData()->IsAnalogBinding(aBinding);
            if(index != -1)
            {
                //Is this analog input an analog stick
                if(aController->IsAnalogStick(aBinding) == true)
                {
                    //Get the x and y binding for the analog stick
                    unsigned int bindings[2];
                    aController->GetAnalogStickBindings(aBinding, bindings);
                
                    //Get the last logical x and y value for the controller
                    long lastLogicalValueX = aController->GetControllerData()->GetAnalogValueForBinding(bindings[0]);
                    long lastLogicalValueY = aController->GetControllerData()->GetAnalogValueForBinding(bindings[1]);
                
                    //Set the newly updated logical value
                    aController->GetControllerData()->SetAnalogValueAtIndex(index, aLogicalValue);
                
                    //Calculate the deadzone for the controller
                    float deadZone = aController->GetControllerData()->GetAnalogStickDeadZone();
                    vec2 stickInput = vec2(aController->GetAnalogValue(bindings[0]), aController->GetAnalogValue(bindings[1]));
                    float magnitude = sqrtf((stickInput.x * stickInput.x) + (stickInput.y * stickInput.y));

                    //Is the magnitude less than the deadzone? if it is, zero the analog input
                    if(magnitude < deadZone)
                    {
                        stickInput = vec2(0.0f, 0.0f);
                    }
                    
                    //Get the min and max bounds for the x and y axis
                    long minX = aController->GetControllerData()->GetMinAnalogValue(bindings[0]);
                    long maxX = aController->GetControllerData()->GetMaxAnalogValue(bindings[0]);
                    long minY = aController->GetControllerData()->GetMinAnalogValue(bindings[1]);
                    long maxY = aController->GetControllerData()->GetMaxAnalogValue(bindings[1]);
                    
                    //Determine if we need to account for platform and user inverted axis
                    bool platformInvertedX = aController->GetControllerData()->IsAnalogInputPlatformInverted(bindings[0]);
                    bool platformInvertedY = aController->GetControllerData()->IsAnalogInputPlatformInverted(bindings[1]);
                    bool userInvertedX = aController->GetControllerData()->IsAnalogInputUserInverted(bindings[0]);
                    bool userInvertedY = aController->GetControllerData()->IsAnalogInputUserInverted(bindings[1]);

                    //calculate the newly calculated x and y value, based on the min and max bounds
                    long x = long((((stickInput.x * (platformInvertedX == true ? -1.0f : 1.0f) * (userInvertedX == true ? -1.0f : 1.0f)) + 1) / 2) * (maxX - minX) + minX);
                    long y = long((((stickInput.y * (platformInvertedY == true ? -1.0f : 1.0f) * (userInvertedY == true ? -1.0f : 1.0f)) + 1) / 2) * (maxY - minY) + minY);

                    //Apply the deadzone to the controller data
                    aController->GetControllerData()->SetAnalogValueForBinding(bindings[0], x);
                    aController->GetControllerData()->SetAnalogValueForBinding(bindings[1], y);

                    //Calculate the last x and y values and magnitude
                    float lastX = ((lastLogicalValueX - minX) / (float)(maxX - minX) * 2.0f - 1.0f) * (platformInvertedX == true ? -1.0f : 1.0f) * (userInvertedX == true ? -1.0f : 1.0f);
                    float lastY = ((lastLogicalValueY - minY) / (float)(maxY - minY) * 2.0f - 1.0f) * (platformInvertedY == true ? -1.0f : 1.0f) * (userInvertedY == true ? -1.0f : 1.0f);
                    float lastMagnitude = sqrtf((lastX * lastX) + (lastY * lastY));
                
                    //Only dispatch the analog stick event if BOTH the x and y axis do NOT equal zero
                    if((stickInput.x != 0.0f && stickInput.y != 0.0f) || lastMagnitude > deadZone)
                    {
                        DispatchEvent(new ControllerAnalogStickEvent(aController, bindings));
                    }
                }
                else
                {
                    //Set the logical value for the analog input
                    aController->GetControllerData()->SetAnalogValueAtIndex(index, aLogicalValue);
                
                    //Lastly, dispatch an analog controller (ie triggers) event
                    DispatchEvent(new ControllerAnalogEvent(aController, aBinding));
                }
            }
        }
    }
    
    void InputManager::HandleTouchBegan(float aX, float aY, float aPreviousX, float aPreviousY)
    {
        //Are we simulating touch events with a mouse, if we are the data is handled differently
        #if SIMULATE_TOUCH_EVENTS_WITH_MOUSE
        vec2 touchLocation = vec2(aX, aY);
        vec2 previousLocation = vec2(aPreviousX, aPreviousY);
        unsigned int index = 0;
        #else
        int height = ServiceLocator::GetPlatformLayer()->GetHeight();
        vec2 touchLocation = vec2(aX, height - aY);
        vec2 previousLocation = vec2(aPreviousX, height - aPreviousY);
        unsigned int index = (unsigned int)m_Touches.size();
        #endif
        
        //Dispatch a TouchBegan event
        DispatchEvent(new TouchEvent(TouchBegan, index, touchLocation, previousLocation, 0.0));

        //Add the touch location to the touch data, this is used for debug drawing
        m_Touches.push_back(new TouchData(touchLocation, previousLocation));
    }
    
    void InputManager::HandleTouchMoved(float aX, float aY, float aPreviousX, float aPreviousY)
    {
        //Are we simulating touch events with a mouse, if we are the data is handled differently
        #if SIMULATE_TOUCH_EVENTS_WITH_MOUSE
        vec2 touchLocation = vec2(aX, aY);
        vec2 previousLocation = vec2(aPreviousX, aPreviousY);
        int index = m_Touches.size() - 1;
        #else
        int height = ServiceLocator::GetPlatformLayer()->GetHeight();
        vec2 touchLocation = vec2(aX, height - aY);
        vec2 previousLocation = vec2(aPreviousX, height - aPreviousY);
        int index = GetIndexForPreviousTouchLocation(previousLocation);
        assert(index != -1);
        #endif
        
        //Safety check that the index isn't -1 and that the touch hasn't ended
        if(index != -1 && m_Touches.at(index)->HasEnded() == false)
        {
            //Dispatch a TouchMoved event
            DispatchEvent(new TouchEvent(TouchMoved, index, touchLocation, previousLocation, m_Touches.at(index)->GetDuration()));
        
            //Add the touch location to the touch data, this is used for debug drawing
            m_Touches.at(index)->AddTouchLocation(touchLocation, previousLocation);
        }
    }
    
    void InputManager::HandleTouchEnded(float aX, float aY, float aPreviousX, float aPreviousY)
    {
        //Are we simulating touch events with a mouse, if we are the data is handled differently
        #if SIMULATE_TOUCH_EVENTS_WITH_MOUSE
        vec2 touchLocation = vec2(aX, aY);
        vec2 previousLocation = vec2(aPreviousX, aPreviousY);
        int index = m_Touches.size() - 1;
        #else
        int height = ServiceLocator::GetPlatformLayer()->GetHeight();
        vec2 touchLocation = vec2(aX, height - aY);
        vec2 previousLocation = vec2(aPreviousX, height - aPreviousY);
        int index = GetIndexForPreviousTouchLocation(previousLocation);
        assert(index != -1);
        #endif
        
        //Safety check that the index isn't -1 and that the touch hasn't ended
        if(index != -1 && m_Touches.at(index)->HasEnded() == false)
        {
            //Dispatch a TouchEnded event
            DispatchEvent(new TouchEvent(TouchEnded, index, touchLocation, previousLocation, m_Touches.at(index)->GetDuration()));
        
            //Add the touch location to the touch data, this is used for debug drawing
            #if DEBUG && DRAW_TOUCH_DEBUG_DATA
            m_Touches.at(index)->AddTouchLocation(touchLocation, previousLocation, true);
            #else
            SafeDelete(m_Touches.at(index));
            m_Touches.erase(m_Touches.begin() + index);
            #endif
        }
    }
    
    void InputManager::HandleTouchCancelled(float aX, float aY, float aPreviousX, float aPreviousY)
    {
        //Are we simulating touch events with a mouse, if we are the data is handled differently
        #if SIMULATE_TOUCH_EVENTS_WITH_MOUSE
        vec2 touchLocation = vec2(aX, aY);
        vec2 previousLocation = vec2(aPreviousX, aPreviousY);
        int index = m_Touches.size() - 1;
        #else
        int height = ServiceLocator::GetPlatformLayer()->GetHeight();
        vec2 touchLocation = vec2(aX, height - aY);
        vec2 previousLocation = vec2(aPreviousX, height - aPreviousY);
        int index = GetIndexForPreviousTouchLocation(previousLocation);
        assert(index != -1);
        #endif
        
        //Safety check that the index isn't -1 and that the touch hasn't ended
        if(index != -1 && m_Touches.at(index)->HasEnded() == false)
        {
            //Dispatch a TouchCancelled event
            DispatchEvent(new TouchEvent(TouchCancelled, index, touchLocation, previousLocation, m_Touches.at(index)->GetDuration()));
        
            //Add the touch location to the touch data, this is used for debug drawing
            #if DEBUG && DRAW_TOUCH_DEBUG_DATA
            m_Touches.at(index)->AddTouchLocation(touchLocation, previousLocation, true);
            #else
            SafeDelete(m_Touches.at(index));
            m_Touches.erase(m_Touches.begin() + index);
            #endif
        }
    }
    
    void InputManager::HandleAccelerometerData(double aX, double aY, double aZ)
    {
        //Set the accelerometer data
        m_AccelerometerData.x = aX;
        m_AccelerometerData.y = aY;
        m_AccelerometerData.z = aZ;
    
        //Do we have an Accelerometer event object
        if(m_AccelerometerEvent != nullptr)
        {
            //Set the acceleromter data and dispatch the AcceleromterEvent
            m_AccelerometerEvent->Set(aX, aY, aZ);
            DispatchEvent(m_AccelerometerEvent, false);
        }
        else
        {
            //Otherwise dispatch a new Accelerometer event
            DispatchEvent(new AccelerometerEvent(aX, aY, aZ));
        }
    }
    
    void InputManager::HandleGyroscopeRotation(double aX, double aY, double aZ)
    {
        //Set the gyroscope data
        m_GyroscopeData.x = aX;
        m_GyroscopeData.y = aY;
        m_GyroscopeData.z = aZ;
    
        //Do we have an Gyroscope event object
        if(m_GyroscopeEvent != nullptr)
        {
            //et the gyroscope data and dispatch the GyroscopeEvent
            m_GyroscopeEvent->Set(aX, aY, aZ);
            DispatchEvent(m_GyroscopeEvent, false);
        }
        else
        {
            //Otherwise dispatch a new Gyroscope event
            DispatchEvent(new GyroscopeEvent(aX, aY, aZ));
        }
    }
    
    int InputManager::GetIndexForPreviousTouchLocation(vec2 aPrevious)
    {
        for(unsigned int i = 0; i < m_Touches.size(); i++)
        {
            if(m_Touches.at(i)->HasEnded() == false && m_Touches.at(i)->ContainsPrevious(aPrevious) == true)
            {
                return i;
            }
        }
        return -1;
    }
    
    bool InputManager::IsMatchedControllerXbox360(int aVendorId, int aProductId)
    {
        return aVendorId == HID_VENDOR_MICROSOFT && aProductId == HID_PRODUCT_XBOX_360_CONTROLLER;
    }

    bool InputManager::IsMatchedControllerPS3(int aVendorId, int aProductId)
    {
        return aVendorId == HID_VENDOR_SONY && aProductId == HID_PRODUCT_PS3_CONTROLLER;
    }
    
    bool InputManager::CanUpdate()
    {
        return true;
    }
    
    bool InputManager::CanDraw()
    {
    #if DEBUG && DRAW_DEBUG_UI && (DRAW_TOUCH_DEBUG_DATA || DRAW_JOYSTICK_DATA)
        return true;
    #else
        return false;
    #endif
    }
}