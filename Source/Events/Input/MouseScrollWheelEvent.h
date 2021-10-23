//
//  MouseScrollWheelEvent.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-22.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__MouseScrollWheelEvent__
#define __GameDev2D__MouseScrollWheelEvent__

#include "../Event.h"


namespace GameDev2D
{
    //The MouseScrollWheelEvent class is a subclass of the event class and is sent by the
    //InputManager whenever the user scrolls the wheel one of the mouse.
    class MouseScrollWheelEvent : public Event
    {
    public:
        MouseScrollWheelEvent(float delta);
        ~MouseScrollWheelEvent();
        
        //Returns how much the scroll wheel was scrolled
        float GetDetla();
        
    protected:
        void LogEvent();

    private:
        float m_Delta;
    };
}

#endif /* defined(__GameDev2D__MouseScrollWheelEvent__) */
