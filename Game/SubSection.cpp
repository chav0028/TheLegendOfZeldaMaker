//
//  SubSection.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-15.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

/*
ChangeLog
_______________________________________________
-Modified CreateTile function, so that the switch statement in it took into account all the different tile types in the game.
-Implemented Load function.
-Implemented Save function.
-Added convenience method "DeactivateSpawnPointsAtTileIndex" to deactivate both enemy and hero spawn points in
a tile.
-Added SetEnemySpawnPoint function to ensure we don't place a enemy spawn point on top of the player spawn point.
*/

#include "SubSection.h"
#include "World.h"
#include "Player/Player.h"
#include "Player/Hero.h"
#include "Player/Enemy.h"
#include "Tiles/GroundTile.h"
#include "Tiles/WaterTile.h"
#include "Tiles/TreeTile.h"
#include "Tiles/BoulderTile.h"
#include "Tiles/RockTile.h"
#include "Tiles/SandTile.h"
#include "Tiles/BridgeTile.h"
#include "Tiles/FireTile.h"
#include "LevelEditor/LevelEditor.h"
#include "../Source/UI/UI.h"
#include "../Source/Graphics/Core/RenderTarget.h"
#include "../Source/Services/ServiceLocator.h"
#include "../Source/Audio/Audio.h"


namespace GameDev2D
{
    SubSection::SubSection(World* aWorld, uvec2 aCoordinates) : GameObject("SubSection"),
        m_World(aWorld),
        m_LevelEditor(nullptr),
        m_Tiles(nullptr),
        m_HeroSpawnPoint(nullptr),
        m_EnemySpawnPoint(nullptr),
        m_DebugDrawingFlags(DebugDrawNothing),
        m_DebugRenderTarget(nullptr)
    {
        //Set the Sub-section's position
        SetLocalPosition((float)(aCoordinates.x * aWorld->GetSubSectionSize().x), (float)(aCoordinates.y * aWorld->GetSubSectionSize().y));

        //Allocate the tiles array, the inheriting class will populate this array with Tile objects
        m_Tiles = new Tile*[GetNumberOfTiles()];

        //Initialize all the tiles to NULL
        for (unsigned int i = 0; i < GetNumberOfTiles(); i++)
        {
            m_Tiles[i] = NULL;
        }
    }

    SubSection::SubSection(LevelEditor* aLevelEditor, const string& aFilename) : GameObject("SubSection"),
        m_World(nullptr),
        m_LevelEditor(aLevelEditor),
        m_Tiles(nullptr),
        m_HeroSpawnPoint(nullptr),
        m_EnemySpawnPoint(nullptr),
        m_DebugDrawingFlags(DebugDrawNothing),
        m_DebugRenderTarget(nullptr)
    {
        //Allocate the tiles array, the inheriting class will populate this array with Tile objects
        m_Tiles = new Tile*[GetNumberOfTiles()];

        //Initialize all the tiles to NULL
        for (unsigned int i = 0; i < GetNumberOfTiles(); i++)
        {
            m_Tiles[i] = NULL;
        }

        //Create the hero and enemy spawn point sprites
        m_HeroSpawnPoint = new Sprite("MainAtlas", "Link-Spawn");
        m_EnemySpawnPoint = new Sprite("MainAtlas", "Enemy-Spawn");

        //Load the subsection
        Load(aFilename);
    }

    SubSection::~SubSection()
    {
        //Safety check the tiles pointer
        if (m_Tiles != nullptr)
        {
            //Cycle through and delete the tile objects
            for (unsigned int i = 0; i < GetNumberOfTiles(); i++)
            {
                SafeDelete(m_Tiles[i]);
            }

            //Delete the tiles array
            SafeDeleteArray(m_Tiles);
        }

        SafeDelete(m_HeroSpawnPoint);
        SafeDelete(m_EnemySpawnPoint);
        SafeDelete(m_DebugRenderTarget);
    }

