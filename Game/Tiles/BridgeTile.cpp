/*******************************************************************
BridgeTile.cpp

Student:        Alvaro Chavez Mixco
Algonquin College
Date:           November 18, 2015
Course Number:  GAM1540
Professor:      David McCue
Purpose:
The Bridge Tile basically handles displaying the correct sprite. Setting
if it is walkable, load the sprite corresponding to the different variants
it has, and returning the correct tile type.
*********************************************************************/

#include "BridgeTile.h"
#include "../../Source/UI/UI.h"

namespace GameDev2D
{
	BridgeTile::BridgeTile(SubSection* aSubSection, uvec2 aCoordinates, unsigned int aVariant) : Tile("Bridge-1", aSubSection, aCoordinates)
	{
		//Set the variant
		SetVariant(aVariant);

		//Set that the bridge tile IS walkable
		SetIsWalkable(true);
	}

	BridgeTile::~BridgeTile()
	{
	}

	TileType BridgeTile::GetTileType()
	{
		return TileTypeBridge;
	}

	unsigned int BridgeTile::GetVariantCount()
	{
		return 2;
	}

	void BridgeTile::GetAtlasKeyForVariant(unsigned int aVariant, string& aAtlasKey)
	{
		switch (aVariant)
		{
		case 0:
			aAtlasKey = "Bridge-1";
			break;

		case 1:
			aAtlasKey = "Bridge-2";
			break;

		default:
			break;
		}
	}
}