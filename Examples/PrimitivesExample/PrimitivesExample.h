//
//  PrimitivesExample.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-08-12.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__PrimitivesExample__
#define __GameDev2D__PrimitivesExample__

#include "../../Source/Core/Scene.h"
#include <vector>

using namespace std;

namespace GameDev2D
{
    class Circle;
    class Rect;
    class Line;
    class Point;
    class Polygon;

    class PrimitivesExample : public Scene
    {
    public:
        PrimitivesExample();
        ~PrimitivesExample();
        
        //Load all Game content in this method
        float LoadContent();

        //Handles input events
        void HandleEvent(Event* event);
        
        //Resets everything back to its original state
        void Reset();
        
    private:
        //Member variables
        Circle* m_Circle;
        Rect* m_Rect;
        Line* m_Line;
        Polygon* m_Polygon;
        vector<Point*> m_Points;
    };
}

#endif /* defined(__GameDev2D__PrimitivesExample__) */
