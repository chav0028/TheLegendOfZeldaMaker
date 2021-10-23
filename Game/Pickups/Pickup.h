//
//  Pickup.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-21.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Pickup__
#define __GameDev2D__Pickup__

#include "../../Source/Core/GameObject.h"

namespace GameDev2D
{
    //Pickup enum to help manage the different types of pickups
    enum PickupType
    {
        PickupTypeHeart = 0,
        PickupTypeGreenRupee,
        PickupTypeBlueRupee,
        PickupTypeCount,
    };

    //Forward declarations
    class Tile;

    //The Pickup class is the base class for all the pickup types in the game, it is very 
    //simple in its design as it only needs to manage its type and the tile it resides on
    class Pickup : public GameObject
    {
    public:
        Pickup(const string& pickupName);
        virtual ~Pickup();

        //MUST be overridden in inheriting classes
        virtual PickupType GetPickupType() = 0;

        //Returns the tile the pickup is on
        Tile* GetTile();

    protected:
        //Set the tile the pickup is on
        void SetTile(Tile* tile);

        //Allows the Tile to access the SetTile() method, don't want that to be called accidently
        friend class Tile;

        //Member variable
        Tile* m_Tile;
    };
}

#endif /* defined(__GameDev2D__Pickup__) */
