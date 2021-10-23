//
//  ControllerPS3.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-26.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__ControllerPS3__
#define __GameDev2D__ControllerPS3__

#include "ControllerGeneric.h"


namespace GameDev2D
{
    //PS3 controller constants
    const unsigned int PS3_CONTROLLER_INPUT_COUNT = 21;
    const float PS3_CONTROLLER_DEFAULT_ANALOG_STICK_DEAD_ZONE = 0.2f;
    const float PS3_CONTROLLER_ANALOG_STICK_DEAD_ZONE_EPSILON = 0.00395f;

    //The ControllerPS3 class inherits from the ControllerGeneric class. It specifies all the
    //input bindings for the PS3 controller. Currently the PS3 controller is ONLY supported on Mac OSX
    class ControllerPS3 : public ControllerGeneric
    {
    public:
        ControllerPS3(ControllerData* controllerData, unsigned int controllerId);
        ~ControllerPS3();
        
        //Returns a string for the input binding
        const char* StringForInputBinding(unsigned int binding, bool specifyAnalogStickAxis = true);
        
        //Validates the input binding, it is an acutally PS3 controller binding
        bool IsBindingValid(unsigned int binding);
        
        //Returns that the binding is a PS3 analog stick input binding
        bool IsAnalogStick(unsigned int binding);

        //Returns that the analog value of the input binding
        float GetAnalogValue(unsigned int binding);

        //Input bindings for the left analog stick
        static unsigned int AnalogStickLeftHorizontal();
        static unsigned int AnalogStickLeftVertical();
        
        //Input bindings for the right analog stick
        static unsigned int AnalogStickRightHorizontal();
        static unsigned int AnalogStickRightVertical();
        
        //Input bindings for the directional pad
        static unsigned int DirectionalPadLeft();
        static unsigned int DirectionalPadRight();
        static unsigned int DirectionalPadUp();
        static unsigned int DirectionalPadDown();
        
        //Input bindings for the X, circle, square and triangle buttons
        static unsigned int ButtonX();
        static unsigned int ButtonCircle();
        static unsigned int ButtonSquare();
        static unsigned int ButtonTriangle();
        
        //Input binding for the Playstation button
        static unsigned int ButtonPlaystation();
        
        //Input bindings for the Start and Select buttons
        static unsigned int ButtonStart();
        static unsigned int ButtonSelect();
        
        //Input bindings for the left and right analog stick buttons
        static unsigned int ButtonLeftStick();
        static unsigned int ButtonRightStick();
        
        //Input bindings for the left and right shoulder buttons
        static unsigned int ButtonLeftShoulder();
        static unsigned int ButtonRightShoulder();
        
        //Input bindings for the left and right triggers
        static unsigned int TriggerLeft();
        static unsigned int TriggerRight();
      
    protected:
        //Pass in one of the input binding for an analog stick and it will return both
        //the x and y axis input binding, in that order, used by the input manager
        void GetAnalogStickBindings(unsigned int inBinding, unsigned int (&outBindings)[2]);
        
    private:
        unsigned int m_ValidBindings[PS3_CONTROLLER_INPUT_COUNT];
    };
}

#endif /* defined(__GameDev2D__Controller_PS3__) */
