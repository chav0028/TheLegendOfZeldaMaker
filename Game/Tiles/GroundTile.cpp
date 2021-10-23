//
//  GroundTile.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-15.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

/*
ChangeLog
_______________________________________________
-Implemented all the different  variants for the ground tile.
*/

#include "GroundTile.h"
#include "../../Source/UI/UI.h"


namespace GameDev2D
{
	GroundTile::GroundTile(SubSection* aSubSection, uvec2 aCoordinates, unsigned int aVariant) : Tile("GroundTile", aSubSection, aCoordinates)
	{
		//Set the variant
		SetVariant(aVariant);

		//Set that the ground tile IS walkable
		SetIsWalkable(true);
	}

	GroundTile::~GroundTile()
	{
	}

	TileType GroundTile::GetTileType()
	{
		return TileTypeGround;
	}

	unsigned int GroundTile::GetVariantCount()
	{
		return 5;
	}

	void GroundTile::GetAtlasKeyForVariant(unsigned int aVariant, string& aAtlasKey)
	{
		//Sets all the variants for ground
		switch (aVariant)
		{
		case 0:
			aAtlasKey = "Ground";
			break;

		case 1:
			aAtlasKey = "WaterEdge-9";
			break;

		case 2:
			aAtlasKey = "WaterEdge-10";
			break;

		case 3:
			aAtlasKey = "WaterEdge-11";
			break;

		case 4:
			aAtlasKey = "WaterEdge-12";
			break;

		default:
			break;
		}
	}
}