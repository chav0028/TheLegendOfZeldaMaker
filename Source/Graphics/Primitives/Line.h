//
//  Line.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-18.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Line__
#define __GameDev2D__Line__

#include "Polygon.h"


using namespace glm;

namespace GameDev2D
{
    //The Line class inherits from the Polygon class, it sets up the
    //Polygon's vertices based on the positions of a Line
    class Line : public Polygon
    {
    public:
        //Creates a Line with both ends of the segment at (0,0)
        Line();
        
        //Creates a Line with end points at position A and position B
        Line(float xA, float yA, float xB, float yB);
        Line(vec2 positionA, vec2 positionB);
        
        //Updates the Line's B position tween
        void Update(double delta);
        
        //Overrides the methods from GameObject to handle line animation
        void SetLocalX(float x, double duration = 0.0, EasingFunction easing = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetLocalY(float y, double duration = 0.0, EasingFunction easing = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Sets the Line's B position, can be set to animate over a duration,
        //an easing function can be applied, can be revered and repeated
        void SetPositionB_X(float x, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetPositionB_Y(float y, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetPositionB(float x, float y, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetPositionB(vec2 position, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Returns the Line's position B position
        vec2 GetPositionB();
        
    private:
        //Resets the polygon data for a Circle, inherited from Polygon
        void ResetPolygonData();
        
        //Member variables
        vec2 m_PositionB;
        Tween m_PositionB_X;
        Tween m_PositionB_Y;
    };
}

#endif /* defined(__GameDev2D__Line__) */
