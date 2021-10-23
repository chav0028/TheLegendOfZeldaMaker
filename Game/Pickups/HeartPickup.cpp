/*******************************************************************
HeartPickup.cpp

Student:        Alvaro Chavez Mixco
Algonquin College
Date:           December 12, 2015
Course Number:  GAM1540
Professor:      David McCue
Purpose:
The heart pick up is a type of pick up, so it is in charge of animating the
sprite, and returning the correct pickup type.
*********************************************************************/

#include "HeartPickup.h"
#include "../../Source/UI/UI.h"

namespace GameDev2D
{
    HeartPickup::HeartPickup() :Pickup("HeartPickUp")
    {
        //Create sprite
        Sprite* sprite = new Sprite("MainAtlas", "Heart-Full");

        //Set properties of the sprite for animation
        sprite->AddFrame("MainAtlas", "Heart-2");
        sprite->SetDoesLoop(true);
        sprite->SetFrameSpeed(HEART_PICKUP_FRAME_SPEED);
        sprite->SetAnchorPoint(0.5f, 0.5f);

        //Add the sprite as a child
        AddChild(sprite, true);
    }

    HeartPickup::~HeartPickup()
    {
    }

    PickupType HeartPickup::GetPickupType()
    {
        return PickupTypeHeart;
    }

}
