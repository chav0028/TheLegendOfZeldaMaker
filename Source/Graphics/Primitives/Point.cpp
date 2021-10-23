//
//  Point.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-16.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "Point.h"
#include "../../Services/Graphics/Graphics.h"


namespace GameDev2D
{
    Point::Point() : Polygon("Point")
    {
        SetLocalPosition(vec2(0.0f, 0.0f));
        ResetPolygonData();
    }
    
    Point::Point(float aX, float aY) : Polygon("Point")
    {
        SetLocalPosition(vec2(aX, aY));
        ResetPolygonData();
    }
    
    Point::Point(vec2 aPosition) : Polygon("Point")
    {
        SetLocalPosition(aPosition);
        ResetPolygonData();
    }
    
    void Point::ResetPolygonData()
    {
        //Clear the vertices vector
        m_Vertices.clear();
        
        //Clear the colors vector
        m_Colors.clear();
    
        //Add the vertices
        AddVertex(vec2(0.0f, 0.0f));
        
        //Set the render mode
        SetRenderMode(GL_POINTS);
        
        //Update the vertex buffer
        UpdateVertexBuffer();
    }
}