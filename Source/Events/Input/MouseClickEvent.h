//
//  MouseClickEvent.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-22.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__MouseClickEvent__
#define __GameDev2D__MouseClickEvent__

#include "../Event.h"
#include "../../Input/MouseData.h"


namespace GameDev2D
{
    //There are 2 types (3 including the unkown state)
    //of mouse click events, the enum below describes them.
    enum MouseClickEventType
    {
        MouseClickUnknown,
        MouseClickDown,
        MouseClickUp
    };

    //The MouseClickEvent class is a subclass of the event class and is sent by the
    //InputManager whenever the user clicks one of the mouse buttons.
    class MouseClickEvent : public Event
    {
    public:
        MouseClickEvent(MouseClickEventType mouseClickEventType, MouseButton mouseButton, vec2 position, double duration);
        ~MouseClickEvent();
        
        //Returns the mouse click event type
        MouseClickEventType GetMouseClickEventType();
        
        //Returns which mouse button was clicked
        MouseButton GetMouseButton();
        
        //Returns the current position of the mouse
        vec2 GetPosition();
        
        //Returns the duration of the mouse click (from the initial mouseClickDown event)
        double GetDuration();
      
    protected:
        void LogEvent();
      
    private:
        MouseClickEventType m_MouseClickEventType;
        MouseButton m_MouseButton;
        vec2 m_Position;
        double m_Duration;
    };
    
    //Conveniance method to display the names for each mouse click event type
    const char* MouseClickEventTypeName(MouseClickEventType mouseClickEventType);
}

#endif /* defined(__GameDev2D__MouseClickEvent__) */
