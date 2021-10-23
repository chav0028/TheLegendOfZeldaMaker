//
//  ControllerXbox360.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-26.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__ControllerXbox360__
#define __GameDev2D__ControllerXbox360__

#include "ControllerGeneric.h"

namespace GameDev2D
{
    //XBOX 360 controller constants
    const unsigned int XBOX_360_CONTROLLER_INPUT_COUNT = 21;
    const float XBOX_360_CONTROLLER_DEFAULT_ANALOG_STICK_DEAD_ZONE = 0.2f;
    const float XBOX_360_CONTROLLER_ANALOG_STICK_DEAD_ZONE_EPSILON = 0.00002f;

    //The ControllerXbox360 class inherits from the ControllerGeneric class. It specifies all the
    //input bindings for the XBOX 360 controller. Currently the XBOX 360 controller is supported on Mac OSX AND Windows
    class ControllerXbox360 : public ControllerGeneric
    {
    public:
        ControllerXbox360(ControllerData* controllerData, unsigned int controllerId);
        ~ControllerXbox360();
        
        //Returns a string for the input binding
        const char* StringForInputBinding(unsigned int binding, bool specifyAnalogStickAxis = true);
        
        //Validates the input binding, it is an acutally XBOX 360 controller binding
        bool IsBindingValid(unsigned int binding);
        
        //Returns that the binding is a XBOX 360 analog stick input binding
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
        
        //Input bindings for the A, B, X, Y buttons
        static unsigned int ButtonA();
        static unsigned int ButtonB();
        static unsigned int ButtonX();
        static unsigned int ButtonY();
        
        //Input binding for the Xbox button, only works on OSX, Windows
        //does NOT send events for this button, the OS handles it itself.
        static unsigned int ButtonXbox();
        
        //Input bindings for the Start and Back buttons
        static unsigned int ButtonStart();
        static unsigned int ButtonBack();
        
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
        unsigned int m_ValidBindings[XBOX_360_CONTROLLER_INPUT_COUNT];
    };
}

#endif /* defined(__GameDev2D__ControllerXbox360__) */
