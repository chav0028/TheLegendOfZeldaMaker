//
//  ControllerEvent.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__GamepadEvent__
#define __GameDev2D__GamepadEvent__

#include "../Event.h"
#include "../../Input/Controller/ControllerGeneric.h"


using namespace glm;

namespace GameDev2D
{
    //The ControllerEvent class is a base class for all controller event class, it is a subclass
    //of the event class and is sent by the InputManager whenever controller data is recieved.
    class ControllerEvent : public Event
    {
    public:
        ControllerEvent(const string& controllerEvent, ControllerGeneric* controller, unsigned int inputBinding);
        virtual ~ControllerEvent();
        
        //Returns the controller that is sending the input
        ControllerGeneric* GetController();
        
        //Returns the controller input binding
        unsigned int GetInputBinding();
        
    private:
        ControllerGeneric* m_Controller;
        unsigned int m_InputBinding;
        
    };
    
    //The ControllerButtonEvent class is a subclass of the ControllerEvent class and is sent by the
    //InputManager whenever the user presses or releases one of the controller's buttons.
    class ControllerButtonEvent : public ControllerEvent
    {
    public:
        ControllerButtonEvent(ControllerGeneric* controller, unsigned int inputBinding);
        ~ControllerButtonEvent();
        
        //Returns wether the button is held down or not
        bool IsButtonDown();
        
        //Returns the duration the button was held down for
        double GetDuration();
        
    protected:
        void LogEvent();
    };
    
    //The ControllerAnalogEvent class is a subclass of the ControllerEvent class and is sent by the
    //InputManager whenever the user presses or releases one of the controller's analog inputs (ie triggers).
    class ControllerAnalogEvent : public ControllerEvent
    {
    public:
        ControllerAnalogEvent(ControllerGeneric* controller, unsigned int inputBinding);
        ~ControllerAnalogEvent();
        
        //Returns the analog value of the input, between 0.0f and 1.0f
        float GetAnalogValue();
    
    protected:
        void LogEvent();
    };
    
    
    //The ControllerAnalogStickEvent class is a subclass of the ControllerEvent class and is sent by the
    //InputManager whenever the user moves one of the analog sticks on the controller.
    class ControllerAnalogStickEvent : public ControllerEvent
    {
    public:
        ControllerAnalogStickEvent(ControllerGeneric* controller, unsigned int inputBinding[2]);
        ~ControllerAnalogStickEvent();
        
        //Returns the input binding for the x-axis
        unsigned int GetInputBindingX();
        
        //Returns the input binding for the y-axis
        unsigned int GetInputBindingY();
        
        //Returns the analog value for the x-axis, between -1.0f and 1.0f
        float GetAnalogValueX();
        
        //Returns the analog value for the y-axis, between -1.0f and 1.0f
        float GetAnalogValueY();
        
        //Returns the analog value for the x-axis and y-axis, between -1.0f and 1.0f
        vec2 GetAnalogValue();
    
    protected:
        void LogEvent();
        
    private:
        unsigned int m_InputBindingY;
    };
}

#endif /* defined(__GameDev2D__GamepadEvent__) */
