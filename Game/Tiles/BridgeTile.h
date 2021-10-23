/*******************************************************************
BridgeTile.h

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

#ifndef __GameDev2D__BridgeTile__
#define __GameDev2D__BridgeTile__

#include "Tile.h"

namespace GameDev2D
{
	//Forward Declarations
	class SubSection;

	/***************************************************************
	Class: BridgeTile
	The BridgeTile class is a child of Tile. It creates the Bridge Tile
	sprite; handles the variants it has; and it sets/returns the correct properties
	according to the tile.
	***************************************************************/
	class BridgeTile : public Tile
	{
	public:
		BridgeTile(SubSection* subSection, uvec2 coordinates, unsigned int variant);
		~BridgeTile();

		//Returns the type of tile
		TileType GetTileType();

		//Returns the number of tile variants
		unsigned int GetVariantCount();

		//Gets the atlas key for the tile variant
		void GetAtlasKeyForVariant(unsigned int variant, string& atlasKey);
	};
}

#endif /* defined(__GameDev2D__BridgeTile__) */