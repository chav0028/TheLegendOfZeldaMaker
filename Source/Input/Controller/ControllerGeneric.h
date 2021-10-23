//
//  ControllerGeneric.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-26.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__ControllerGeneric__
#define __GameDev2D__ControllerGeneric__

#include "../../Core/BaseObject.h"
#include "ControllerData.h"


using namespace std;

namespace GameDev2D
{
    //Forward declaration
    class ControllerEvent;

    //This ControllerGeneric class is the base class for all controller input, it can be subclassed by
    //specific controller implementations (360 and PS3 controllers, XBOX One and PS4 in the future).
    //This class can also be used for non-subclassed controller inputs, the input binding would just
    //have to be mapped manually by the user.
    class ControllerGeneric : public BaseObject
    {
    public:
        ControllerGeneric(const string& controllerType, ControllerData* controllerData, unsigned int controllerId);
        ControllerGeneric(ControllerData* controllerData, unsigned int controllerId);
        virtual ~ControllerGeneric();
        
        //Returns a string for the input binding
        virtual const char* StringForInputBinding(unsigned int binding, bool specifyAnalogStickAxis = true);
        
        //Returns wether the binding is for a button input
        virtual bool IsBindingValid(unsigned int binding);
        
        //Returns wether the binding is for an analog input
        virtual bool IsAnalogStick(unsigned int binding);
        
        //Returns the button state for a specific binding
        virtual bool IsButtonDown(unsigned int binding);
        
        //Returns the analog value for a specific binding
        virtual float GetAnalogValue(unsigned int binding);

        //Sets wether the analog stick (for the binding) is inverted by the user's settings
        void SetAnalogStickInverted(unsigned int binding, bool isInverted);
        
        //Returns wether the analog stick (for the binding) is inverted by the user's settings
        bool IsAnalogStickIsInverted(unsigned int binding);
        
        //Returns the controller id, different from device id, this is the order the controller's were connected
        unsigned int GetId();
        
    protected:    
        //Returns the controller data, used by the input manager
        ControllerData* GetControllerData();
        
        //Returns the device id (or input port on windows), used by the input manager
        const void* GetDevice();
        
        //Pass in one of the input binding for an analog stick and it will return both
        //the x and y axis input binding, in that order, used by the input manager
        virtual void GetAnalogStickBindings(unsigned int inBinding, unsigned int (&outBindings)[2]);
        
        //Friend classes
        friend class InputManager;
        friend class ControllerButtonEvent;
        friend class GameWindow;
    
        //Member variables
        ControllerData* m_ControllerData;
        unsigned int m_ControllerId;
    };
}

#endif /* defined(__GameDev2D__ControllerBase__) */
