//
//  b2DebugDraw.cpp
//  GameDevFramework
//
//  Created by Bradley Flood on 2012-10-03.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "b2DebugDraw.h"
#include "../../Graphics/Primitives/Primitives.h"
#include "../../Graphics/OpenGL.h"

using namespace glm;

namespace GameDev2D
{
    b2DebugDraw::b2DebugDraw(float aRatio)
	: m_Ratio(aRatio)
    {
        
    }
    
    void b2DebugDraw::DrawPolygon(const b2Vec2* aVertices, int aVertexCount, const b2Color& aColor)
    {
        Polygon polygon;
        
        vec2 vertex;
        for(int i = 0; i < aVertexCount; i++)
        {
            vertex.x = aVertices[i].x * m_Ratio;
            vertex.y = aVertices[i].y * m_Ratio;
            polygon.AddVertex(vertex);
        }
        
        polygon.SetColor(Color(aColor.r, aColor.g, aColor.b));
        polygon.SetRenderMode(GL_LINE_LOOP);
        polygon.Draw();
    }
    
    void b2DebugDraw::DrawSolidPolygon(const b2Vec2* aVertices, int aVertexCount, const b2Color& aColor)
    {
        Polygon polygon;
        
        vec2 vertex;
        for(int i = 0; i < aVertexCount; i++)
        {
            vertex.x = aVertices[i].x * m_Ratio;
            vertex.y = aVertices[i].y * m_Ratio;
            polygon.AddVertex(vertex);
        }
        
        polygon.SetColor(Color(aColor.r * 0.5f, aColor.g * 0.5f, aColor.b * 0.5f));
        polygon.SetRenderMode(GL_TRIANGLE_FAN);
        polygon.Draw();
    
        polygon.SetColor(Color(aColor.r, aColor.g, aColor.b));
        polygon.SetRenderMode(GL_LINE_LOOP);
        polygon.Draw();
    }
    
    void b2DebugDraw::DrawCircle(const b2Vec2& aCenter, float aRadius, const b2Color& aColor)
    {
        Circle circle(aCenter.x * m_Ratio, aCenter.y * m_Ratio, aRadius * m_Ratio);
        circle.SetColor(Color(aColor.r * 0.5f, aColor.g * 0.5f, aColor.b * 0.5f));
        circle.SetIsFilled(false);
        circle.Draw();
    }
    
    void b2DebugDraw::DrawSolidCircle(const b2Vec2& aCenter, float aRadius, const b2Vec2& aAxis, const b2Color& aColor)
    {
        Circle circle(aCenter.x * m_Ratio, aCenter.y * m_Ratio, aRadius * m_Ratio);
        circle.SetColor(Color(aColor.r * 0.5f, aColor.g * 0.5f, aColor.b * 0.5f, 0.5f));
        circle.SetIsFilled(true);
        circle.Draw();
        
        circle.SetColor(Color(aColor.r, aColor.g, aColor.b));
        circle.SetIsFilled(false);
        circle.Draw();
        
        DrawSegment(aCenter,aCenter + aRadius * aAxis, aColor);
    }
    
    void b2DebugDraw::DrawSegment(const b2Vec2& aP1, const b2Vec2& aP2, const b2Color& aColor)
    {
        Line line(aP1.x * m_Ratio, aP1.y * m_Ratio, aP2.x * m_Ratio, aP2.y * m_Ratio);
        line.SetColor(Color(aColor.r, aColor.g, aColor.b));
        line.Draw();
    }
    
    void b2DebugDraw::DrawTransform(const b2Transform& xf)
    {
        b2Vec2 p1 = xf.p, p2;
        const float k_axisScale = 0.4f;
        
        p2 = p1 + k_axisScale * xf.q.GetXAxis();
        DrawSegment(p1,p2,b2Color(1,0,0));
        
        p2 = p1 + k_axisScale * xf.q.GetYAxis();
        DrawSegment(p1,p2,b2Color(0,1,0));
    }
}
