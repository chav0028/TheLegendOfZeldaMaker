/*******************************************************************
WaterTile.cpp

Student:        Alvaro Chavez Mixco
Algonquin College
Date:           November 18, 2015
Course Number:  GAM1540
Professor:      David McCue
Purpose:
The Water Tile basically handles displaying the correct sprite. Setting
if it is walkable, load the sprite corresponding to the different variants
it has, and returning the correct tile type.
*********************************************************************/

#include "WaterTile.h"
#include "../../Source/UI/UI.h"

namespace GameDev2D
{
	WaterTile::WaterTile(SubSection* aSubSection, uvec2 aCoordinates, unsigned int aVariant) : Tile("Water", aSubSection, aCoordinates)
	{
		//Set the variant
		SetVariant(aVariant);

		//Set that the water tile ISN'T walkable
		SetIsWalkable(false);
	}

	WaterTile::~WaterTile()
	{
	}

	TileType WaterTile::GetTileType()
	{
		return TileTypeWater;
	}

	unsigned int WaterTile::GetVariantCount()
	{
		return 9;
	}

	void WaterTile::GetAtlasKeyForVariant(unsigned int aVariant, string& aAtlasKey)
	{
		//Sets all the variants for Water
		switch (aVariant)
		{
		case 0:
			aAtlasKey = "Water";
			break;

		case 1:
			aAtlasKey = "WaterEdge-1";
			break;

		case 2:
			aAtlasKey = "WaterEdge-2";
			break;

		case 3:
			aAtlasKey = "WaterEdge-3";
			break;

		case 4:
			aAtlasKey = "WaterEdge-4";
			break;

		case 5:
			aAtlasKey = "WaterEdge-5";
			break;

		case 6:
			aAtlasKey = "WaterEdge-6";
			break;

		case 7:
			aAtlasKey = "WaterEdge-7";
			break;

		case 8:
			aAtlasKey = "WaterEdge-8";
			break;

		default:
			break;
		}
	}
}