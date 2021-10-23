/*******************************************************************
HeroProjectile.cpp

Student:        Alvaro Chavez Mixco
Algonquin College
Date:           December 12, 2015
Course Number:  GAM1540
Professor:      David McCue
Purpose:
The Hero Projectile is merely a child of projectile, so basically it
is a projectile that has a different sprite
*********************************************************************/

#include "HeroProjectile.h"
#include "../../Source/UI/Sprite/Sprite.h"

namespace GameDev2D
{
    HeroProjectile::HeroProjectile() :Projectile(false)
    {
        LoadSprite();//Create a sprite
        Reset();
    }

    HeroProjectile::~HeroProjectile()
    {
    }

    void HeroProjectile::LoadSprite()
    {
        // create sprite and add it as a child.
        m_Sprite = new Sprite("MainAtlas", "Sword-Pickup");
        m_Sprite->SetAnchorPoint(0.5, 0.5);
        AddChild(m_Sprite, false);
    }

}
