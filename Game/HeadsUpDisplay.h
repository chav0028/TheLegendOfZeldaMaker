//
//  HeadsUpDisplay.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-15.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__HeadsUpDisplay__
#define __GameDev2D__HeadsUpDisplay__

#include "../Source/Core/Scene.h"


using namespace glm;

namespace GameDev2D
{
    //Local Constants
    const float HUD_HEART_INCREMENT = 32.0f;
    const unsigned int HUD_NUMBER_OF_HEART_COLUMNS = 10;
    const vec2 HUD_HEARTS_STARTING_POSITION_PCT = vec2(0.665f, 0.88f);
    const vec2 HUD_LIFE_LABEL_POSITION_PCT = vec2(0.8075f, 0.97f);
    const unsigned int HUD_LIFE_LABEL_SIZE = 32;
    const vec2 HUD_SWORD_SPRITE_POSITION_PCT = vec2(0.47f, 0.925f);
    const vec2 HUD_SHIELD_SPRITE_POSITION_PCT = vec2(0.53f, 0.925f);
    const vec2 HUD_RUPEE_SPRITE_POSITION_PCT = vec2(0.325f, 0.87f);

    //Forward declarations
    class World;
    class Rect;
    class Sprite;
    class Label;

    //The Heads up display, displays the hero's health and invertory
    class HeadsUpDisplay : public Scene
    {
    public:
        HeadsUpDisplay(World* world);
        ~HeadsUpDisplay();
        
        float LoadContent();
        
        void Update(double delta);
        void Draw();
        
    private:
        //Member variables
        World* m_World;
        Rect* m_OpeningRectLeft;
        Rect* m_OpeningRectRight;
        Sprite* m_HealthEmpty;
        Sprite* m_HealthHalf;
        Sprite* m_HealthFull;
        Sprite* m_Sword;
        Label* m_RupeeLabel;
    };
}

#endif /* defined(__GameDev2D__HeadsUpDisplay__) */
