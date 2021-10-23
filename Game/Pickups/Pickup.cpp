//
//  Pickup.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-21.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Pickup.h"
#include "../Tiles/Tile.h"

namespace GameDev2D
{
    Pickup::Pickup(const string& aPickupName) : GameObject(aPickupName),
        m_Tile(nullptr)
    {

    }

    Pickup::~Pickup()
    {

    }

    Tile* Pickup::GetTile()
    {
        return m_Tile;
    }

    void Pickup::SetTile(Tile* aTile)
    {
        m_Tile = aTile;
    }
}