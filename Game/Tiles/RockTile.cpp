/*******************************************************************
RockTile.cpp

Student:        Alvaro Chavez Mixco
Algonquin College
Date:           November 18, 2015
Course Number:  GAM1540
Professor:      David McCue
Purpose:
The Rock Tile basically handles displaying the correct sprite. Setting
if it is walkable, load the sprite corresponding to the different variants
it has, and returning the correct tile type.
*********************************************************************/

#include "RockTile.h"
#include "../../Source/UI/UI.h"

namespace GameDev2D
{
	RockTile::RockTile(SubSection* aSubSection, uvec2 aCoordinates, unsigned int aVariant) : Tile("Rock-1", aSubSection, aCoordinates)
	{
		//Set the variant
		SetVariant(aVariant);

		//Set that the rock tile ISN'T walkable
		SetIsWalkable(false);
	}

	RockTile::~RockTile()
	{
	}

	TileType RockTile::GetTileType()
	{
		return TileTypeRock;
	}

	unsigned int RockTile::GetVariantCount()
	{
		return 12;
	}

	void RockTile::GetAtlasKeyForVariant(unsigned int aVariant, string& aAtlasKey)
	{
		//Sets all the variants for rock
		switch (aVariant)
		{
		case 0:
			aAtlasKey = "Rock-1";
			break;

		case 1:
			aAtlasKey = "Rock-2";
			break;

		case 2:
			aAtlasKey = "Rock-3";
			break;

		case 3:
			aAtlasKey = "Rock-4";
			break;

		case 4:
			aAtlasKey = "Rock-5";
			break;

		case 5:
			aAtlasKey = "Rock-6";
			break;

		case 6:
			aAtlasKey = "RedRock-1";
			break;

		case 7:
			aAtlasKey = "RedRock-2";
			break;

		case 8:
			aAtlasKey = "RedRock-3";
			break;

		case 9:
			aAtlasKey = "RedRock-4";
			break;

		case 10:
			aAtlasKey = "RedRock-5";
			break;

		case 11:
			aAtlasKey = "RedRock-6";
			break;

		default:
			break;
		}
	}
}