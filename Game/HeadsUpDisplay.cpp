//
//  HeadsUpDisplay.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-15.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "HeadsUpDisplay.h"
#include "Game.h"
#include "World.h"
#include "Player/Hero.h"
#include "../Source/Graphics/Primitives/Primitives.h"
#include "../Source/Services/ServiceLocator.h"
#include "../Source/Platforms/PlatformLayer.h"
#include "../Source/UI/UI.h"


namespace GameDev2D
{
    HeadsUpDisplay::HeadsUpDisplay(World* aWorld) : Scene("HeadsUpDisplay"),
        m_World(aWorld),
        m_OpeningRectLeft(nullptr),
        m_OpeningRectRight(nullptr),
        m_HealthEmpty(nullptr),
        m_HealthHalf(nullptr),
        m_HealthFull(nullptr),
        m_Sword(nullptr),
        m_RupeeLabel(nullptr)
    {
    
    }
    
    HeadsUpDisplay::~HeadsUpDisplay()
    {
        SafeDelete(m_HealthEmpty);
        SafeDelete(m_HealthHalf);
        SafeDelete(m_HealthFull);
    }
    
    float HeadsUpDisplay::LoadContent()
    {
        //Get the screen width and height
        float screenWidth = (float)ServiceLocator::GetPlatformLayer()->GetWidth();
        float screenHeight = (float)ServiceLocator::GetPlatformLayer()->GetHeight();
    
        //Setup the opening transition (left)
        m_OpeningRectLeft = new Rect(0.0f, 0.0f, screenWidth / 2.0f, screenHeight);
        m_OpeningRectLeft->SetColor(Color::BlackColor());
        m_OpeningRectLeft->SetWidth(0.0f, GAME_INTRO_DURATION);
        AddGameObject(m_OpeningRectLeft);
        
        //Setup the opening transition (right)
        m_OpeningRectRight = new Rect(screenWidth, 0.0f, screenWidth / 2.0f, screenHeight);
        m_OpeningRectRight->SetColor(Color::BlackColor());
        m_OpeningRectRight->SetAnchorPoint(1.0f, 0.0f);
        m_OpeningRectRight->SetWidth(0.0f, GAME_INTRO_DURATION);
        AddGameObject(m_OpeningRectRight);
    
        //Create the top black bar for the heads up display elements to be drawn on top of
        float startingY = (float)(m_World->GetTileSize() * m_World->GetNumberOfVerticalTiles());
        float height = screenHeight - startingY;
        Rect* rect = new Rect(0.0f, 704.0f, screenWidth, height);
        rect->SetColor(Color::BlackColor());
        AddGameObject(rect);
        
        //Create the life label
        Label* lifeLabel = new Label("BPdotsSquareBold", "otf", HUD_LIFE_LABEL_SIZE);
        lifeLabel->SetText("-LIFE-");
        lifeLabel->SetLocalPosition(screenWidth * HUD_LIFE_LABEL_POSITION_PCT.x, screenHeight * HUD_LIFE_LABEL_POSITION_PCT.y);
        lifeLabel->SetAnchorPoint(0.5f, 0.5f);
        lifeLabel->SetColor(Color::RedColor());
        AddGameObject(lifeLabel);
        
        //Create the health sprites
        m_HealthEmpty = new Sprite("MainAtlas", "Heart-Empty");
        m_HealthEmpty->SetAnchorPoint(0.5f, 0.5f);
        m_HealthHalf = new Sprite("MainAtlas", "Heart-Half");
        m_HealthHalf->SetAnchorPoint(0.5f, 0.5f);
        m_HealthFull = new Sprite("MainAtlas", "Heart-Full");
        m_HealthFull->SetAnchorPoint(0.5f, 0.5f);
        
        //Create the sword sprite
        m_Sword = new Sprite("MainAtlas", "Sword-Pickup");
        m_Sword->SetAnchorPoint(0.5f, 0.5f);
        m_Sword->SetLocalPosition(screenWidth * HUD_SWORD_SPRITE_POSITION_PCT.x, screenHeight * HUD_SWORD_SPRITE_POSITION_PCT.y);
        m_Sword->SetIsEnabled(false);
        AddGameObject(m_Sword);
        
        //Create the shield sprite
        Sprite* shield = new Sprite("MainAtlas", "Shield");
        shield->SetAnchorPoint(0.5f, 0.5f);
        shield->SetLocalPosition(screenWidth * HUD_SHIELD_SPRITE_POSITION_PCT.x, screenHeight * HUD_SHIELD_SPRITE_POSITION_PCT.y);
        shield->SetIsEnabled(true);
        AddGameObject(shield);
        
        //Create the rupee sprite
        Sprite* rupeeSprite = new Sprite("MainAtlas", "Rupee-Green");
        rupeeSprite->SetAnchorPoint(1.0f, 0.0f);
        rupeeSprite->SetLocalPosition(screenWidth * HUD_RUPEE_SPRITE_POSITION_PCT.x, screenHeight * HUD_RUPEE_SPRITE_POSITION_PCT.y);
        rupeeSprite->SetLocalScale(0.5f, 0.5f);
        AddGameObject(rupeeSprite);
        
        //Create the rupee label
        m_RupeeLabel = new Label("slkscr", "ttf", 32);
        m_RupeeLabel->SetAnchorPoint(0.0f, 0.0f);
        m_RupeeLabel->SetLocalPosition(rupeeSprite->GetLocalPosition() + vec2(8.0f, 0.0f));
        AddGameObject(m_RupeeLabel);
        
        //Return loading percentage
        return 1.0f;
    }
    
