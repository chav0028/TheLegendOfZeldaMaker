//
//  Rect.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-16.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "Rect.h"
#include "../../Services/ServiceLocator.h"
#include "../../Services/Graphics/Graphics.h"


namespace GameDev2D
{
    Rect::Rect() : Polygon("Rect"),
        m_Size(vec2(0.0f, 0.0f)),
        m_TweenWidth(),
        m_TweenHeight()
    {
        SetLocalPosition(vec2(0.0f, 0.0f));
        ResetPolygonData();
        ModelMatrixIsDirty();
    }
    
    Rect::Rect(float aX, float aY, float aWidth, float aHeight) : Polygon("Rect"),
        m_Size(vec2(aWidth, aHeight)),
        m_TweenWidth(),
        m_TweenHeight()
    {
        SetLocalPosition(vec2(aX, aY));
        ResetPolygonData();
        ModelMatrixIsDirty();
    }
    
    Rect::Rect(vec2 aPosition, vec2 aSize) : Polygon("Rect"),
        m_Size(aSize),
        m_TweenWidth(),
        m_TweenHeight()
    {
        SetLocalPosition(aPosition);
        ResetPolygonData();
        ModelMatrixIsDirty();
    }
    
    void Rect::Update(double aDelta)
    {
        //Update the size tweens
        UpdateTween(aDelta, &m_TweenWidth, static_cast<GameObject::TweenSetMethod>(&Rect::SetWidth));
        UpdateTween(aDelta, &m_TweenHeight, static_cast<GameObject::TweenSetMethod>(&Rect::SetHeight));
    
        //Call the Polygon's Update() method
        Polygon::Update(aDelta);
    }
    
    void Rect::SetWidth(float aWidth, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aDuration == 0.0)
        {
            m_Size.x = fmaxf(aWidth, 0.0f);
            ResetPolygonData();
            ModelMatrixIsDirty();
        }
        else
        {
            m_TweenWidth.Set(m_Size.x, aWidth, aDuration, aEasingFunction, aReverse, aRepeatCount);
        }
    }
    
    void Rect::SetHeight(float aHeight, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aDuration == 0.0)
        {
            m_Size.y = fmaxf(aHeight, 0.0f);
            ResetPolygonData();
            ModelMatrixIsDirty();
        }
        else
        {
            m_TweenHeight.Set(m_Size.y, aHeight, aDuration, aEasingFunction, aReverse, aRepeatCount);
        }
    }
    
    void Rect::SetSize(float aWidth, float aHeight, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        SetWidth(aWidth, aDuration, aEasingFunction, aReverse, aRepeatCount);
        SetHeight(aHeight, aDuration, aEasingFunction, aReverse, aRepeatCount);
    }
    
    void Rect::SetSize(vec2 aSize, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        SetSize(aSize.x, aSize.y, aDuration, aEasingFunction, aReverse, aRepeatCount);
    }
    
    float Rect::GetWidth()
    {
        return m_Size.x;
    }
    
    float Rect::GetHeight()
    {
        return m_Size.y;
    }

    vec2 Rect::GetSize()
    {
        return m_Size;
    }
    
    void Rect::ResetPolygonData()
    {
        //Safety check the radius
        if(GetSize().x == 0.0f && GetSize().y == 0.0f)
        {
            return;
        }
        
        //Clear the vertices vector
        m_Vertices.clear();
        
        //Clear the colors vector
        m_Colors.clear();
    
        //Add the vertices
        AddVertex(vec2(0.0f, GetSize().y));
        AddVertex(vec2(GetSize().x, GetSize().y));
        AddVertex(vec2(GetSize().x, 0.0f));
        AddVertex(vec2(0.0f, 0.0f));

        //Set the render mode
        SetRenderMode(m_IsFilled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
        
        //Update the vertex buffer
        UpdateVertexBuffer();
    }
}
