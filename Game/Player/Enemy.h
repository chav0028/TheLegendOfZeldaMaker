//
//  Enemy.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

/*
ChangeLog
_______________________________________________
-Added enemy searching and attacking states
-Added constants for the chances of enemy dropping something upon death.
-Added constants for flickering alpha.
*/

#ifndef __GameDev2D__Enemy__
#define __GameDev2D__Enemy__

#include "Player.h"
#include "../Pool.h"
#include "../Projectile.h"

namespace GameDev2D
{
    //Enum to help manage the enemy's internal state
    enum EnemyState
    {
        EnemyIdle = 0,

        //Add additonal enemy states here
		EnemyWalking,
		EnemySearching,
        EnemyAttacking,		
        EnemyStateCount,
        EnemyDead,
        EnemyUnknown
    };

    //Local constants
    const string ENEMY_WALKING_ATLAS_KEY_1[] = { "EnemyUp-1", "EnemyDown-1", "EnemyLeft-1", "EnemyRight-1" };
    const string ENEMY_WALKING_ATLAS_KEY_2[] = { "EnemyUp-2", "EnemyDown-2", "EnemyLeft-2", "EnemyRight-2" };

    const float ENEMY_WALKING_SPEED = 2048.0f;
    const float ENEMY_WALKING_ANIMATION_FRAME_SPEED = 5.0f;
    const double ENEMY_IDLE_TIME_INCREMENTS = 0.5;
    const unsigned int ENEMY_MIN_IDLE_TIME = 1;//1
    const unsigned int ENEMY_MAX_IDLE_TIME = 2;//4
    const double ENEMY_PROJECTILE_DELAY = 1.5;//1.5
    const unsigned int ENEMY_DEFAULT_NUMBER_PROJECTILES = 5;
    const float ENEMY_PROJECTILE_DEFAULT_SPEED = 100.0f;
    const int ENEMY_PROJECTILE_DEFAULT_ATTACK_DAMAGE = 1;

    //Enemy random drop rate percentage

    //drop rate when hero has low health
    const unsigned int ENEMY_DROP_RATE_LOW_HERO_HEALTH_HEART_PICKUP_MIN = 0;//50% chance of dropping heart pick up
    const unsigned int ENEMY_DROP_RATE_LOW_HERO_HEALTH_HEART_PICKUP_MAX = 50;

    //drop rate when hero has full health
    const unsigned int ENEMY_DROP_RATE_MAX_HERO_HEALTH_GREEN_RUPEE_PICKUP_MIN = 0;
    const unsigned int ENEMY_DROP_RATE_MAX_HERO_HEALTH_GREEN_RUPEE_PICKUP_MAX = 30;//30% chance of dropping a green rupee pick up
    const unsigned int ENEMY_DROP_RATE_MAX_HERO_HEALTH_BLUE_RUPEE_PICKUP_MIN = ENEMY_DROP_RATE_MAX_HERO_HEALTH_GREEN_RUPEE_PICKUP_MAX;
    const unsigned int ENEMY_DROP_RATE_MAX_HERO_HEALTH_BLUE_RUPEE_PICKUP_MAX = 50;//20% chance of dropping a blue rupee pick up

    //drop rates in normal state
    const unsigned int ENEMY_DROP_RATE_NORMAL_HEART_PICKUP_MIN = 0;//20% chance of dropping heart pick up
    const unsigned int ENEMY_DROP_RATE_NORMAL_HEART_PICKUP_MAX = 20;
    const unsigned int ENEMY_DROP_RATE_NORMAL_GREEN_RUPEE_PICKUP_MIN = ENEMY_DROP_RATE_NORMAL_HEART_PICKUP_MAX;//20% chance of dropping a green rupee pick up
    const unsigned int ENEMY_DROP_RATE_NORMAL_GREEN_RUPEE_PICKUP_MAX = 40;
    const unsigned int ENEMY_DROP_RATE_NORMAL_BLUE_RUPEE_PICKUP_MIN = ENEMY_DROP_RATE_NORMAL_GREEN_RUPEE_PICKUP_MAX;//10% chance of dropping a blue rupee pick up
    const unsigned int ENEMY_DROP_RATE_NORMAL_BLUE_RUPEE_PICKUP_MAX = 50;

    //Forward declarations
    class PathFinder;
    class SubSection;
    class Random;
    class Audio;

    //Create an enemy object, enemies can be killed by the Hero's sword and can apply damage to the hero when they share
    //the same time. The enemy will cycle its behaviour between Idle, randomly walking, searching for the hero and
    //firing a projectile at the hero.
    class Enemy : public Player
    {
    public:
        Enemy(World* world, Tile* spawnTile);
        ~Enemy();

        //Update method, used to perform pathfinding operations
        void Update(double delta);

        void Draw();

        //Debug Draw method, use to draw pathfinding debug data
        void DebugDraw();

        //Used to reset the enemy
        void Reset();

        //Pure virtual function used to manage the enemy's internal state
        void SetState(unsigned int state);

        //Overwrrite functions so that they can play sound
        void ApplyDamage(unsigned int attackDamage);

    private:
        //Called when the Enemy has died
        void HasDied();

        //Called when the pathfinder found a path to the destination
        void StartWalking();

        //Walk to the next node in the path-called delayed recursively
        void Walk();

        //Fire Projectile, function to use delayed call
        void FireProjectile();

        //Change the enemy's current state to something different
        void ChangeState();

        //Refreshes the active sprite and updates it based on the current direction
        void RefreshActiveSprite();

        //Member variables
        PathFinder* m_PathFinder;
        EnemyState m_EnemyState;
        Random* m_Random;
        unsigned int m_PathIndex;
        Audio* m_DamagedSound;
        Audio* m_DeadSound;

        //Projectiles object pooling
        Pool<Projectile> m_ProjectilePool;

    };
}

#endif /* defined(__GameDev2D__Enemy__) */
