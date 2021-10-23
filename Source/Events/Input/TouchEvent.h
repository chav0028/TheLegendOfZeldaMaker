//
//  TouchEvent.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__TouchEvent__
#define __GameDev2D__TouchEvent__

#include "../Event.h"


using namespace glm;

namespace GameDev2D
{
    //There are 4 types of touch events,
    //the enum below describes them.
    enum TouchType
    {
        TouchBegan,
        TouchMoved,
        TouchEnded,
        TouchCancelled
    };
    
    //The TouchEvent class is a subclass of the event class and is sent by the
    //InputManager when a new touch is recieved.
    class TouchEvent : public Event
    {
    public:
        TouchEvent(TouchType touchType, unsigned int index, vec2 touchLocation, vec2 previousLocation, double duration);
        ~TouchEvent();
        
        //Returns the type of touch event
        TouchType GetTouchType();
        
        //Returns the index of the finger that is touching (first finger, second finget, etc)
        unsigned int GetIndex();
        
        //Returns the current location the finger is touching
        vec2 GetTouchLocation();
        
        //Returns the previous location the finger was touching
        vec2 GetPreviousLocation();
        
        //Returns the duration of the touch event (from the start of the touchBegan event)
        double GetDuration();
        
    protected:
        void LogEvent();
      
    private:
        TouchType m_TouchType;
        unsigned int m_Index;
        vec2 m_TouchLocation;
        vec2 m_PreviousLocation;
        double m_Duration;
    };
    
    //Conveniance method to display the names for each touch type
    const char* TouchTypeName(TouchType touchType);
}

#endif /* defined(__GameDev2D__TouchEvent__) */
