//
//  Line.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-18.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Line.h"
#include "../../Services/Graphics/Graphics.h"


namespace GameDev2D
{
    Line::Line() : Polygon("Line"),
        m_PositionB(0.0f, 0.0f),
        m_PositionB_X(),
        m_PositionB_Y()
    {
        SetLocalPosition(vec2(0.0f, 0.0f));
        ResetPolygonData();
        ModelMatrixIsDirty();
    }

    Line::Line(float aXA, float aYA, float aXB, float aYB) : Polygon("Line"),
        m_PositionB(aXB, aYB),
        m_PositionB_X(),
        m_PositionB_Y()
    {
        SetLocalPosition(vec2(aXA, aYA));
        ResetPolygonData();
        ModelMatrixIsDirty();
    }

    Line::Line(vec2 aPositionA, vec2 aPositionB) : Polygon("Line"),
        m_PositionB(aPositionB),
        m_PositionB_X(),
        m_PositionB_Y()
    {
        SetLocalPosition(aPositionA);
        ResetPolygonData();
        ModelMatrixIsDirty();
    }

    void Line::Update(double aDelta)
    {
        //Update the position B's position tweens
        UpdateTween(aDelta, &m_PositionB_X, static_cast<GameObject::TweenSetMethod>(&Line::SetPositionB_X));
        UpdateTween(aDelta, &m_PositionB_Y, static_cast<GameObject::TweenSetMethod>(&Line::SetPositionB_Y));
    
        //Call the Polygon's Update() method
        Polygon::Update(aDelta);
    }
    
    void Line::SetLocalX(float aX, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        GameObject::SetLocalX(aX, aDuration, aEasingFunction, aReverse, aRepeatCount);
        
        if(aDuration == 0.0)
        {
            ResetPolygonData();
            ModelMatrixIsDirty();
        }
    }
    
    void Line::SetLocalY(float aY, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        GameObject::SetLocalY(aY, aDuration, aEasingFunction, aReverse, aRepeatCount);
        
        if(aDuration == 0.0)
        {
            ResetPolygonData();
            ModelMatrixIsDirty();
        }
    }
    
    void Line::SetPositionB_X(float aX, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aDuration == 0.0)
        {
            m_PositionB.x = aX;
            ResetPolygonData();
            ModelMatrixIsDirty();
        }
        else
        {
            m_PositionB_X.Set(m_PositionB.x, aX, aDuration, aEasingFunction, aReverse, aRepeatCount);
        }
    }
    
    void Line::SetPositionB_Y(float aY, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aDuration == 0.0)
        {
            m_PositionB.y = aY;
            ResetPolygonData();
            ModelMatrixIsDirty();
        }
        else
        {
            m_PositionB_Y.Set(m_PositionB.y, aY, aDuration, aEasingFunction, aReverse, aRepeatCount);
        }
    }
    
    void Line::SetPositionB(float aX, float aY, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        SetPositionB_X(aX, aDuration, aEasingFunction, aReverse, aRepeatCount);
        SetPositionB_Y(aY, aDuration, aEasingFunction, aReverse, aRepeatCount);
    }
    
    void Line::SetPositionB(vec2 aPosition, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        SetPositionB(aPosition.x, aPosition.y, aDuration, aEasingFunction, aReverse, aRepeatCount);
    }

    vec2 Line::GetPositionB()
    {
        return m_PositionB;
    }

    void Line::ResetPolygonData()
    {
        //Clear the vertices vector
        m_Vertices.clear();
        
        //Clear the colors vector
        m_Colors.clear();
    
        //Add the vertices
        AddVertex(vec2(0.0f, 0.0f));
        AddVertex(vec2(m_PositionB.x - GetWorldPosition().x, m_PositionB.y - GetWorldPosition().y));
        
        //Set the render mode
        SetRenderMode(GL_LINES);
        
        //Update the vertex buffer
        UpdateVertexBuffer();
    }
}