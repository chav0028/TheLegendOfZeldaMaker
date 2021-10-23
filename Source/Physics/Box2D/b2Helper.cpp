//
//  b2Helper.cpp
//  GameDevFramework
//
//  Created by Bradley Flood on 2012-10-04.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "b2Helper.h"


namespace GameDev2D
{
    //Initialize the static vars
    float b2Helper::s_PixelsToMetersRatio = 1.0f;
    float b2Helper::s_Scale = 1.0f;
    
    float b2Helper::PixelsToMeters(float aValue)
    {
        return aValue / b2Helper::Ratio();
    }
    
    b2Vec2 b2Helper::PixelsToMeters(float aX, float aY)
    {
        return b2Vec2(b2Helper::PixelsToMeters(aX), b2Helper::PixelsToMeters(aY));
    }
    
    b2Vec2 b2Helper::PixelsToMeters(b2Vec2 aValue)
    {
        return b2Vec2(b2Helper::PixelsToMeters(aValue.x), b2Helper::PixelsToMeters(aValue.y));
    }
    
    float b2Helper::MetersToPixels(float aValue)
    {
        return aValue * b2Helper::Ratio();
    }
    
    b2Vec2 b2Helper::MetersToPixels(float aX, float aY)
    {
        return b2Vec2(b2Helper::MetersToPixels(aX), b2Helper::MetersToPixels(aY));
    }
    
    b2Vec2 b2Helper::MetersToPixels(b2Vec2 aValue)
    {
        return b2Vec2(b2Helper::MetersToPixels(aValue.x), b2Helper::MetersToPixels(aValue.y));
    }
    
    void b2Helper::SetPixelsToMetersRatio(float aPixelsToMetersRatio)
    {
        s_PixelsToMetersRatio = aPixelsToMetersRatio;
    }
    
    float b2Helper::Ratio()
    {
        return s_PixelsToMetersRatio * s_Scale;
    }
}
