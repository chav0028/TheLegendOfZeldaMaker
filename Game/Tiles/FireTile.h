/*******************************************************************
FireTile.cpp

Student:        Alvaro Chavez Mixco
Algonquin College
Date:           December 12, 2015
Course Number:  GAM1540
Professor:      David McCue
Purpose:
The Fire Tile basically handles displaying the correct sprite. Setting
if it is walkable, load the sprite corresponding to the different variants
it has, and returning the correct tile type; it also animates the sprite.
*********************************************************************/

#ifndef __GameDev2D__FireTile__
#define __GameDev2D__FireTile__

#include "Tile.h"

namespace GameDev2D
{
	const double  FIRE_TILE_DEFAULT_ANIMATION_FRAMERATE=0.3;

	//Forward Declarations
	class SubSection;

	/***************************************************************
	Class: FireTile
	The FireTile class is a child of Tile. It creates the fire Tile
	sprite; handles the variants it has; and it sets/returns the correct properties
	according to the tile.
	***************************************************************/
	class FireTile : public Tile
	{
	public:
		FireTile(SubSection* subSection, uvec2 coordinates, unsigned int variant);
		~FireTile();


		void Update(double delta);

		//Returns the type of tile
		TileType GetTileType();

		//Returns the number of tile variants
		unsigned int GetVariantCount();

		//Gets the atlas key for the tile variant
		void GetAtlasKeyForVariant(unsigned int variant, string& atlasKey);

	private:
		double m_AnimationFrameRate;
		bool m_CurrentFrame;//Since we only have 2 animations we can easily use a bool to change them
		void changeAnimation();//Since we only have 2 animations we can easily use a bool to change them
	};
}

#endif /* defined(__GameDev2D__BridgeTile__) */