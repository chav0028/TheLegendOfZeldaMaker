//
//  Circle.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-18.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Circle__
#define __GameDev2D__Circle__

#include "Polygon.h"


using namespace glm;

namespace GameDev2D
{
    //Circle Constants
    const unsigned int DEFAULT_LINE_SEGMENTS = 36;

    //The Circle class inherits from the Polygon class, it sets up the
    //Polygon's vertices based on the position and radius of the Circle
    class Circle : public Polygon
    {
    public:
        //Creates a Circle at (0,0) with a radius of zero
        Circle();
        
        //Creates a Circle at a specified position with a specified radius
        Circle(float x, float y, float radius);
        Circle(vec2 position, float radius);
        
        //Updates the circle radius tween
        void Update(double delta);
        
        //Sets the radius of circle, can be set to animate over a duration,
        //an easing function can be applied, can be revered and repeated
        void SetRadius(float radius, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);

        //Returns the radius of the circle
        float GetRadius();
        
        //Line segment getter and setter methods, represents
        //how many lines segments are used to draw the circle
        void SetLineSegments(unsigned int lineSegments);
        unsigned int GetLineSegments();
        
    private:
        //Resets the polygon data for a Circle, inherited from Polygon
        void ResetPolygonData();
    
        //Member variables
        float m_Radius;
        unsigned int m_LineSegments;
        Tween m_TweenRadius;
    };
}

#endif /* defined(__GameDev2D__Circle__) */
