//
//  KeyEvent.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__KeyEvent__
#define __GameDev2D__KeyEvent__

#include "../Event.h"
#include "../../Input/KeyCodes.h"


namespace GameDev2D
{
    //There are 3 types (4 including the unkown state)
    //of key events, the enum below describes them.
    enum KeyEventType
    {
        KeyUnknown,
        KeyDown,
        KeyUp,
        KeyRepeat
    };

    //The KeyEvent class is a subclass of the event class and is sent by the
    //InputManager when new key presses are recieved.
    class KeyEvent : public Event
    {
    public:
        KeyEvent(KeyEventType keyEventType, KeyCode keycode, double duration);
        ~KeyEvent();
        
        //Returns the key event type
        KeyEventType GetKeyEventType();
        
        //Returns the keycode of the key pressed, to handle keycodes
        //(on multiple platforms) properly look in the KeyCodes.h
        KeyCode GetKeyCode();
        
        //Returns the duration of the Key event (from the KeyDown event)
        double GetDuration();
        
    protected:
        void LogEvent();
      
    private:
        KeyEventType m_KeyEventType;
        KeyCode m_KeyCode;
        double m_Duration;
    };
    
    //Conveniance method to display the names for each key event type
    const char* KeyEventTypeName(KeyEventType keyEventType);
}

#endif /* defined(__GameDev2D__KeyEvent__) */
