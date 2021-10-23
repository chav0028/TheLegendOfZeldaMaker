//
//  Timer.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-04-03.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Timer.h"
#include "../Events/Event.h"


namespace GameDev2D
{
    Timer::Timer(double aDuration) : BaseObject("Timer"),
        m_UpdateEvent(nullptr),
        m_Elapsed(0.0),
        m_Duration(aDuration),
        m_IsRunning(false)
    {
        //We cache this one because it is called frequently.
        m_UpdateEvent = new Event(TIMER_UPDATE_EVENT, this, VerbosityLevel_Animation);
    }
    
    Timer::~Timer()
    {
        SafeDelete(m_UpdateEvent);
    }
    
    void Timer::Update(double aDelta)
    {
        if(IsRunning() == true)
        {
            m_Elapsed += aDelta;
            if(m_Elapsed >= m_Duration)
            {
                m_Elapsed = m_Duration;
                m_IsRunning = false;
                
                if(m_Duration > 0.0)
                {
                    DispatchEvent(new Event(TIMER_DID_FINISH_EVENT, this, VerbosityLevel_Animation));
                }
            }
            else
            {
                DispatchEvent(m_UpdateEvent, false);
            }
        }
    }
    
    void Timer::Start()
    {
        if(m_IsRunning == false)
        {
            if(m_Duration == 0.0)
            {
                DispatchEvent(new Event(TIMER_DID_FINISH_EVENT, this, VerbosityLevel_Animation));
                m_IsRunning = false;
            }
            else
            {
                DispatchEvent(new Event(TIMER_DID_START_EVENT, this, VerbosityLevel_Animation));
                m_IsRunning = true;
            }
        }
        else
        {
            Log(VerbosityLevel_Animation, "Attempting to start a timer: %p that is already running", this);
        }
    }
    
    void Timer::Stop()
    {
        if(m_IsRunning == true)
        {
            DispatchEvent(new Event(TIMER_DID_STOP_EVENT, this, VerbosityLevel_Animation));
        }
    
        m_IsRunning = false;
    }
    
    void Timer::Reset(bool aStart)
    {
        m_Elapsed = 0.0;
        
        if(aStart == true)
        {
            Stop();
            Start();
        }
    }
    
    bool Timer::IsRunning()
    {
        return m_IsRunning;
    }
    
    void Timer::SetDuration(double aDuration)
    {
        m_Duration = aDuration;
    }
    
    double Timer::GetDuration()
    {
        return m_Duration;
    }
    
    double Timer::GetElapsedTime()
    {
        return m_Elapsed;
    }
    
    double Timer::GetRemainingTime()
    {
        return GetDuration() - GetElapsedTime();
    }
    
    float Timer::GetPercentageComplete()
    {
        if(GetDuration() > 0.0)
        {
            return (float)(GetElapsedTime() / GetDuration());
        }
        return 0.0f;
    }
}