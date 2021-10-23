/*******************************************************************
TreeTile.cpp

Student:        Alvaro Chavez Mixco
Algonquin College
Date:           November 18, 2015
Course Number:  GAM1540
Professor:      David McCue
Purpose:
The Tree Tile basically handles displaying the correct sprite. Setting
if it is walkable, load the sprite corresponding to the different variants
it has, and returning the correct tile type.
*********************************************************************/

#include "TreeTile.h"
#include "../../Source/UI/UI.h"


namespace GameDev2D
{
	TreeTile::TreeTile(SubSection* aSubSection, uvec2 aCoordinates, unsigned int aVariant) : Tile("Tree-Light", aSubSection, aCoordinates)
	{
		//Set the variant
		SetVariant(aVariant);

		//Set that the tree tile ISN'T walkable
		SetIsWalkable(false);
	}

	TreeTile::~TreeTile()
	{
	}

	TileType TreeTile::GetTileType()
	{
		return TileTypeTree;
	}

	unsigned int TreeTile::GetVariantCount()
	{
		return 7;
	}

	void TreeTile::GetAtlasKeyForVariant(unsigned int aVariant, string& aAtlasKey)
	{
		//Set all the different variants
		switch (aVariant)
		{
		case 0:
			aAtlasKey = "Tree-Light";
			break;

		case 1:
			aAtlasKey = "Tree-Dark";
			break;

		case 2:
			aAtlasKey = "EvilTree-1";
			break;

		case 3:
			aAtlasKey = "EvilTree-2";
			break;

		case 4:
			aAtlasKey = "EvilTree-3";
			break;

		case 5:
			aAtlasKey = "EvilTree-4";
			break;

		case 6:
			aAtlasKey = "EvilTree-5";
			break;

		default:
			break;
		}
	}
}