    void SubSection::Load(const string& aFilename)
    {
        //Flag used to determine if an empty level should be loaded
        bool loadEmptyLevel = false;

        //Ensure the filename actually has some data in it
        if (aFilename.length() > 0)
        {

            //Open input stream (read file)
            ifstream inputStream;
            inputStream.open(aFilename, ifstream::in | ifstream::binary);

            //If the file opened
            if (inputStream.is_open() == true)
            {
                //Check how long the file is in bytes
                inputStream.seekg(0, inputStream.end);
                long long bufferSize = inputStream.tellg();//Save the size of the file

                //Rewind back to start of file
                inputStream.seekg(0, inputStream.beg);

                //Allocate buffer
                const unsigned int bufferLength = (unsigned int)bufferSize / sizeof(unsigned short);
                unsigned short* buffer = new unsigned short[bufferLength];//Create the buffer

                ///Read the data from the file, and assign it to the bufer
                inputStream.read((char*)buffer, (int)bufferSize);

                //Close the	input stream
                inputStream.close();

                //Go throguh all the tiles in the buffer and get their information
                for (unsigned int i = 0; i < bufferLength; i++)
                {
                    TileType tileType = TileTypeUnknown;
                    bool isHeroSpawnPoint = false;
                    bool isEnemySpawnPoint = false;

                    //Check if the tile is the hero spawn point
                    unsigned int heroValue = buffer[i] & TILE_HERO_SPAWN_POINT;//Check if it is the hero spawn point
                    if (heroValue > 0)//if it is a spawn point
                    {
                        isHeroSpawnPoint = true;//Set it so that the hero will spawn here
                        buffer[i] &= ~TILE_HERO_SPAWN_POINT;//Turn off the hero spawn point bits
                    }

                    //Check if the tile is the enemy spawn point
                    unsigned int enemyValue = buffer[i] & TILE_ENEMY_SPAWN_POINT;//Check if it is the enemy spawn point
                    if (enemyValue > 0)//if it is a spawn point
                    {
                        isEnemySpawnPoint = true;//Set it so that the enemy will spawn here
                        buffer[i] &= ~TILE_ENEMY_SPAWN_POINT;//Turn off the enemy spawn point bits
                    }

                    for (unsigned int j = 0; j < TileTypeCount; j++)
                    {
                        //"Bit mask" to check the tile type set
                        unsigned int value = (buffer[i] & TILES_TYPES[j]);

                        //If we have a tile type set
                        if (value > 0)
                        {
                            tileType = TILES_TYPES[j];//Set the current tile type

                            //Turn off the tile type bits using the "~' complement bitwie operator, to flip
                            //its value
                            buffer[i] &= ~TILES_TYPES[j];

                            break;
                        }
                    }

                    //Assert if the buffer didn't have a valid tile type data saved
                    assert(tileType != TileTypeUnknown);

                    //Check the tile variant
                    unsigned int tileVariant = buffer[i];

                    //Set the tile accordings to its type and variant
                    Tile*tile = SetTileTypeAtIndex(i, tileType, tileVariant);

                    //Check if the hero or enemy spawn points are in the tile
                    if (tile != nullptr)//Check that the tile is valid
                    {
                        tile->SetIsHeroSpawnPoint(isHeroSpawnPoint);
                        tile->SetIsEnemySpawnPoint(isEnemySpawnPoint);
                    }

                    //Spawn the enemy if it is a spawn point
                    if (isEnemySpawnPoint == true)
                    {
                        SpawnEnemy(tile);
                    }
                }

                //Delete the buffer
                SafeDeleteArray(buffer);
            }
            else//If the input stream couldn't be opened
            {
                Error(false, "The input stream failed to open: %s", aFilename.c_str());
                loadEmptyLevel = true;///Load an empty level instead
            }
        }
        else
        {
            //If we don't have a file to read (its name is empty), load an empty level
            loadEmptyLevel = true;
        }

        //Is the load empty level flag set to true, if so, load an empty level
        if (loadEmptyLevel == true)
        {
            //Create an empty world section
            for (unsigned int i = 0; i < GetNumberOfTiles(); i++)
            {
                SetTileTypeAtIndex(i, TileTypeGround, 0);//Set the tiles to be ground type

                //Ensure that the tile is not a hero or an enemy spawn point
                DeactivateSpawnPointsAtTileIndex(i);

                GetTileForIndex(i)->RemovePickup();//Remove any pick up that was in that tile

            }
        }
    }

