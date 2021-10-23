/*******************************************************************
HeartPickup.h

Student:        Alvaro Chavez Mixco
Algonquin College
Date:           December 12, 2015
Course Number:  GAM1540
Professor:      David McCue
Purpose:
The heart pick up is a type of pick up, so it is in charge of animating the
sprite, and returning the correct pickup type.
*********************************************************************/

#ifndef __GameDev2D__HeartPickup__
#define __GameDev2D__HeartPickup__

#include "Pickup.h"

namespace GameDev2D
{
    const float HEART_PICKUP_FRAME_SPEED = 4.0f;

    /***************************************************************
    Class: HeartPickup
    The HeartPickup is a type of pick up, so it is in charge of drawing the correct
    sprite and animating it, and returning the correct pickup type.  The class DOESN'T distacte
    the effects of the pick up.
    ***************************************************************/
    class HeartPickup : public Pickup
    {
    public:
        HeartPickup();
        ~HeartPickup();
        virtual PickupType GetPickupType();
    };

}


#endif