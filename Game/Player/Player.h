//
//  Player.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-14.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Player__
#define __GameDev2D__Player__

#include "../../Source/Core/GameObject.h"


namespace GameDev2D
{
    //Local Constants
    const vec2 PLAYER_DIRECTION_UP = vec2(0.0f, 1.0f);
    const vec2 PLAYER_DIRECTION_DOWN = vec2(0.0f, -1.0f);
    const vec2 PLAYER_DIRECTION_LEFT = vec2(-1.0f, 0.0f);
    const vec2 PLAYER_DIRECTION_RIGHT = vec2(1.0f, 0.0f);
    const vec2 PLAYER_DIRECTIONS[] = {PLAYER_DIRECTION_UP, PLAYER_DIRECTION_DOWN, PLAYER_DIRECTION_LEFT, PLAYER_DIRECTION_RIGHT};
    const unsigned int PLAYER_DIRECTION_COUNT = 4;

    //Forward Declarations
    class World;
    class Tile;
    class SubSection;
    class Sprite;
    class Timer;
    
    //The Player class is the base class, all hero and enemy classes should inherit from the Player,
    //provides functionality for direction, speed, health, health capacity, attack damage. Has methods
    //that are called when the player takes damage or dies. Conveniance method to easily determine
    //which Tile or SubSection the player is on.
    class Player : public GameObject
    {
    public:
        Player(const string& playerType, World* world);
        virtual ~Player();
        
        //Pure virtual method, MUST be overridden, handles player's internal state
        virtual void SetState(unsigned int state) = 0;
        
        //Resets the Player, ensures it is enable, can be overridden
        virtual void Reset();
        
        //Returns the Tile the player is currently on
        Tile* GetTile();
        
        //Returns the SubSection the player is currently on
        SubSection* GetSubSection();
        
        //Gets and sets the direction of the player
        vec2 GetDirection();
        void SetDirection(vec2 direction);
        
        //Gets and sets the speed of the player
        float GetSpeed();
        void SetSpeed(float speed);
        
        //Gets and sets the health of the player
        unsigned int GetHealth();
        void SetHealth(unsigned int health);
        
        //Gets and sets the health capacity of the player
        unsigned int GetHealthCapacity();
        void SetHealthCapacity(unsigned int healthCapacity);
        
        //Gets and sets the attack damage of the player
        unsigned int GetAttackDamage();
        void SetAttackDamage(unsigned int attackDamage);

        //Is the player invincible, damage can NOT be applied to the Player while invicible, by default returns false, can be overridden
        virtual bool IsInvincible();

        //Apply a certain damage amount to the player, can be overridden
        virtual void ApplyDamage(unsigned int attackDamage);
        
    protected:
        //Conveniance method that is called when a player has died, can be overridden
        virtual void HasDied();
    
        //Conveniance method that is called when a player changes tiles, can be overridden
        virtual void HasChangedTiles(Tile* newTile, Tile* oldTile);
        
        //Conveniance method that is called when a player changes subsections, can be overridden
        virtual void HasChangedSubSections(SubSection* newSubSection, SubSection* oldSubsection);
        
        //Allows the World class to access the protected members
        friend class World;
    
        //Member variables
        World* m_World;
        Sprite* m_ActiveSprite;
        Sprite* m_WalkingSprite[4];
        unsigned int m_DirectionIndex;
        float m_Speed;
        unsigned int m_Health;
        unsigned int m_HealthCapacity;
        unsigned int m_AttackDamage;
    };
}

#endif /* defined(__GameDev2D__Player__) */
