//
//  Event.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-07.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Event__
#define __GameDev2D__Event__

#include "../Core/BaseObject.h"
#include <string>


using namespace std;

namespace GameDev2D
{
    //Forward declaration
    class EventDispatcher;
    
    //Platform events
    const unsigned int UNKNOWN_EVENT = 0;
    const unsigned int SHUTDOWN_EVENT = 1;
    const unsigned int SUSPEND_EVENT = 2;
    const unsigned int RESUME_EVENT = 3;
    const unsigned int LOW_MEMORY_WARNING_EVENT = 4;    //iOS Only
    const unsigned int ORIENTATION_CHANGED_EVENT = 5;
    const unsigned int RESIZE_EVENT = 6;
    const unsigned int FULLSCREEN_EVENT = 7;
    
    //Sprite events
    const unsigned int SPRITE_DID_FINISH_ANIMATING = 25;
    
    //Button events
    const unsigned int BUTTON_ACTION = 26;
    const unsigned int BUTTON_STATE_CHANGED = 27;
    
    //Timer events
    const unsigned int TIMER_UPDATE_EVENT = 28;
    const unsigned int TIMER_DID_START_EVENT = 29;
    const unsigned int TIMER_DID_STOP_EVENT = 30;
    const unsigned int TIMER_DID_FINISH_EVENT = 31;
    
    //Audio events
    const unsigned int AUDIO_PLAY_EVENT = 32;
    const unsigned int AUDIO_PAUSE_EVENT = 33;
    const unsigned int AUDIO_STOP_EVENT = 34;
    const unsigned int AUDIO_PLAYBACK_FINISHED_EVENT = 35;
    const unsigned int AUDIO_CHANNEL_INVALIDATED_EVENT = 36;
    
    //Key event
    const unsigned int KEYBOARD_EVENT = 40;
    
    //Mouse events
    const unsigned int MOUSE_MOVEMENT_EVENT = 42;
    const unsigned int MOUSE_CLICK_EVENT = 43;
    const unsigned int MOUSE_WHEEL_EVENT = 44;
    
    //Controller events
    const unsigned int CONTROLLER_PLUGGED_IN = 50;
    const unsigned int CONTROLLER_UNPLUGGED = 51;
    const unsigned int CONTROLLER_BUTTON_EVENT = 52;
    const unsigned int CONTROLLER_ANALOG_EVENT = 53;
    const unsigned int CONTROLLER_ANALOG_STICK_EVENT = 54;
    
    //Touch events
    const unsigned int TOUCH_EVENT = 64;
    const unsigned int MULTI_TOUCH_ENABLED_EVENT = 65;
    const unsigned int MULTI_TOUCH_DISABLED_EVENT = 66;
    
    //Accelerometer events
    const unsigned int ACCELEROMETER_EVENT = 67;
    const unsigned int ACCELEROMETER_ENABLED_EVENT = 68;
    const unsigned int ACCELEROMETER_DISABLED_EVENT = 69;
    
    //Gyroscope events
    const unsigned int GYROSCOPE_EVENT = 70;
    const unsigned int GYROSCOPE_ENABLED_EVENT = 71;
    const unsigned int GYROSCOPE_DISABLED_EVENT = 72;


    //The Event class, can be used as is to send events (like platform events) or can be
    //overridden to add additonal functionality (like the input events). It is the main
    //interface for events that are sent by the EventDispatcher.
    class Event : public BaseObject
    {
    public:
        //The constructor takes in the eventCode, as well as optional eventData,
        //which can be any type of pointer.
        Event(int eventCode, void* eventData = nullptr, LogVerbosity verbosityLevel = VerbosityLevel_Events);
        Event(int eventCode, const string& eventDescription, void* eventData = nullptr, LogVerbosity verbosityLevel = VerbosityLevel_Events);
        virtual ~Event();
        
        //Returns the event code of the event
        unsigned int GetEventCode();
        
        //Returns a description of the event code associated with the event
        const char* GetEventDescription();
        
        //Returns the event data (if any), can be any type of pointer
        void* GetEventData();
        
        //Returns the EventDispatcher
        EventDispatcher* GetDispatcher() const;
        
    protected:
        //This constructor is to be used by inheriting classes to set their event type
        Event(const string& type);
    
        //Can be called by inheriting classes to set the event code. Can be overridden.
        virtual void SetEventCode(unsigned int eventCode);
        
        //Can be called by inheriting classes to set the event data. Can be overridden.
        virtual void SetEventData(void* eventData);
        
        //Called right before the event is dispatched, it logs an description of the event that is triggered, Can be overridden.
        virtual void LogEvent();
        
        //Called by the EventDispatcher to set the dispatcher
        void SetDispatcher(EventDispatcher* dispatcher);
    
        //The EventDispatcher is a friend of Event and calls the SetDispatcher() method
        friend class EventDispatcher;
    
    private:
        //Member variables
        unsigned int m_EventCode;
        string m_EventDescription;
        void* m_EventData;
        EventDispatcher* m_Dispatcher;
        LogVerbosity m_VerbosityLevel;
    };
    
    //Returns a string for the event code
    const char* EventTypeForCode(int eventCode);
}

#endif /* defined(__GameDev2D__Event__) */