    //Used to set the conditions of hero and enemy spawn points at the desired index to false, useful when we want to deactivate both of the spawn points in functions
    // like clear ,or when loading an empty level.
    void SubSection::DeactivateSpawnPointsAtTileIndex(unsigned int aIndex)
    {
        //Ensure that if we are loading an empty level, none of the tiles is set to be a spawn point
        m_Tiles[aIndex]->SetIsEnemySpawnPoint(false);
        m_Tiles[aIndex]->SetIsHeroSpawnPoint(false);
    }

    void SubSection::Save(const string& aFilename)
    {

        //Check that he actually passed a filename 
        if (aFilename.length() > 0)
        {
            //Create output stream (write to a file)
            ofstream outputStream;
            outputStream.open(aFilename, ofstream::out | ofstream::binary);

            //Check if the file opened
            if (outputStream.is_open() == true)
            {
                //Create an  array (buffer) to hold the tile type data
                const unsigned short bufferLength = GetNumberOfTiles();
                unsigned short* buffer = new unsigned short[bufferLength];

                //Fill the buffer
                //Go through all the tiles in the subsection, and save teh data
                for (unsigned int i = 0; i < bufferLength; i++)
                {
                    //Get the current tile
                    Tile* tile = GetTileForIndex(i);

                    //Set the tile in our buffer
                    buffer[i] = tile->GetTileType();

                    //Set the tile variant by using an "or" binary operator, to use the current tile variant as a bit mask.
                    buffer[i] |= tile->GetVariant();

                    //Set in the buffer the hero or enemy spawn point
                    if (tile->IsHeroSpawnPoint() == true)
                    {
                        //Set the tile variant by using an "or" binary operator, to use the hero spawn point as a bit mask.
                        buffer[i] |= TILE_HERO_SPAWN_POINT;
                    }
                    else if (tile->IsEnemySpawnPoint() == true)
                    {
                        //Set the tile variant by using an "or" binary operator, to use the enemy spawn point as a bit mask.
                        buffer[i] |= TILE_ENEMY_SPAWN_POINT;
                    }
                }

                //Write the buffer to a file
                unsigned int bufferSize = bufferLength*sizeof(unsigned short);
                outputStream.write((const char*)buffer, bufferSize);

                //Close the output stream
                outputStream.close();

                //Now that the content of the buffer has been written to a file, delete the buffer.
                SafeDeleteArray(buffer);
            }
            else //if we can't open the ouput stream
            {
                Error(false, "Failed to save: %s", aFilename.c_str());
            }
        }
    }

    void SubSection::Update(double aDelta)
    {
        //Is the sub-section active?
        if (IsEnabled() == true)
        {
            //Cycle through the tiles and update them
            unsigned int i = 0;
            for (i = 0; i < GetNumberOfTiles(); i++)
            {
                if (m_Tiles[i] != nullptr)
                {
                    m_Tiles[i]->Update(aDelta);
                }
            }

            //Cycle through and update all the enemies
            for (i = 0; i < m_Enemies.size(); i++)
            {
                m_Enemies.at(i)->Update(aDelta);
            }
        }
    }