    void HeadsUpDisplay::Update(double aDelta)
    {
        if(m_World != nullptr && m_World->GetHero() != nullptr)
        {
            //Check to see if the sword has been added
            if(m_Sword->IsEnabled() == false)
            {
                if(m_World->GetHero()->HasSword() == true)
                {
                    m_Sword->SetIsEnabled(true);
                }
            }

            //Update the rupee label
            stringstream ss;
            ss << m_World->GetHero()->GetRupees();
            m_RupeeLabel->SetText(ss.str());
        }
    
        Scene::Update(aDelta);
    }
    
    void HeadsUpDisplay::Draw()
    {
        Scene::Draw();
        
        //Safety check the world and hero pointers
        if(m_World != nullptr && m_World->GetHero() != nullptr)
        {
            //Get the health and health capacity
            float screenWidth = (float)ServiceLocator::GetPlatformLayer()->GetWidth();
            float screenHeight = (float)ServiceLocator::GetPlatformLayer()->GetHeight();
            const unsigned int health = m_World->GetHero()->GetHealth();
            const unsigned int halfHealth = health / 2;
            const unsigned int healthCapacity = m_World->GetHero()->GetHealthCapacity();
            const unsigned int numberOfHearts = healthCapacity / 2;
            bool needsHalfHeart = (healthCapacity - health) % 2 == 1;
            unsigned int columnIndex = 0;
            vec2 position = vec2(screenWidth * HUD_HEARTS_STARTING_POSITION_PCT.x, screenHeight * HUD_HEARTS_STARTING_POSITION_PCT.y);

            //Cycle through the number of hearts and draw the hero's health
            for(unsigned int i = 0; i < numberOfHearts; i++)
            {
                if(needsHalfHeart == true && halfHealth == i)
                {
                    m_HealthHalf->SetLocalPosition(position);
                    m_HealthHalf->Draw();
                }
                else
                {
                    if(halfHealth > i)
                    {
                        m_HealthFull->SetLocalPosition(position);
                        m_HealthFull->Draw();
                    }
                    else
                    {
                        m_HealthEmpty->SetLocalPosition(position);
                        m_HealthEmpty->Draw();
                    }
                }
                
                //Increment the x position
                position.x += HUD_HEART_INCREMENT;
                
                //Increment the column index
                columnIndex++;
                
                //Has the max columns been reached?
                if(columnIndex >= HUD_NUMBER_OF_HEART_COLUMNS)
                {
                    columnIndex = 0;
                    position.y += HUD_HEART_INCREMENT;
                    position.x = screenWidth * HUD_HEARTS_STARTING_POSITION_PCT.x;
                }
            }
        }
    }
}