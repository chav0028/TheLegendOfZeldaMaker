/*******************************************************************
HeroProjectile.h

Student:        Alvaro Chavez Mixco
Algonquin College
Date:           December 12, 2015
Course Number:  GAM1540
Professor:      David McCue
Purpose:
The Hero Projectile is merely a child of projectile, so basically it
is a projectile that has a different sprite
*********************************************************************/

#ifndef __GameDev2D__HeroProjectile__
#define __GameDev2D__HeroProjectile__

#include "Projectile.h"

using namespace glm;

namespace GameDev2D
{
    /***************************************************************
    Class: HeroProjectile
    The Hero Projectile is merely a child of projectile, so basically it
    is a projectile that has a different sprite
    ***************************************************************/
    class HeroProjectile : public Projectile
    {
    public:
        HeroProjectile();
        ~HeroProjectile();

        void LoadSprite();
    };
}

#endif /* defined(__GameDev2D__HeroProjectile__) */
