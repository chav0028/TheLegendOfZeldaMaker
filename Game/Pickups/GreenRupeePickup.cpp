/*******************************************************************
GreenRupeePickup.cpp

Student:        Alvaro Chavez Mixco
Algonquin College
Date:           December 12, 2015
Course Number:  GAM1540
Professor:      David McCue
Purpose:
The green rupee is a type of pick up, so it is in charge of drawing the correct
sprite, and returning the correct pickup type.
*********************************************************************/

#include "GreenRupeePickup.h"
#include "../../Source/UI/UI.h"

namespace GameDev2D
{
    GreenRupeePickup::GreenRupeePickup() :Pickup("GreenRupeePickup")
    {
        //Create sprite
        Sprite* sprite = new Sprite("MainAtlas", "Rupee-Green");
        sprite->SetAnchorPoint(0.5f, 0.5f);

        //Add the sprite as a child
        AddChild(sprite, true);
    }

    GreenRupeePickup::~GreenRupeePickup()
    {
    }

    PickupType GreenRupeePickup::GetPickupType()
    {
        return PickupTypeGreenRupee;
    }

}
