/*******************************************************************
BoulderTile.cpp

Student:        Alvaro Chavez Mixco
Algonquin College
Date:           November 18, 2015
Course Number:  GAM1540
Professor:      David McCue
Purpose:
The Boulder Tile basically handles displaying the correct sprite. Setting
if it is walkable, load the sprite corresponding to the different variants
it has, and returning the correct tile type.
*********************************************************************/

#include "BoulderTile.h"
#include "../../Source/UI/UI.h"

namespace GameDev2D
{
	BoulderTile::BoulderTile(SubSection* aSubSection, uvec2 aCoordinates, unsigned int aVariant) : Tile("Boulder-Light", aSubSection, aCoordinates)
	{
		//Set the variant
		SetVariant(aVariant);

		//Set that the boulder tile ISN'T walkable
		SetIsWalkable(false);
	}

	BoulderTile::~BoulderTile()
	{
	}

	TileType BoulderTile::GetTileType()
	{
		return TileTypeBoulder;
	}

	unsigned int BoulderTile::GetVariantCount()
	{
		return 2;
	}

	void BoulderTile::GetAtlasKeyForVariant(unsigned int aVariant, string& aAtlasKey)
	{
		switch (aVariant)
		{
		case 0:
			aAtlasKey = "Boulder-Light";
			break;

		case 1:
			aAtlasKey = "Boulder-Dark";
			break;

		default:
			break;
		}
	}
}