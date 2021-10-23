//
//  MouseMovementEvent.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__MouseEvent__
#define __GameDev2D__MouseEvent__

#include "../Event.h"


using namespace glm;

namespace GameDev2D
{
    //The MouseMovementEvent class is a subclass of the event class and is sent by the
    //InputManager whenever the mouse moves.
    class MouseMovementEvent : public Event
    {
    public:
        MouseMovementEvent();
        MouseMovementEvent(float x, float y, float deltaX, float deltaY);
        ~MouseMovementEvent();
        
        //Returns the current position of the mouse,
        //caution it could be outside of the active window
        vec2 GetPosition();
        
        //Returns the delta position of the mouse
        //(ie how much the mouse moved in the x and y axis)
        vec2 GetDeltaPosition();
        
    protected:
        void Set(float x, float y, float deltaX, float deltaY);
        
        void LogEvent();
        
        friend class InputManager;
      
    private:
        vec2 m_Position;
        vec2 m_DeltaPosition;
    };
}

#endif /* defined(__GameDev2D__MouseEvent__) */
