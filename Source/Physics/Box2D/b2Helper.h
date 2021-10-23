//
//  b2Helper.h
//  GameDev2D
//
//  Created by Bradley Flood on 2012-10-04.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef GameDevFramework_b2Helper_h
#define GameDevFramework_b2Helper_h

#include "Box2D.h"


namespace GameDev2D
{
    class b2Helper
    {
    public:
        static float PixelsToMeters(float value);
        static b2Vec2 PixelsToMeters(float x, float y);
        static b2Vec2 PixelsToMeters(b2Vec2 value);
        
        static float MetersToPixels(float value);
        static b2Vec2 MetersToPixels(float x, float y);
        static b2Vec2 MetersToPixels(b2Vec2 value);
        
        static void SetPixelsToMetersRatio(float pixelsToMetersRatio);
        static void SetScale(float scale);
        
        static float Ratio();
        
    private:
        static float s_PixelsToMetersRatio;
        static float s_Scale;
    };
}

#endif
