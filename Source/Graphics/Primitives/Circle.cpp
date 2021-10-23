//
//  Circle.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-18.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Circle.h"
#include "../../Services/Graphics/Graphics.h"


namespace GameDev2D
{
    Circle::Circle() : Polygon("Circle"),
        m_Radius(0.0f),
        m_LineSegments(DEFAULT_LINE_SEGMENTS),
        m_TweenRadius()
    {
        SetLocalPosition(0.0f, 0.0f);
        SetAnchorPoint(0.5f, 0.5f);
        ResetPolygonData();
        ModelMatrixIsDirty();
    }
    
    Circle::Circle(float aX, float aY, float aRadius) : Polygon("Circle"),
        m_Radius(aRadius),
        m_LineSegments(DEFAULT_LINE_SEGMENTS),
        m_TweenRadius()
    {
        SetLocalPosition(aX, aY);
        SetAnchorPoint(0.5f, 0.5f);
        ResetPolygonData();
        ModelMatrixIsDirty();
    }
    
    Circle::Circle(vec2 aPosition, float aRadius) : Polygon("Circle"),
        m_Radius(aRadius),
        m_LineSegments(DEFAULT_LINE_SEGMENTS),
        m_TweenRadius()
    {
        SetLocalPosition(aPosition);
        SetAnchorPoint(0.5f, 0.5f);
        ResetPolygonData();
        ModelMatrixIsDirty();
    }
    
    void Circle::Update(double aDelta)
    {
        //Update the size tweens
        UpdateTween(aDelta, &m_TweenRadius, static_cast<GameObject::TweenSetMethod>(&Circle::SetRadius));
    
        //Call the Polygon's Update() method
        Polygon::Update(aDelta);
    }
    
    void Circle::SetRadius(float aRadius, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aDuration == 0.0)
        {
            m_Radius = fmaxf(aRadius, 0.0f);
            ResetPolygonData();
            ModelMatrixIsDirty();
        }
        else
        {
            m_TweenRadius.Set(m_Radius, aRadius, aDuration, aEasingFunction, aReverse, aRepeatCount);
        }
    }
    
    float Circle::GetRadius()
    {
        return m_Radius;
    }
    
    void Circle::SetLineSegments(unsigned int aLineSegments)
    {
        m_LineSegments = aLineSegments;
        ResetPolygonData();
    }
    
    unsigned int Circle::GetLineSegments()
    {
        return m_LineSegments;
    }
    
    void Circle::ResetPolygonData()
    {
        //Safety check the radius
        if(GetRadius() == 0.0f)
        {
            return;
        }
        
        //Clear the vertices vector
        m_Vertices.clear();
        
        //Clear the colors vector
        m_Colors.clear();
    
        //Calculate the rotation amount
        float rotationAmount = (360.0f / (float)GetLineSegments());
        
        //Set the vertices
        vec2 vertex;
        for (float i = 0; i < 359.99f; i+= rotationAmount)
        {
            vertex.x = GetRadius() - (cosf(((float)M_PI * i / 180.0f)) * GetRadius());
            vertex.y = GetRadius() - (sinf(((float)M_PI * i / 180.0f)) * GetRadius());
            AddVertex(vertex);
        }
        
        //Set the render mode
        SetRenderMode(m_IsFilled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
        
        //Update the vertex buffer
        UpdateVertexBuffer();
    }
}