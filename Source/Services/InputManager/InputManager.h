//
//  InputManager.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__InputManager__
#define __GameDev2D__InputManager__

#include "../GameService.h"
#include "../../Input/TouchData.h"
#include "../../Input/KeyData.h"
#include "../../Input/MouseData.h"
#include "../../Input/KeyCodes.h"
#include "../../Animation/Timer.h"


//Sony Playstation 3 vendor and product constants
#define HID_VENDOR_SONY 1356
#define HID_PRODUCT_PS3_CONTROLLER 616

//XBOX 360 vendor and product constants
#define HID_VENDOR_MICROSOFT 1118
#define HID_PRODUCT_XBOX_360_CONTROLLER 654

using namespace glm;
using namespace std;

namespace GameDev2D
{
    //Forward Declarations
    class MouseMovementEvent;
    class AccelerometerEvent;
    class GyroscopeEvent;
    class ControllerGeneric;
    class ControllerData;

    //The InputManager game service handles all input for all platforms, including mouse, keyboard,
    //touch, controller, accelerometer and gyroscope and dispatches events for the input it receieves.
    //This class dispatches the following events:
    // KEYBOARD_EVENT
    // MOUSE_MOVEMENT_EVENT
    // MOUSE_CLICK_EVENT
    // MOUSE_WHEEL_EVENT
    // CONTROLLER_PLUGGED_IN
    // CONTROLLER_UNPLUGGED
    // CONTROLLER_BUTTON_EVENT
    // CONTROLLER_ANALOG_EVENT
    // CONTROLLER_ANALOG_STICK_EVENT
    // TOUCH_EVENT
    // MULTI_TOUCH_ENABLED_EVENT
    // MULTI_TOUCH_DISABLED_EVENT
    // ACCELEROMETER_EVENT
    // ACCELEROMETER_ENABLED_EVENT
    // ACCELEROMETER_DISABLED_EVENT
    // GYROSCOPE_EVENT
    // GYROSCOPE_ENABLED_EVENT
    // GYROSCOPE_DISABLED_EVENT
    class InputManager : public GameService
    {
    public:
        InputManager();
        ~InputManager();
        
        //Used to Update and Draw the InputManager
        void Update(double delta);
        void Draw();

        //Returns the mouse position in the window
        vec2 GetMousePosition();

        //Returns wether the mouse button is pressed or not
        bool IsMouseButtonPressed(MouseButton mouseButton);
        
        //Returns wether the key is pressed or not
        bool IsKeyPressed(KeyCode keyCode);
        
        //Controller methods
        ControllerGeneric* GetControllerForIndex(unsigned int index);
        unsigned long GetNumberOfControllers();
        
        //Touch methods
        bool IsTouching();
        unsigned int GetNumberOfFingersTouching();
        
        //Getters methods for Accelermeter and Gyroscope data
        dvec3 GetAccelerometerData();
        dvec3 GetGyroscopeData();
        
        //Methods to handle Keyboard input
        void HandleKeyDown(KeyCode keyCode);
        void HandleKeyUp(KeyCode keyCode);
        
        //Methods to handle Mouse input
        void HandleLeftMouseDown(float x, float y);
        void HandleLeftMouseUp(float x, float y);
        void HandleCenterMouseDown(float x, float y);
        void HandleCenterMouseUp(float x, float y);
        void HandleRightMouseDown(float x, float y);
        void HandleRightMouseUp(float x, float y);
        void HandleMouseMoved(float x, float y, float deltaX, float deltaY);
        void HandleScrollWheel(float delta);
        
        //Methods to handle Controller input
        ControllerGeneric* HandleMatchedController(ControllerData* controllerData);
        void HandleRemovedController(void* device);
        void HandleControllerInput(ControllerGeneric* controller, unsigned int binding, long logicalValue);
        
        //Methods to handle Touch input
        void HandleTouchBegan(float x, float y, float previousX, float previousY);
        void HandleTouchMoved(float x, float y, float previousX, float previousY);
        void HandleTouchEnded(float x, float y, float previousX, float previousY);
        void HandleTouchCancelled(float x, float y, float previousX, float previousY);
        
        //Methods to handle Accelerometer and Gyroscope input
        void HandleAccelerometerData(double x, double y, double z);
        void HandleGyroscopeRotation(double x, double y, double z);
        
        //Conveniance methods used to determine if a controller is an XBOX 360 or PS3 controller
        bool IsMatchedControllerXbox360(int vendorId, int productId);
        bool IsMatchedControllerPS3(int vendorId, int productId);

        //Methods used to determine if this GameService should be drawn and updated
        bool CanUpdate();
        bool CanDraw();
    
    private:
        //Conveniance method to get the index for a touch event, based on the previous touch location
        int GetIndexForPreviousTouchLocation(vec2 previous);

        //Touch data
        vector<TouchData*> m_Touches;
        
        //Keyboard data
        KeyData m_Keys[KEY_CODE_COUNT];
        
        //Mouse data
        MouseData m_MouseData;
        
        //Controller data
        unsigned int m_ControllerId;
        vector<ControllerGeneric*> m_Controllers;
        
        //Accelerometer and Gyroscope data
        dvec3 m_AccelerometerData;
        dvec3 m_GyroscopeData;
        
        //Event object that are cached because they are thrown very 
        //frequently, this avoids having to call 'new' many times a second
        MouseMovementEvent* m_MouseMovementEvent;
        AccelerometerEvent* m_AccelerometerEvent;
        GyroscopeEvent* m_GyroscopeEvent;
    };
}

#endif /* defined(__GameDev2D__InputManager__) */
