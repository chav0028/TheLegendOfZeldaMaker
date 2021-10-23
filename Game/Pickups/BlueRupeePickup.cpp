/*******************************************************************
BlueRupeePickup.cpp

Student:        Alvaro Chavez Mixco
Algonquin College
Date:           December 12, 2015
Course Number:  GAM1540
Professor:      David McCue
Purpose:
The blue rupee is a type of pick up, so it is in charge of drawing the correct
sprite, and returning the correct pickup type.
*********************************************************************/

#include "BlueRupeePickup.h"
#include "../../Source/UI/UI.h"

namespace GameDev2D
{
    BlueRupeePickup::BlueRupeePickup() :Pickup("BlueRupeePickup")
    {
        //Create sprite
        Sprite* sprite = new Sprite("MainAtlas", "Rupee-Blue");
        sprite->SetAnchorPoint(0.5f, 0.5f);

        //Add the sprite as a child
        AddChild(sprite, true);
    }

    BlueRupeePickup::~BlueRupeePickup()
    {
    }

    PickupType BlueRupeePickup::GetPickupType()
    {
        return PickupTypeBlueRupee;
    }

}