    void SubSection::Draw()
    {
        //Is the sub-section active?
        if (IsEnabled() == true)
        {
            //Cycle through the tiles and draw them
            unsigned int i = 0;
            for (i = 0; i < GetNumberOfTiles(); i++)
            {
                //
                if (m_Tiles[i] != nullptr)
                {
                    //Draw the tile
                    m_Tiles[i]->Draw();

                    //Are we in the level editor (we know this because the pointer would be null if we weren't)
                    if (m_LevelEditor != nullptr)
                    {
                        //Draw the hero spawn point
                        if (m_Tiles[i]->IsHeroSpawnPoint() == true)
                        {
                            m_HeroSpawnPoint->SetLocalPosition(m_Tiles[i]->GetLocalPosition());
                            m_HeroSpawnPoint->Draw();
                        }

                        //Draw the enemy spawn point
                        if (m_Tiles[i]->IsEnemySpawnPoint() == true)
                        {
                            m_EnemySpawnPoint->SetLocalPosition(m_Tiles[i]->GetLocalPosition());
                            m_EnemySpawnPoint->Draw();
                        }
                    }
                }
            }

            //If there is debug data to draw, draw it
            if (m_DebugDrawingFlags != DebugDrawNothing && m_DebugRenderTarget != nullptr)
            {
                mat4 matrix = translate(mat4(1.0f), vec3(GetWorldPosition().x, GetWorldPosition().y, 0.0f));
                m_DebugRenderTarget->GetTextureFrame()->Draw(matrix);
            }

            //Cycle through and draw all the enemies
            for (i = 0; i < m_Enemies.size(); i++)
            {
                m_Enemies.at(i)->Draw();
            }
        }
    }

    uvec2 SubSection::GetSize()
    {
        return vec2(GetNumberOfHorizontalTiles() * GetTileSize(), GetNumberOfVerticalTiles() * GetTileSize());
    }

    unsigned int SubSection::GetNumberOfHorizontalTiles()
    {
        if (m_World != nullptr)
        {
            return m_World->GetNumberOfHorizontalTiles();
        }
        else if (m_LevelEditor != nullptr)
        {
            return m_LevelEditor->GetNumberOfHorizontalTiles();
        }
        return 0;
    }

    unsigned int SubSection::GetNumberOfVerticalTiles()
    {
        if (m_World != nullptr)
        {
            return m_World->GetNumberOfVerticalTiles();
        }
        else if (m_LevelEditor != nullptr)
        {
            return m_LevelEditor->GetNumberOfVerticalTiles();
        }
        return 0;
    }

    unsigned int SubSection::GetNumberOfTiles()
    {
        return GetNumberOfHorizontalTiles() * GetNumberOfVerticalTiles();
    }

    unsigned int SubSection::GetTileSize()
    {
        if (m_World != nullptr)
        {
            return m_World->GetTileSize();
        }
        else if (m_LevelEditor != nullptr)
        {
            return m_LevelEditor->GetTileSize();
        }
        return 0;
    }

    TileType SubSection::GetTileTypeForIndex(unsigned int aIndex)
    {
        if (aIndex < GetNumberOfTiles())
        {
            if (m_Tiles[aIndex] != nullptr)
            {
                return m_Tiles[aIndex]->GetTileType();
            }
        }
        return TileTypeUnknown;
    }

    Tile* SubSection::SetTileTypeAtIndex(unsigned int aIndex, TileType aTileType, unsigned int aVariant)
    {
        if (aIndex < GetNumberOfTiles())
        {


            //Don't replace the tile if its the same type of tile that already exists
            if (m_Tiles[aIndex] != nullptr && m_Tiles[aIndex]->GetTileType() == aTileType)
            {
                //Ensure the variant was updated before we return
                m_Tiles[aIndex]->SetVariant(aVariant);
                return m_Tiles[aIndex];
            }

            //Delete the tile at the index, if one exists
            RemoveChild(m_Tiles[aIndex]);
            SafeDelete(m_Tiles[aIndex]);

            //BeginProfile();
            //Set the tile at the index
            m_Tiles[aIndex] = CreateTile(aTileType, aVariant, GetTileCoordinatesForIndex(aIndex));

            //double duration = EndProfile();
            //Log("%f", duration);

            //Add the tile as a child
            if (m_Tiles[aIndex] != nullptr)
            {
                AddChild(m_Tiles[aIndex], false);
            }



            return m_Tiles[aIndex];
        }

        return nullptr;
    }

