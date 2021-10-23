//
//  Player.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-14.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Player.h"
#include "../World.h"
#include "../SubSection.h"
#include "../../Source/Animation/Timer.h"


namespace GameDev2D
{
    Player::Player(const string& aPlayerType, World* aWorld) : GameObject(aPlayerType),
        m_World(aWorld),
        m_ActiveSprite(nullptr),
        m_DirectionIndex(1),
        m_Speed(0.0f),
        m_Health(1),
        m_HealthCapacity(1),
        m_AttackDamage(1)
    {

    }
    
    Player::~Player()
    {
        //Set the world pointer active sprite pointer to null
        m_World = nullptr;
        m_ActiveSprite = nullptr;
    }
    
    void Player::Reset()
    {
        //Reset the player's enabled state
        SetIsEnabled(true);
    
        //Reset the base object
        GameObject::Reset();
    }
    
    Tile* Player::GetTile()
    {
        //Get the sub-section the player is on
        SubSection* subSection = m_World->GetSubSectionForPlayer(this);
        
        //Get the position of the player relative to the subsection
        vec2 position = GetWorldPosition() - subSection->GetWorldPosition();
    
        //Get the tile for the player's relative position
        Tile* tile = subSection->GetTileForPosition(position);
        
        //Return the tile
        return tile;
    }
    
    SubSection* Player::GetSubSection()
    {
        if(m_World != nullptr)
        {
            return m_World->GetSubSectionForPlayer(this);
        }
        return nullptr;
    }
    
    vec2 Player::GetDirection()
    {
        return PLAYER_DIRECTIONS[m_DirectionIndex];
    }
    
    void Player::SetDirection(vec2 aDirection)
    {
        if(aDirection == PLAYER_DIRECTION_UP)
        {
            m_DirectionIndex = 0;
        }
        else if(aDirection == PLAYER_DIRECTION_DOWN)
        {
            m_DirectionIndex = 1;
        }
        else if(aDirection == PLAYER_DIRECTION_LEFT)
        {
            m_DirectionIndex = 2;
        }
        else if(aDirection == PLAYER_DIRECTION_RIGHT)
        {
            m_DirectionIndex = 3;
        }
    }
    
    float Player::GetSpeed()
    {
        return m_Speed;
    }
    
    void Player::SetSpeed(float aSpeed)
    {
        m_Speed = aSpeed;
    }
    
    unsigned int Player::GetHealth()
    {
        return m_Health;
    }
    
    void Player::SetHealth(unsigned int aHealth)
    {
        //Set the health
        m_Health = aHealth;
        
        //Safety check the health against the capacity
        if(m_Health > GetHealthCapacity())
        {
            m_Health = GetHealthCapacity();
        }
    }
    
    unsigned int Player::GetHealthCapacity()
    {
        return m_HealthCapacity;
    }
    
    void Player::SetHealthCapacity(unsigned int aHealthCapacity)
    {
        m_HealthCapacity = aHealthCapacity;
    }
    
    unsigned int Player::GetAttackDamage()
    {
        return m_AttackDamage;
    }
    
    void Player::SetAttackDamage(unsigned int aAttackDamage)
    {
        m_AttackDamage = aAttackDamage;
    }
    
    bool Player::IsInvincible()
    {
        return false;
    }
    
    void Player::ApplyDamage(unsigned int aAttackDamage)
    {
        //Decrement the player's health if they aren't currently invincible
        if(IsInvincible() == false)
        {
            SetHealth(GetHealth() - aAttackDamage);
        }
    
        //If the health has reached zero, then call the HasDied() method
        if(GetHealth() <= 0)
        {
            HasDied();
        }
    }
    
    void Player::HasDied()
    {
        SetIsEnabled(false);
    }
    
    void Player::HasChangedTiles(Tile* aNewTile, Tile* aOldTile)
    {
        //Override this method to handle a player changing tiles
    }
    
    void Player::HasChangedSubSections(SubSection* aNewSubSection, SubSection* aOldSubsection)
    {
        //Override this method to handle a player changing subsections
    }
}