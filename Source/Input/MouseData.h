//
//  MouseData.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-22.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__MouseData__
#define __GameDev2D__MouseData__

#include "../Core/BaseObject.h"


using namespace glm;

namespace GameDev2D
{
    //This enum describes the mouse
    //buttons on a standard mouse
    enum MouseButton
    {
        MouseButtonLeft = 0,
        MouseButtonCenter,
        MouseButtonRight,
        MouseButtonCount,
        MouseButtonUnknown = -1,
    };

    //The MouseData class holds all relevant mouse data, this class is managed by the InputManager.
    class MouseData : public BaseObject
    {
    public:
        MouseData();
        ~MouseData();
        
        //Sets the current mouse's position
        void SetPosition(vec2 position);
        
        //Returns the current mouse's position
        vec2 GetPosition();
        
        //Returns the previous mouse's position
        vec2 GetPreviousPosition();
        
        //Sets that a mouse button has been pressed or released
        void SetIsPressed(MouseButton mouseButton, bool isPressed);
        
        //Returns wether a mouse button is pressed or not
        bool IsPressed(MouseButton mouseButton);
        
        //Returns the duration a mouse button has been pressed for (if at all)
        double GetDuration(MouseButton mouseButton);
        
        //Updates the mouse data, specifically to update the duration a
        //mouse button has been pressed for.
        void Update(double delta);
        
    private:
        //Member variables
        vec2 m_Position;
        vec2 m_PreviousPosition;
        double m_Duration[MouseButtonCount];
        bool m_IsPressed[MouseButtonCount];
    };
    
    //Conveniance method to convert a mouse button to a c-string
    const char* MouseButtonName(MouseButton mouseButton);
}

#endif /* defined(__GameDev2D__MouseData__) */
