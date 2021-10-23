//
//  Tile.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-14.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

/*
ChangeLog
_______________________________________________
-Made a variable TileTypeCount, type enum TileType, to account for my special number of tiles, 7.
- Made a const TileType TILES_TYPES[], to account for all the 7 different tile types in the game.
*/

#ifndef __GameDev2D__Tile__
#define __GameDev2D__Tile__

#include "../../Source/Core/GameObject.h"


namespace GameDev2D
{
	//Enum to help manage the tile types
	enum TileType
	{
		TileTypeUnknown = 0,         // (0000 0000 0000 0000)

		//The first 4 bits are reserved for tile variants

		TileTypeGround = 16,          // (0000 0000 0001 0000)
		TileTypeWater = 32,           // (0000 0000 0010 0000)
		TileTypeRock = 64,            // (0000 0000 0100 0000)
		TileTypeTree = 128,           // (0000 0000 1000 0000)
		TileTypeBoulder = 256,        // (0000 0001 0000 0000)
		TileTypeSand = 512,           // (0000 0010 0000 0000)
		TileTypeBridge = 1024,        // (0000 0100 0000 0000)
		TileTypeDoor = 2048,          // (0000 1000 0000 0000)
		TileTypeStairs = 4096,        // (0001 0000 0000 0000)
		TileTypeFire = 8192,          // (0010 0000 0000 0000)

		TileTypeCount = 8,//Modified, since it has more tiles than the first single player
		//TileTypeCount =5,//Individual project, only 5 tile types
		//TileTypeCount = 10,   //This value represents how many tile types are supported in your game
	};

	//Enum to help manage the tile variants
	enum TileVariant
	{
		TileVariant0 = 0,   // (0000 0000 0000 0000)
		TileVariant1 = 1,   // (0000 0000 0000 0001)
		TileVariant2 = 2,   // (0000 0000 0000 0010)
		TileVariant3 = 3,   // (0000 0000 0000 0011)
		TileVariant4 = 4,   // (0000 0000 0000 0100)
		TileVariant5 = 5,   // (0000 0000 0000 0101)
		TileVariant6 = 6,   // (0000 0000 0000 0110)
		TileVariant7 = 7,   // (0000 0000 0000 0111)
		TileVariant8 = 8,   // (0000 0000 0000 1000)
		TileVariant9 = 9,   // (0000 0000 0000 1001)
		TileVariant10 = 10,   // (0000 0000 0000 1010)
		TileVariant11 = 11,   // (0000 0000 0000 1011)
		TileVariant12 = 12,   // (0000 0000 0000 1100)
		TileVariant13 = 13,   // (0000 0000 0000 1101)
		TileVariant14 = 14,   // (0000 0000 0000 1110)
		TileVariant15 = 15,   // (0000 0000 0000 1111)
		TileVariantCount = 16
	};

	//Local constants
	const unsigned int TILE_HERO_SPAWN_POINT = 16384;    // (0100 0000 0000 0000)
	const unsigned int TILE_ENEMY_SPAWN_POINT = 32768;   // (1000 0000 0000 0000)
	const TileVariant TILE_VARIANTS[] = { TileVariant0, TileVariant1, TileVariant2, TileVariant3, TileVariant4, TileVariant5, TileVariant6, TileVariant7, TileVariant8, TileVariant9, TileVariant10, TileVariant11, TileVariant12, TileVariant13, TileVariant14, TileVariant15 };

	//For the TILE_TYPES[] constant below, uncomment the one that applies to you group size (1 or 2) based on the requirements
	const TileType TILES_TYPES[] = { TileTypeGround, TileTypeWater, TileTypeTree, TileTypeBoulder, TileTypeRock, TileTypeSand, TileTypeBridge,TileTypeFire };//Modified, since it has more tiles than the first single player
	//const TileType TILES_TYPES[] = {TileTypeGround, TileTypeWater, TileTypeTree, TileTypeBoulder, TileTypeRock};
//    const TileType TILES_TYPES[] = {TileTypeGround, TileTypeWater, TileTypeTree, TileTypeBoulder, TileTypeRock, TileTypeSand, TileTypeBridge, TileTypeDoor, TileTypeStairs, TileTypeFire};

	//Forward Declarations
	class SubSection;
	class Pickup;

	//The Tile class is the base class for all the Tile types in the game, it is an abstract class and must be inherited from.
	//It provides a lot of functionality for determining what type of tile it is, to easily calculating the center position
	//of the tile, managing tile variants and pickups. Keeps track of if its a 'walkable' tile OR not.
	class Tile : public GameObject
	{
	public:
		Tile(const string& tileName, SubSection* subSection, uvec2 coordinates);
		virtual ~Tile();

		//Pure virtual method, MUST be overridden in inheriting classes, should return the type of tile (from the enum above)
		virtual TileType GetTileType() = 0;

		//Pure virtual method, MUST be overridden in inheriting classes. Sets the atlas key (passed by reference) for the tile variant parameter
		virtual void GetAtlasKeyForVariant(unsigned int variant, string& atlasKey) = 0;

		//Updates and Draws the tile
		void Update(double delta);
		void Draw();

		//Returns how many variants are available for the Tile, by default its 1, can be overridden in inheriting classes
		virtual unsigned int GetVariantCount();

		//Returns the tile variant
		unsigned int GetVariant();

		//Set the tile's variant
		void SetVariant(unsigned int variant);

		//Returns the size of the tile
		unsigned int GetSize();

		//Returns the center position of the tile, either in local space (to the subsection) or world space
		vec2 GetCenter(bool worldPosition);

		//Returns the coordinates of the tile
		uvec2 GetCoordinates();

		//Returns the subsection the Tile is part of
		SubSection* GetSubSection();

		//Adds a pickup to the center of the tile
		void AddPickup(Pickup* pickup);

		//Remove the pickup from the tile
		void RemovePickup();

		//Returns the current Pickup object on the Tile, can return null if there is no pickup on the tile
		Pickup* GetPickup();

		//Returns wether the tile is walkable
		bool IsWalkable();

		//Set wether the tile can be walkable
		void SetIsWalkable(bool isWalkable);

		//Returns wether the tile is a hero spawn point
		bool IsHeroSpawnPoint();

		//Sets wether the tile is a hero spawn point
		void SetIsHeroSpawnPoint(bool isHeroSpawnPoint);

		//Returns wether the tile is an enemy spawn point
		bool IsEnemySpawnPoint();

		//Sets wether the tile is an enemy spawn point
		void SetIsEnemySpawnPoint(bool isEnemySpawnPoint);

	protected:
		//Conveniance method to update the sprite, based on the variant parameter, can be overridden
		virtual void UpdateSprite(unsigned int variant);

		//Member variables
		SubSection* m_SubSection;
		Pickup* m_Pickup;
		bool m_IsWalkable;
		bool m_IsHeroSpawnPoint;
		bool m_IsEnemySpawnPoint;
		unsigned int m_Variant;
	};
}

#endif /* defined(__GameDev2D__Tile__) */