    bool SubSection::ValidateCoordinates(ivec2 aCoordinates)
    {
        if (aCoordinates.x < 0 || aCoordinates.y < 0 || aCoordinates.x >= (int)GetNumberOfHorizontalTiles() || aCoordinates.y >= (int)GetNumberOfVerticalTiles())
        {
            return false;
        }
        return true;
    }

    uvec2 SubSection::GetTileCoordinatesForIndex(unsigned int aIndex)
    {
        //If this assert is hit, the index you passed in is out of bounds
        assert(aIndex < GetNumberOfTiles());

        //Calculate the coordinates and set the tile position and size
        uvec2 coordinates = uvec2(0, 0);
        coordinates.x = (aIndex % GetNumberOfHorizontalTiles());
        coordinates.y = ((aIndex - coordinates.x) / GetNumberOfHorizontalTiles());

        return coordinates;
    }

    uvec2 SubSection::GetTileCoordinatesForPosition(vec2 aPosition)//
    {
        uvec2 coordinates = uvec2(aPosition.x / (float)GetTileSize(), aPosition.y / (float)GetTileSize());
        return coordinates;
    }

    uvec2 SubSection::GetTileCoordinatesForTile(Tile* aTile)
    {
        return GetTileCoordinatesForIndex(GetTileIndexForTile(aTile));
    }

    unsigned int SubSection::GetTileIndexForPosition(vec2 aPosition)//
    {
        uvec2 coordinates = GetTileCoordinatesForPosition(aPosition);
        return GetTileIndexForCoordinates(coordinates);
    }

    unsigned int SubSection::GetTileIndexForCoordinates(uvec2 aCoordinates)
    {
        return aCoordinates.x + (aCoordinates.y * GetNumberOfHorizontalTiles());
    }

    unsigned int SubSection::GetTileIndexForTile(Tile* aTile)
    {
        return GetTileIndexForPosition(aTile->GetLocalPosition());
    }

    Tile* SubSection::GetTileForIndex(unsigned int aIndex)
    {
        //If this assert is hit, the index that was passed in is out of bounds

        assert(aIndex < GetNumberOfTiles());
        assert(aIndex >= 0);

        //If this assert is hit, then the tile object at the index is null
        assert(m_Tiles[aIndex] != nullptr);

        //Return the tile at the index
        return m_Tiles[aIndex];
    }

    Tile* SubSection::GetTileForPosition(vec2 aPosition)
    {
        return GetTileForIndex(GetTileIndexForPosition(aPosition));
    }

    Tile* SubSection::GetTileForCoordinates(uvec2 aCoordinates)
    {
        return GetTileForIndex(GetTileIndexForCoordinates(aCoordinates));
    }

    void SubSection::GetActiveEnemies(vector<Enemy*>& aEnemies)
    {
        for (unsigned int i = 0; i < m_Enemies.size(); i++)
        {
            if (m_Enemies.at(i)->IsEnabled() == true)
            {
                aEnemies.push_back(m_Enemies.at(i));
            }
        }
    }

    Tile* SubSection::CreateTile(TileType aTileType, unsigned int aVariant, uvec2 aCoordinates)
    {
        //Create the new tile based on the TileType
        switch (aTileType)
        {
        case TileTypeGround:
            return new GroundTile(this, aCoordinates, aVariant);

        case TileTypeTree:
            return new TreeTile(this, aCoordinates, aVariant);

        case TileTypeWater:
            return new WaterTile(this, aCoordinates, aVariant);

        case TileTypeBoulder:
            return new BoulderTile(this, aCoordinates, aVariant);

        case TileTypeRock:
            return new RockTile(this, aCoordinates, aVariant);

        case TileTypeSand:
            return new SandTile(this, aCoordinates, aVariant);

        case TileTypeBridge:
            return new BridgeTile(this, aCoordinates, aVariant);

        case TileTypeFire:
            return new FireTile(this, aCoordinates, aVariant);

        case TileTypeUnknown:
        default:
            break;
        }

        return nullptr;
    }

