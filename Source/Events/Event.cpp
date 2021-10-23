//
//  Event.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-07.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//


#include "Event.h"


namespace GameDev2D
{
	Event::Event(const string& aType) : BaseObject(aType),
        m_EventCode(0),
        m_EventDescription(EventTypeForCode(GetEventCode())),
        m_EventData(nullptr),
        m_Dispatcher(nullptr),
        m_VerbosityLevel(VerbosityLevel_Events)
    {

	}
	
	Event::Event(int aEventCode, void* aEventData, LogVerbosity aVerbosityLevel) : BaseObject("Event"),
        m_EventCode(aEventCode),
        m_EventDescription(EventTypeForCode(GetEventCode())),
        m_EventData(aEventData),
        m_Dispatcher(nullptr),
        m_VerbosityLevel(aVerbosityLevel)
    {        

	}
    
    Event::Event(int aEventCode, const string& aEventDescription, void* aEventData, LogVerbosity aVerbosityLevel) : BaseObject("Event"),
        m_EventCode(aEventCode),
        m_EventDescription(aEventDescription),
        m_EventData(aEventData),
        m_Dispatcher(nullptr),
        m_VerbosityLevel(aVerbosityLevel)
    {        

	}
	
	Event::~Event()
    {
		
	}
    
    unsigned int Event::GetEventCode()
    {
		return m_EventCode;
	}
	
	const char* Event::GetEventDescription()
    {
        return m_EventDescription.c_str();
	}
    
    void* Event::GetEventData()
    {
        return m_EventData;
    }
	
	EventDispatcher* Event::GetDispatcher() const
    {
		return m_Dispatcher;
	}
    
    void Event::SetEventCode(unsigned int aEventCode)
    {
        m_EventCode = aEventCode;
    }
    
    void Event::SetEventData(void* aEventData)
    {
        m_EventData = aEventData;
    }
    
    void Event::LogEvent()
    {
        Log(m_VerbosityLevel, "%s", GetEventDescription());
    }
	
	void Event::SetDispatcher(EventDispatcher* aDispatcher)
    {
		m_Dispatcher = aDispatcher;
	}
    
    const char* EventTypeForCode(int aEventCode)
    {
        switch (aEventCode)
        {
            case SHUTDOWN_EVENT:
                return "Shutdown";
                
            case SUSPEND_EVENT:
                return "Suspend";
                
            case RESUME_EVENT:
                return "Resume";
                
            case LOW_MEMORY_WARNING_EVENT:
                return "Low Memory";
                
            case ORIENTATION_CHANGED_EVENT:
                return "Orientation Changed";
                
            case RESIZE_EVENT:
                return "Resize Changed";
                
            case FULLSCREEN_EVENT:
                return "Fullscreen";
             
            case SPRITE_DID_FINISH_ANIMATING:
                return "Sprite did finish animating";
                
            case BUTTON_ACTION:
                return "Button action";
                
            case BUTTON_STATE_CHANGED:
                return "Button state changed";

            case TIMER_UPDATE_EVENT:
                return "Timer updated";
              
            case TIMER_DID_START_EVENT:
                return "Timer did start";
              
            case TIMER_DID_STOP_EVENT:
                return "Timer did stop";
              
            case TIMER_DID_FINISH_EVENT:
                return "Timer did finish";
              
            case AUDIO_PLAY_EVENT:
                return "Audio play";
              
            case AUDIO_PAUSE_EVENT:
                return "Audio paused";
                
            case AUDIO_STOP_EVENT:
                return "Audio stopped";
                
            case AUDIO_PLAYBACK_FINISHED_EVENT:
                return "Audio playback finished";
                
            case AUDIO_CHANNEL_INVALIDATED_EVENT:
                return "Audio channel invalidated";
                
            case KEYBOARD_EVENT:
                return "Keyboard";
                
            case MOUSE_MOVEMENT_EVENT:
                return "Mouse movement";
                
            case MOUSE_CLICK_EVENT:
                return "Mouse click";
            
            case MOUSE_WHEEL_EVENT:
                return "Mouse wheel";
                
            case CONTROLLER_PLUGGED_IN:
                return "Controller Plugged In";
                
            case CONTROLLER_UNPLUGGED:
                return "Controller Unplugged";
             
            case CONTROLLER_BUTTON_EVENT:
                return "Controller button";
                
            case CONTROLLER_ANALOG_EVENT:
                return "Controller analog";
                
            case CONTROLLER_ANALOG_STICK_EVENT:
                return "Controller analog stick";

            case TOUCH_EVENT:
                return "Touch";
             
            case MULTI_TOUCH_ENABLED_EVENT:
                return "Multi-touch enabled";
                
            case MULTI_TOUCH_DISABLED_EVENT:
                return "Multi-touch disabled";
             
            case ACCELEROMETER_EVENT:
                return "Accelerometer";
             
            case ACCELEROMETER_ENABLED_EVENT:
                return "Accelerometer enabled";
                
            case ACCELEROMETER_DISABLED_EVENT:
                return "Accelerometer disabled";
             
            case GYROSCOPE_EVENT:
                return "Gryoscope";
             
            case GYROSCOPE_ENABLED_EVENT:
                return "Gyroscope enabled";
                
            case GYROSCOPE_DISABLED_EVENT:
                return "Gyroscope disabled";

            case UNKNOWN_EVENT:
            default:
                return "Unknown";
        }
    }
}