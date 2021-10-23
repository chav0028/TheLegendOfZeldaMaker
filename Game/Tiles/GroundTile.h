//
//  GroundTile.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-15.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__GroundTile__
#define __GameDev2D__GroundTile__

#include "Tile.h"


namespace GameDev2D
{
    //Forward Declarations
    class SubSection;

    //Ground Tile class
    class GroundTile : public Tile
    {
    public:
        GroundTile(SubSection* subSection, uvec2 coordinates, unsigned int variant);
        ~GroundTile();
        
        //Returns the type of tile
        TileType GetTileType();
        
        //Returns the number of tile variants
        unsigned int GetVariantCount();
        
        //Gets the atlas key for the tile variant
        void GetAtlasKeyForVariant(unsigned int variant, string& atlasKey);
    };
}

#endif /* defined(__GameDev2D__GroundTile__) */
