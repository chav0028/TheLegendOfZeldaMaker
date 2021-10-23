/*******************************************************************
GreenRupeePickup.h

Student:        Alvaro Chavez Mixco
Algonquin College
Date:           December 12, 2015
Course Number:  GAM1540
Professor:      David McCue
Purpose:
The green rupee is a type of pick up, so it is in charge of drawing the correct
sprite, and returning the correct pickup type.
*********************************************************************/

#ifndef __GameDev2D__GreenRupeePickup__
#define __GameDev2D__GreenRupeePickup__

#include "Pickup.h"

namespace GameDev2D
{
    /***************************************************************
    Class: GreenRupeePickup
    The blue rupee is a type of pick up, so it is in charge of drawing the correct
    sprite, and returning the correct pickup type.  The class DOESN'T distacte
    the effects of the pick up.
    ***************************************************************/
    class GreenRupeePickup : public Pickup
    {
    public:
        GreenRupeePickup();
        ~GreenRupeePickup();
        virtual PickupType GetPickupType();
    };

}


#endif