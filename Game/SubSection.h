//
//  SubSection.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-15.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

/*
ChangeLog
_______________________________________________
-Added convenience method "DeactivateSpawnPointsAtTileIndex" to deactivate both enemy and hero spawn points in
a tile.
-Added SetEnemySpawnPoint function to ensure we don't place a enemy spawn point on top of the player spawn point.
*/

#ifndef __GameDev2D__SubSection__
#define __GameDev2D__SubSection__

#include "../Source/Core/GameObject.h"
#include "Tiles/Tile.h"


namespace GameDev2D
{
    //Local enum for DebugDrawFlags
    enum SubSectionDebugDrawing
    {
        DebugDrawNothing = 0,
        DebugDrawTileIndex = 1,
        DebugDrawTileCoordinates = 2,
        DebugDrawTilePositionLocal = 4,
        DebugDrawTilePositionWorld = 8,
        DebugDrawPathFindingScores = 16,
        DebugDrawSubSectionIndex = 32,
        DebugDrawSubSectionCoordinates = 64,
        DebugDrawSubSectionPosition = 128
    };

    //Forward declarations
    class World;
    class LevelEditor;
    class Tile;
    class Player;
    class Enemy;
    class Label;
    class Sprite;
    class RenderTarget;

    //The Subsection class manages the Tile objects and the Enemy objects that
    //reside within the SubSection. Enemies can NOT leave the SubSection
    class SubSection : public GameObject
    {
    public:
        //There are two constructors for SubSection, one for the LevelEditor, one for the Wolrd (in game).
        //The SubSection can be used by both without conflict.
        SubSection(World* world, uvec2 coordinates);
        SubSection(LevelEditor* levelEditor, const string& filename);
        ~SubSection();

        //Loads a Subsection for the filename, if load fails, then an empty level will be loaded
        void Load(const string& filename);

        //Saves a Subsection for the filename
        void Save(const string& filename);

        //Updates the Subsection
        void Update(double delta);

        //Draws the subsection
        void Draw();

        //Returns the size of the subsection
        uvec2 GetSize();

        //Returns the tile size for the Tile's in the subsection. Tile's are perfectly square
        unsigned int GetTileSize();

        //Returns the number of horizontal, vertical and total tiles in the subsection
        unsigned int GetNumberOfHorizontalTiles();
        unsigned int GetNumberOfVerticalTiles();
        unsigned int GetNumberOfTiles();

        //Returns the Tile type of the Tile at the index
        TileType GetTileTypeForIndex(unsigned int index);

        //Change the tile type at the index, returns the Tile object that was changed/created
        Tile* SetTileTypeAtIndex(unsigned int index, TileType tileType, unsigned int variant);

        //Validates that the coordinates are on the subsection
        bool ValidateCoordinates(ivec2 coordinates);

        //Methods to return a Tile coordinates
        uvec2 GetTileCoordinatesForIndex(unsigned int index);
        uvec2 GetTileCoordinatesForPosition(vec2 position);
        uvec2 GetTileCoordinatesForTile(Tile* tile);

        //Methods to return a Tile index
        unsigned int GetTileIndexForPosition(vec2 position);
        unsigned int GetTileIndexForCoordinates(uvec2 coordinates);
        unsigned int GetTileIndexForTile(Tile* tile);

        //Methods to return a Tile object
        Tile* GetTileForIndex(unsigned int index);
        Tile* GetTileForPosition(vec2 position);
        Tile* GetTileForCoordinates(uvec2 coordinates);

        //Populates the enemies parameter with the active enemy objects in the SubSection
        void GetActiveEnemies(vector<Enemy*>& enemies);

        //Methods for debug drawing
        void EnableDebugDrawing(unsigned int debugDrawingFlags);
        void DisableDebugDrawing();
        unsigned int GetDebugDrawFlags();
        void RefreshDebugDraw();

    protected:
        //Create a new Tile object for tile type and variant at the coordinates passed in
        Tile* CreateTile(TileType tileType, unsigned int variant, uvec2 coordinates);

        //Method to set the hero's spawn point, if another hero spawn point is already set, it will be removed
        void SetHeroSpawnPointForTile(Tile* tile);
        void SetEnemySpawnPoint(Tile* tile);

        //Convenience method to set the state for both hero and enemy spawn points.
        void DeactivateSpawnPointsAtTileIndex(unsigned int index);

        //Spawn's an enemy on the tile
        void SpawnEnemy(Tile* tile);

        //Allow level editor access to the protected memebers
        friend LevelEditor;

    private:
        //Private debug drawing methods
        void DebugDrawTileData();
        void DebugDrawSubSectionData();

        //Member variables
        World* m_World;
        LevelEditor* m_LevelEditor;
        Tile** m_Tiles;
        vector<Enemy*> m_Enemies;
        Sprite* m_HeroSpawnPoint;
        Sprite* m_EnemySpawnPoint;
        unsigned int m_DebugDrawingFlags;
        RenderTarget* m_DebugRenderTarget;
    };
}

#endif /* defined(__GameDev2D__SubSection__) */