    void SubSection::SetHeroSpawnPointForTile(Tile* aTile)
    {
        if (aTile->IsHeroSpawnPoint() == true)//If the tile is a hero spawn point already
        {
            aTile->SetIsHeroSpawnPoint(false);//Deactivate it
        }
        else//If the tile is not a hero spawn point
        {
            //Cycle through and ensure there isn't already a spawn point set for this hero
            for (unsigned int i = 0; i < GetNumberOfTiles(); i++)
            {
                GetTileForIndex(i)->SetIsHeroSpawnPoint(false);
            }

            //Set the tile hero spawn point flag
            aTile->SetIsHeroSpawnPoint(true);

            //If there is an enemy spawn point set, turn it off
            if (aTile->IsEnemySpawnPoint() == true)
            {
                aTile->SetIsEnemySpawnPoint(false);
            }
        }
    }

    void SubSection::SetEnemySpawnPoint(Tile* aTile)
    {
        //If the tile is already a spawn point
        if (aTile->IsEnemySpawnPoint() == true)
        {
            aTile->SetIsEnemySpawnPoint(false);//Deactivate the spawn point
        }
        else//If the tile isn't a enemy spawn point
        {
            aTile->SetIsEnemySpawnPoint(true);//Set it as a spawn point

            //Check if there is already a hero spawn point in the tile
            if (aTile->IsHeroSpawnPoint() == true)//If it is a hero spawn point
            {
                aTile->SetIsHeroSpawnPoint(false);//Deactivate the hero spawn point
            }
        }

    }

    void SubSection::SpawnEnemy(Tile* aTile)
    {
        if (m_World != nullptr)
        {
            m_Enemies.push_back(new Enemy(m_World, aTile));
        }
    }

    void SubSection::EnableDebugDrawing(unsigned int aDebugDrawingFlags)
    {
#if DEBUG
        //Set the debug draw flags
        m_DebugDrawingFlags = aDebugDrawingFlags;

        //Is there anything to draw?
        if (m_DebugDrawingFlags != DebugDrawNothing)
        {
            //Create a render target, if one doesn't already exist
            if (m_DebugRenderTarget == nullptr)
            {
                unsigned int width = GetTileSize() * GetNumberOfHorizontalTiles();
                unsigned int height = GetTileSize() * GetNumberOfVerticalTiles();
                m_DebugRenderTarget = new RenderTarget(width, height);
                m_DebugRenderTarget->CreateFrameBuffer();
                m_DebugRenderTarget->GetTextureFrame()->SetShader(ServiceLocator::GetShaderManager()->GetPassthroughTextureShader());
            }

            //Refresh the debug draw render target
            RefreshDebugDraw();
        }
        else
        {
            //Delete the render target
            SafeDelete(m_DebugRenderTarget);
        }
#endif
    }

    void SubSection::DisableDebugDrawing()
    {
        EnableDebugDrawing(DebugDrawNothing);
    }

    unsigned int SubSection::GetDebugDrawFlags()
    {
        return m_DebugDrawingFlags;
    }

    void SubSection::RefreshDebugDraw()
    {
#if DEBUG
        //Cache the graphics active render target
        RenderTarget* oldRenderTarget = ServiceLocator::GetGraphics()->GetActiveRenderTarget();

        //Set the Label's render target as the active render target
        ServiceLocator::GetGraphics()->SetActiveRenderTarget(m_DebugRenderTarget);
        ServiceLocator::GetGraphics()->Clear();

        //Draw the debug tile data
        bool tileData = (m_DebugDrawingFlags & (DebugDrawTileIndex)) != 0 || (m_DebugDrawingFlags & (DebugDrawTileCoordinates)) != 0 || (m_DebugDrawingFlags & (DebugDrawTilePositionLocal)) != 0 || (m_DebugDrawingFlags & (DebugDrawTilePositionWorld)) != 0;
        if (tileData == true)
        {
            DebugDrawTileData();
        }

        //Draw the enemy pathfinding data
        bool pathFindingScores = (m_DebugDrawingFlags & (DebugDrawPathFindingScores)) != 0;
        if (pathFindingScores == true)
        {
            for (unsigned int i = 0; i < m_Enemies.size(); i++)
            {
                m_Enemies.at(i)->DebugDraw();
            }
        }

        //Draw the sub section data
        bool coordinateData = (m_DebugDrawingFlags & (DebugDrawSubSectionIndex)) != 0 || (m_DebugDrawingFlags & (DebugDrawSubSectionCoordinates)) != 0 || (m_DebugDrawingFlags & (DebugDrawSubSectionPosition)) != 0;
        if (coordinateData == true)
        {
            DebugDrawSubSectionData();
        }

        //Set the original graphic's render target back
        ServiceLocator::GetGraphics()->SetActiveRenderTarget(oldRenderTarget);
#endif
    }

