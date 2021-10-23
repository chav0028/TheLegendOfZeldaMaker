//
//  Timer.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-04-03.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Timer__
#define __GameDev2D__Timer__

#include "FrameworkConfig.h"
#include "../Core/BaseObject.h"
#include "../Events/EventDispatcher.h"


namespace GameDev2D
{
    //The Timer class has all the functionality to help
    //set up accurate timing events in game.
    //This class dispatches the following events:
    // TIMER_UPDATE_EVENT
    // TIMER_DID_START_EVENT
    // TIMER_DID_STOP_EVENT
    // TIMER_DID_FINISH_EVENT
    //The EventData* contains a pointer to the Timer* that disptached the events
    class Timer : public BaseObject, public EventDispatcher
    {
    public:
        //Constructor for the Timer class, a duration can be set
        //optionally at construction timer OR later
        Timer(double duration = 0.0);
        ~Timer();
        
        //Updates the timer, must be called to properly update the timer,
        //if your timer doesn't run, check if Update() is called
        void Update(double delta);
        
        //Starts the timer, it may be from the start, or if Stop() was
        //call, from where it left off.
        void Start();
        
        //Stops the timer at the current position, can be resumed from
        //that spot using the Start() method
        void Stop();
        
        //Resets the timer back to zero, optionally it can also start
        // the timer on reset, by default it wont though
        void Reset(bool start = false);
        
        //Returns wether or not the timer is currently running
        bool IsRunning();
        
        //Sets the duration of the timer (in seconds)
        void SetDuration(double duration);
        
        //Returns the duration of the timer (in seconds)
        double GetDuration();
        
        //Returns the elapsed time (in seconds)
        double GetElapsedTime();
        
        //Returns the time remaining (in seconds)
        double GetRemainingTime();
        
        //Returns the percentage completed by the timer
        float GetPercentageComplete();
        
    private:
        //Member variables
        Event* m_UpdateEvent; 
        double m_Elapsed;
        double m_Duration;
        bool m_IsRunning;
    };
}

#endif /* defined(__GameDev2D__Timer__) */
