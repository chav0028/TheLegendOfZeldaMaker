//
//  TouchData.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-04.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__TouchData__
#define __GameDev2D__TouchData__

#include "../Core/BaseObject.h"
#include "../Animation/Timer.h"


using namespace std;
using namespace glm;

namespace GameDev2D
{
    //Constants
    const double FADE_IN_DURATION = 0.1;
    const double FADE_OUT_DURATION = 0.15;
    const float DEFAULT_TOUCH_RADIUS = 50.0f;
    const float FADE_IN_TOUCH_RADIUS = 100.0f;
    const float MAX_TOUCH_ALPHA = 1.0f;
    
    //The TouchData class holds all relevant touch data, this class is managed by
    //the InputManager. This class also handles debug drawing for the touch input.
    class TouchData : public BaseObject
    {
    public:
        TouchData(vec2 location, vec2 previous);
        ~TouchData();
        
        //Adds a touch location to the touches vector
        void AddTouchLocation(vec2 location, vec2 previous, bool ended = false);
        
        //Is the previous touch location contained in our touch data
        bool ContainsPrevious(vec2 previous);
        
        //Returns the duration of the touch (if at all)
        double GetDuration();
        
        //Returns if the touch has ended
        bool HasEnded();
        
        //Debug flag to erase the touch data from the scrren
        bool CanErase();
        
        //Methods to update and draw the touch data, the draw is specifially for debug drawing
        void Update(double delta);
        void Draw();
        
    private:
        //Member variables
        vector<pair<vec2, vec2>> m_TouchLocations;
        double m_Duration;
        Timer m_FadeInTimer;
        Timer m_FadeOutTimer;
        float m_TouchRadius;
        float m_TouchAlpha;
        bool m_HasEnded;
    };
}

#endif /* defined(__GameDev2D__TouchData__) */
