/*******************************************************************
BlueRupeePickup.h

Student:        Alvaro Chavez Mixco
Algonquin College
Date:           December 12, 2015
Course Number:  GAM1540
Professor:      David McCue
Purpose:
The blue rupee is a type of pick up, so it is in charge of drawing the correct
sprite, and returning the correct pickup type.
*********************************************************************/

#ifndef __GameDev2D__BlueRupeePickup__
#define __GameDev2D__BlueRupeePickup__

#include "Pickup.h"

namespace GameDev2D
{
    /***************************************************************
    Class: BlueRupeePickup
    The blue rupee is a type of pick up, so it is in charge of drawing the correct
    sprite, and returning the correct pickup type.  The class DOESN'T distacte
    the effects of the pick up.
    ***************************************************************/
    class BlueRupeePickup : public Pickup
    {
    public:
        BlueRupeePickup();
        ~BlueRupeePickup();
        virtual PickupType GetPickupType();
    };

}


#endif