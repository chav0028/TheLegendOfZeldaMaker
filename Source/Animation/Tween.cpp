//
//  Tween.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-04-16.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Tween.h"
#include "Timer.h"


namespace GameDev2D
{
    Tween::Tween() : BaseObject("Tween"),
        m_EasingFunction(nullptr),
        m_Timer(),
        m_Start(0.0f),
        m_End(0.0f),
        m_Reverse(false),
        m_RepeatCount(0),
        m_Counter(0)
    {
    
    }
    
    Tween::Tween(float aStart, float aEnd, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount) : BaseObject("Tween"),
        m_EasingFunction(aEasingFunction),
        m_Timer(aDuration),
        m_Start(aStart),
        m_End(aEnd),
        m_Reverse(aReverse),
        m_RepeatCount(aRepeatCount),
        m_Counter(0)
    {
    
    }
    
    Tween::~Tween()
    {
    }
    
    void Tween::Update(double aDelta)
    {
        m_Timer.Update(aDelta);
        
        if(m_Timer.GetPercentageComplete() == 1.0f)
        {
            if(m_RepeatCount == -1)
            {
                if(m_Reverse == true)
                {
                    Reverse();
                }
                
                Restart();
            }
            else
            {
                if(m_Reverse == true)
                {
                    if(m_Counter == 0 || m_Counter <= (unsigned int)m_RepeatCount * 2)
                    {
                        Reverse();
                        Restart();
                    }
                }
                else
                {
                    if(m_Counter < (unsigned int)m_RepeatCount)
                    {
                        Restart();
                    }
                }
            }
        }
    }
    
    bool Tween::IsRunning()
    {
        return m_Timer.IsRunning();
    }
    
    void Tween::Set(float aStart, float aEnd, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        //Set the Tween properties
        SetStart(aStart);
        SetEnd(aEnd);
        SetDuration(aDuration);
        SetEasingFunction(aEasingFunction);
        SetReverse(aReverse);
        SetRepeatCount(aRepeatCount);
        m_Counter = 0;

        //Reset and start the timer
        m_Timer.Reset(true);
    }
    
    float Tween::GetCurrent()
    {
        if(m_EasingFunction != nullptr)
        {
            return m_Start + (m_End - m_Start) * m_EasingFunction(m_Timer.GetPercentageComplete());
        }
        else
        {
            return m_Start + (m_End - m_Start) * m_Timer.GetPercentageComplete();
        }
    }

    void Tween::SetStart(float aStart)
    {
        m_Start = aStart;
    }
    
    float Tween::GetStart()
    {
        return m_Start;
    }
    
    void Tween::SetEnd(float aEnd)
    {
        m_End = aEnd;
    }
    
    float Tween::GetEnd()
    {
        return m_End;
    }

    void Tween::SetDuration(double aDuration)
    {
        m_Timer.SetDuration(aDuration);
    }
    
    double Tween::GetDuration()
    {
        return m_Timer.GetDuration();
    }
    
    void Tween::SetReverse(bool aReverse)
    {
        m_Reverse = aReverse;
    }
    
    bool Tween::GetReverse()
    {
        return m_Reverse;
    }
        
    void Tween::SetRepeatCount(int aRepeatCount)
    {
        m_RepeatCount = aRepeatCount;
    }
    
    int Tween::GetRepeatCount()
    {
        return m_RepeatCount;
    }
    
    void Tween::SetEasingFunction(EasingFunction aEasingFunction)
    {
        m_EasingFunction = aEasingFunction;
    }
    
    EasingFunction Tween::GetEasingFunction()
    {
        return m_EasingFunction;
    }
    
    void Tween::Cancel()
    {
        m_Timer.Stop();
        m_Timer.Reset();
        
        SetStart(0.0f);
        SetEnd(0.0f);
        SetDuration(0.0);
        SetEasingFunction(nullptr);
        SetReverse(false);
        SetRepeatCount(0);
        m_Counter = 0;
    }
    
    void Tween::Reverse()
    {
        float start = GetStart();
        float end = GetEnd();
        SetStart(end);
        SetEnd(start);
    }
    
    void Tween::Restart()
    {
        m_Timer.Reset(true);
        m_Counter++;
    }
}
