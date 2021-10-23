//
//  Point.h
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-16.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Point__
#define __GameDev2D__Point__

#include "Polygon.h"


using namespace glm;

namespace GameDev2D
{
    //The Point class inherits from the Polygon class, it sets up the
    //Polygon's vertices based on the position of the Point
    class Point : public Polygon
    {
    public:
        //Creates a Point at (0,0)
        Point();
        
        //Creates a Point at a specified position
        Point(float x, float y);
        Point(vec2 position);
    
    private:
        //Resets the polygon data for a Point, inherited from Polygon
        void ResetPolygonData();
    };
}

#endif /* defined(__GameDev2D__Point__) */
