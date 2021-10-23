/*******************************************************************
Projectile.h

Student:        Alvaro Chavez Mixco
Algonquin College
Date:           December 12, 2015
Course Number:  GAM1540
Professor:      David McCue
Purpose:
The projectile class is a class that handles all the basic properties  a
projectile has. Mostly involving the stats it will have once it is activated.
The class also handles deactivating the projectile (settind enabled as false
and if it belongs to a pool telling it to the pool ) when it goes off the Hero
subsection or when it collides with a nonwalkable tile.
*********************************************************************/

#ifndef __GameDev2D__Projectile__
#define __GameDev2D__Projectile__

#include "../Source/Core/GameObject.h"
#include "Pool.h"

using namespace glm;

namespace GameDev2D
{
    class Projectile;
    class SubSection;
    class Sprite;
    class Tile;

    /***************************************************************
    Class: Projectile
    The Projectile is a child of game objects. It is responsible for setting
    the properties of the projecitle once it is fired, deactivating the projectile,
    and checking for collision against the enviroment.
    ***************************************************************/
    class Projectile : public GameObject
    {
    public:
        Projectile(bool useDefaultSprite = true);//Special constructor, because this doesn't content a load content function
        Projectile(Pool<Projectile>* pool);//if the projectile is part of a pool
        ~Projectile();

        virtual void LoadSprite();

        virtual void Reset();

        virtual void Fire(SubSection* subsection, vec2 position, vec2 direction, float speed, int attackDamage);

        virtual void Update(double delta);

        virtual void Draw();

        int GetAttackDamage();

        Tile* GetProjectileTile();
        void DeactivateProjectile();

    protected:

        SubSection* m_SubSection;

        Sprite* m_Sprite;
        vec2 m_Direction;
        float m_Speed;
        int m_AttackDamage;

        //Sprite* m_Sprite;

        Pool<Projectile>* m_PoolOwner;//the pool in which the projectile is
    };
}

#endif /* defined(__GameDev2D__Projectile__) */