    void SubSection::DebugDrawTileData()
    {
        //Which debug tile data do we draw
        bool tileIndex = (m_DebugDrawingFlags & (DebugDrawTileIndex)) != 0;
        bool tileCoordinates = (m_DebugDrawingFlags & (DebugDrawTileCoordinates)) != 0;
        bool tilePositionLocal = (m_DebugDrawingFlags & (DebugDrawTilePositionLocal)) != 0;
        bool tilePositionWorld = (m_DebugDrawingFlags & (DebugDrawTilePositionWorld)) != 0;

        //Set the font size
        const unsigned int fontSizes[] = { 32, 18, 13, 12 };
        unsigned int fontSize = fontSizes[0];

        if (tilePositionWorld)
        {
            fontSize = fontSizes[3];
        }
        else
        {
            if (tilePositionLocal)
            {
                fontSize = fontSizes[2];
            }
            else
            {
                fontSize = fontSizes[1];
            }
        }


        //Create the debug draw label (on the stack), it will only be used in this method, which is seldomly called
        Label debugLabel("Ubuntu-B", "ttf", fontSize);
        debugLabel.SetAnchorPoint(0.5f, 0.5f);
        debugLabel.SetJustification(JustifyCenter);
        debugLabel.SetColor(Color::WhiteColor());

        //Create the rect outline
        Rect rect;
        rect.SetSize((float)GetTileSize(), (float)GetTileSize());
        rect.SetAnchorPoint(0.5f, 0.5f);
        rect.SetColor(Color::WhiteColor());
        rect.SetIsFilled(false);

        //Initialize a string stream
        stringstream ss;

        //Cycle through all the tiles in the subsection
        for (unsigned int i = 0; i < GetNumberOfTiles(); i++)
        {
            //Set the position for the debug label
            vec2 position = GetTileForIndex(i)->GetCenter(false);
            debugLabel.SetLocalPosition(position);

            //Clear the string stream
            ss.str("");

            //Do we need to draw the tile index data
            if (tileIndex == true)
            {
                //Add the index to the string stream
                ss << i;

                //Do we also have to draw the tile coordinates data?
                if (tileCoordinates == true || tilePositionLocal == true || tilePositionWorld == true)
                {
                    //If we do - then do a new line
                    ss << "ren";
                }
                else
                {
                    //Otherwise set the text and draw the label
                    debugLabel.SetText(ss.str());
                    debugLabel.Draw();
                }
            }

            //Do we need to draw the tile coordinate data
            if (tileCoordinates == true)
            {
                //Add the tile coordinates to the string stream
                uvec2 coordinates = GetTileCoordinatesForIndex(i);
                ss << "(" << coordinates.x << "," << coordinates.y << ")";

                //Do we also have to draw the tile coordinates data?
                if (tilePositionLocal == true || tilePositionWorld == true)
                {
                    //If we do - then do a new line
                    ss << "\n";
                }
                else
                {
                    //Otherwise set the text and draw the label
                    debugLabel.SetText(ss.str());
                    debugLabel.Draw();
                }
            }

            //Do we need to draw the tile's local position
            if (tilePositionLocal == true)
            {
                //Add the tile coordinates to the string stream
                Tile* tile = GetTileForIndex(i);
                ss << "(" << tile->GetLocalX() << "," << tile->GetLocalY() << ")";

                //Do we also have to draw the tile coordinates data?
                if (tilePositionWorld == true)
                {
                    //If we do - then do a new line
                    ss << "\n";
                }
                else
                {
                    //Otherwise set the text and draw the label
                    debugLabel.SetText(ss.str());
                    debugLabel.Draw();
                }
            }

            //Do we need to draw the tile's world position
            if (tilePositionWorld == true)
            {
                //Add the tile coordinates to the string stream
                Tile* tile = GetTileForIndex(i);
                ss << "(" << tile->GetWorldX() << "," << tile->GetWorldY() << ")";


                //Set the text and draw the label
                debugLabel.SetText(ss.str());
                debugLabel.Draw();
            }


            //Draw a white Rect around the Tile
            rect.SetLocalPosition(position);
            rect.Draw();
        }
    }

