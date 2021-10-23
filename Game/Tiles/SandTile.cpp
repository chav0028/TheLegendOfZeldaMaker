/*******************************************************************
SandTile.cpp

Student:        Alvaro Chavez Mixco
Algonquin College
Date:           November 18, 2015
Course Number:  GAM1540
Professor:      David McCue
Purpose:
The Sand Tile basically handles displaying the correct sprite. Setting
if it is walkable, load the sprite corresponding to the different variants
it has, and returning the correct tile type.
*********************************************************************/

#include "SandTile.h"
#include "../../Source/UI/UI.h"

namespace GameDev2D
{
	SandTile::SandTile(SubSection* aSubSection, uvec2 aCoordinates, unsigned int aVariant) : Tile("Sand", aSubSection, aCoordinates)
	{
		//Set the variant
		SetVariant(aVariant);

		//Set that the sand tile IS walkable
		SetIsWalkable(true);
	}

	SandTile::~SandTile()
	{
	}

	TileType SandTile::GetTileType()
	{
		return TileTypeSand;
	}

	unsigned int SandTile::GetVariantCount()
	{
		return 1;
	}

	void SandTile::GetAtlasKeyForVariant(unsigned int aVariant, string& aAtlasKey)
	{
		switch (aVariant)
		{
		case 0:
			aAtlasKey = "Sand";
			break;

		default:
			break;
		}
	}
}