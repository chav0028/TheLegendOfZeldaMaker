//
//  ControllerData.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-28.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__ControllerData__
#define __GameDev2D__ControllerData__

#include "../../Core/BaseObject.h"


using namespace std;

namespace GameDev2D
{
    //The ControllerData class holds all relevant controller data, this class is managed by the
    //InputManager.
    class ControllerData : public BaseObject
    {
    public:
        ControllerData(void* device, int vendorId, int productId);
        ~ControllerData();
        
        //Updates the controller data, specifically to update the
        //duration a button has been pressed for.
        void Update(double delta);
        
        //Returns the device id (or on windows, the port that the controller is connected to)
        const void* GetDevice();
        
        //Returns the vendor id, (ie Sony, Microsoft)
        int GetVendorId();
        
        //Returns the product id (ie 360 Controller, PS3 controller)
        int GetProductId();
        
        //Add an input binding for a button to the controller data
        void AddButton(unsigned int binding);
        
        //Add an analog binding for a trigger or analog stick to the controller data, as well as the min, max and starting value
        void AddAnalog(unsigned int binding, long min, long max, long value, bool platformInverted = false);
        
        //Returns the number of button bindings
        unsigned long GetNumberOfButtonBindings();
        
        //Returns the number of analog bindings
        unsigned long GetNumberOfAnalogBindings();

        //Returns the button binding for an index
        unsigned int GetButtonBindingForIndex(unsigned int index);

        //Returns the index if the binding is a button or not, -1 is returned if the binding is NOT a button
        int IsButtonBinding(unsigned int binding);
        
        //Returns the index if the binding is a analog input or not, -1 is returned if the binding is NOT an analog input
        int IsAnalogBinding(unsigned int binding);
        
        //Returns the min and max values for an analog binding
        long GetMinAnalogValue(unsigned int binding);
        long GetMaxAnalogValue(unsigned int binding);
        
        //Sets the button's state for the button index
        void SetButtonValueAtIndex(unsigned int index, bool isDown);
        
        //Sets the analog input's value for the analog index
        void SetAnalogValueAtIndex(unsigned int index, long value);
        
        //Sets the button's state for the input binding
        void SetButtonValueForBinding(unsigned int binding, bool isDown);
        
        //Sets the analog input's value for the input binding
        void SetAnalogValueForBinding(unsigned int binding, long value);
        
        //Returns the button state for the button index
        bool IsButtonDownForIndex(unsigned int index);
        
        //Returns the analog value for the analog index
        long GetAnalogValueForIndex(unsigned int index);
        
        //Returns the button state for the input binding
        bool IsButtonDownForBinding(unsigned int binding);
        
        //Returns the analog value for the input binding
        long GetAnalogValueForBinding(unsigned int binding);

        //Sets the analog input has been inverted by the user settings
        void SetAnalogInputUserInverted(unsigned int binding, bool userInverted);
        
        //Returns wether the analog input has been inverted by the user settings
        bool IsAnalogInputUserInverted(unsigned int binding);

        //Returns wether the analog input has been platform inverted, OSX and Windows
        //do NOT always give the same output, this is for consistency across both platforms.
        bool IsAnalogInputPlatformInverted(unsigned int binding);

        //Sets the analog sticks dead zone
        void SetAnalogStickDeadZone(float deadZone);
        
        //Returns the analog sticks dead zone
        float GetAnalogStickDeadZone();
        
        //Returns the duration a button has been held down for (if at all)
        double GetButtonDuration(unsigned int binding);

        //Sets and returns the xinput controller state, we have to poll this data everytime
        //and compare it to the previous input data to check to see if there was input.
        #if _WIN32
        void SetStateInfo(XINPUT_STATE stateInfo);
        XINPUT_STATE GetStateInfo();
        #endif

    private:
        //Member variables
        void* m_Device;
        int m_VendorId;
        int m_ProductId;
        
        //Button data stuct
        struct ButtonData
        {
            unsigned int binding;
            bool isDown;
            double duration;
            
            ButtonData(unsigned int aBinding) { binding = aBinding; isDown = false; duration = 0.0; }
        };
        
        //Button bindings vector
        vector<ButtonData> m_ButtonBindings;
        
        //Analog data struct
        struct AnalogData
        {
            unsigned int binding;
            long min;
            long max;
            long value;
            bool isPlatformInverted;
            bool isUserInverted;
            
            AnalogData(unsigned int aBinding, long aMin, long aMax, long aValue, bool aPlatformInverted) { binding = aBinding; min = aMin; max = aMax; value = aValue; isPlatformInverted = aPlatformInverted; isUserInverted = false; }
        };
        
        //Analog bindings vector
        vector<AnalogData> m_AnalogBindings;
        
        //Member to hold the dead zone for an analog stick
        float m_DeadZone;

        //Member to hold the xinput controller state, we have
        //to poll this data everytime and compare it to the
        //previous input data to check to see if there was input.
        #if _WIN32
        XINPUT_STATE m_StateInfo;
        #endif
    };
}

#endif /* defined(__GameDev2D__ControllerData__) */
