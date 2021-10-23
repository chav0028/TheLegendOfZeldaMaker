//
//  Rect.h
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-16.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Frame2D__
#define __GameDev2D__Frame2D__

#include "Polygon.h"


using namespace glm;

namespace GameDev2D
{
    //The Rect class inherits from the Polygon class, it sets up the
    //Polygon's vertices based on the position and size of the Rect
    class Rect : public Polygon
    {
    public:
        //Creates a Rect at (0,0) with a width and height of zero
        Rect();
        
        //Creates a Rect at a specified position with a specified size
        Rect(float x, float y, float width, float height);
        Rect(vec2 position, vec2 size);
        
        //Updates the rect size tweens
        void Update(double delta);
                
        //Sets the size (width and height) for the rect, can be set to animate over a duration,
        //an easing function can be applied, can be revered and repeated
        void SetWidth(float width, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetHeight(float height, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetSize(float width, float height, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetSize(vec2 size, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Getter methods for the size of the rect
        float GetWidth();
        float GetHeight();
        vec2 GetSize();

    private:
        //Resets the polygon data for a Rect, inherited from Polygon
        void ResetPolygonData();
    
        //Member variables
        vec2 m_Size;
        Tween m_TweenWidth;
        Tween m_TweenHeight;
    };
}

#endif /* defined(__GameDev2D__GDFrame2D__) */
