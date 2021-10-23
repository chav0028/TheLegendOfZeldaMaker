//
//  Tween.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-04-16.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Tween__
#define __GameDev2D__Tween__

#include "FrameworkConfig.h"
#include "../Core/BaseObject.h"
#include "Timer.h"
#include "Easing.h"


using namespace glm;

namespace GameDev2D
{
    //The Tween class handles animating from one float value to another for a given duration.
    //Various easing functions can be applied, the tweens can also be reversed and repeated.
    class Tween : public BaseObject
    {
    public:
        //Tween's one value to another value for a given duration, easing function. 
        //Can be reversed and repeated. Repeat count of -1 means it will repeat forever.
        Tween(float start, float end, double duration, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        Tween();
        ~Tween();
        
        //Updates the tween, must be called to properly update the timer,
        //if your tween doesn't run, check if Update() is called
        void Update(double delta);
    
        //Returns wether the Tween is currently running
        bool IsRunning();
        
        //Sets the Tween's intiail value, end value, duration, easing function. 
        //Can be reversed and repeated. Repeat count of -1 means it will repeat forever.
        void Set(float start, float end, double duration, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
    
        //Returns the current value
        float GetCurrent();
    
        //Start value getter and setter methods
        void SetStart(float start);
        float GetStart();
        
        //End value getter and setter methods
        void SetEnd(float end);
        float GetEnd();
        
        //Duration value getter and setter methods
        void SetDuration(double duration);
        double GetDuration();
        
        //Reversing getter and setter methods
        void SetReverse(bool reverse);
        bool GetReverse();
        
        //Repeating getter and setter methods
        void SetRepeatCount(int repeat);
        int GetRepeatCount();
        
        //Easing value getter and setter methods
        void SetEasingFunction(EasingFunction easingFunction);
        EasingFunction GetEasingFunction();
        
        //Cancels the currently running tween and resets everything back to zero
        void Cancel();
    
    private:
        //Conveniance methods to restart and reverse the Tween
        void Reverse();
        void Restart();
    
        //Member variables
        EasingFunction m_EasingFunction;
        Timer m_Timer;
        float m_Start;
        float m_End;
        bool m_Reverse;
        int m_RepeatCount;
        unsigned int m_Counter;
    };
}

#endif /* defined(__GameDev2D__Tween__) */