    void SubSection::DebugDrawSubSectionData()
    {
        //Which debug tile data do we draw
        bool subSectionIndex = (m_DebugDrawingFlags & (DebugDrawSubSectionIndex)) != 0;
        bool subSectionCoordinates = (m_DebugDrawingFlags & (DebugDrawSubSectionCoordinates)) != 0;
        bool subSectionPosition = (m_DebugDrawingFlags & (DebugDrawSubSectionPosition)) != 0;
        unsigned int fontSize = subSectionIndex == true && subSectionCoordinates == false ? 60 : 42;

        //Create the debug draw label (on the stack), it will only be used in this method, which is seldomly called
        Label debugLabel("Ubuntu-B", "ttf", fontSize);
        debugLabel.SetAnchorPoint(0.5f, 0.5f);
        debugLabel.SetJustification(JustifyCenter);
        debugLabel.SetColor(Color::GhostWhiteColor());

        //Initialize a string stream
        stringstream ss;

        //Get the index for this subsection
        unsigned int index = m_World->GetSubSectionIndexForPosition(GetWorldPosition());

        //Set the position for the debug label
        vec2 position = vec2((float)(GetTileSize() * GetNumberOfHorizontalTiles()) / 2.0f, (float)(GetTileSize() * GetNumberOfVerticalTiles()) / 2.0f);
        debugLabel.SetLocalPosition(position);

        //Do we need to draw the subsection index data
        if (subSectionIndex == true)
        {
            //Add the index to the string stream
            ss << index;

            //Do we also have to draw the subsection coordinates data?
            if (subSectionCoordinates == true || subSectionPosition == true)
            {
                //If we do - then do a new line
                ss << "\n";
            }
            else
            {
                //Otherwise set the text and draw the label
                debugLabel.SetText(ss.str());
                debugLabel.Draw();
            }
        }

        //Do we need to draw the subsection coordinate data
        if (subSectionCoordinates == true)
        {
            //Add the subsection coordinates to the string stream
            uvec2 coordinates = m_World->GetSubSectionCoordinatesForIndex(index);
            ss << "(" << coordinates.x << "," << coordinates.y << ")";

            //Do we also have to draw the subsection coordinates data?
            if (subSectionPosition == true)
            {
                //If we do - then do a new line
                ss << "\n";
            }
            else
            {
                //Otherwise set the text and draw the label
                debugLabel.SetText(ss.str());
                debugLabel.Draw();
            }
        }

        //Do we need to draw the subsection position data
        if (subSectionPosition == true)
        {
            //Add the subsection coordinates to the string stream
            ss << "(" << GetWorldX() << "," << GetWorldY() << ")";


            //Set the text and draw the label
            debugLabel.SetText(ss.str());
            debugLabel.Draw();
        }

        //Draw a white Rect around the SubSection
        Rect rect(GetLocalPosition(), GetSize());
        rect.SetColor(Color::WhiteColor());
        rect.SetIsFilled(false);
        rect.Draw();
    }
}