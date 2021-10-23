//
//  LevelEditor.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-14.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__LevelEditor__
#define __GameDev2D__LevelEditor__

#include "../../Source/Core/Scene.h"
#include "../../Source/UI/UI.h"
#include "../Tiles/Tile.h"


namespace GameDev2D
{
    //Level editor load step enum
    enum LevelEditorLoadStep
    {
        LevelEditorLoadRequired = 0,
        LevelEditorLoadSelectionSprite,
        LevelEditorLoadSubSection,
        LevelEditorLoadButtons,
        LevelEditorLoadSubSectionMenu,
        LevelEditorLoadMusic,
        LevelEditorLoadStepCount
    };

    //Local constants
    const string LEVEL_EDITOR_TILE_BUTTON_TEXT[] = { "Ground", "Water", "Tree", "Boulder", "Rock", "Sand", "Bridge", "Fire" };
    const TileType LEVEL_EDITOR_TILE_TYPES[] = { TileTypeGround, TileTypeWater, TileTypeTree, TileTypeBoulder, TileTypeRock, TileTypeSand, TileTypeBridge, TileTypeFire };
    const unsigned int LEVEL_EDITOR_NUMBER_OF_TILES = 8;

    const unsigned int LEVEL_EDITOR_TOTAL_BUTTONS = LEVEL_EDITOR_NUMBER_OF_TILES + 4;
    const string LEVEL_EDITOR_SPAWN_POINT_ATLAS_KEYS[] = { "Link-Spawn", "Enemy-Spawn" };
    const unsigned int LEVEL_EDITOR_SPAWN_POINT_VARIANT_COUNT = 2;
    const int LEVEL_EDITOR_SPAWN_POINT_INDEX = -2;

    //TODO: Polish EDITO PICK UPS
    /*const string LEVEL_EDITOR_PICKUP_ATLAS_KEYS[] = { "Heart-Full", "Rupee-Green","Rupee-Blue" };
    const unsigned int LEVEL_EDITOR_PICKUP_VARIANT_COUNT = 3;
    const int LEVEL_EDITOR_PICKUP_INDEX = -3;*/

    //const string LEVEL_EDITOR_BACKGROUND_MUSIC_FILE_NAMES[] = { "Fanfare", "Secret", "HeroDeath" };
    const string LEVEL_EDITOR_BACKGROUND_MUSIC_FILE_NAMES[] = { "EndingTheme", "DeathMountainTheme", "UnderworldTheme" };
    const unsigned int LEVEL_EDITOR_MAX_NUMBER_OF_MUSIC = 3;

    //Forward declarations
    class SubSectionMenu;
    class SubSection;
    class Button;
    class Sprite;
    class Random;
    class Audio;
    struct TileData;

    //The Level Editor edits one subsection at a time, it will automatically present the
    //SubSection menu when it is time to save and load a subsection.
    class LevelEditor : public Scene
    {
    public:
        LevelEditor();
        ~LevelEditor();

        float LoadContent();

        //Updates and Draw the Level Editor
        void Update(double delta);
        void Draw();

        //Handles mouse, keyboard and button events
        void HandleEvent(Event* event);

        //Returns the tile size and number of horizontal and vertical tiles in the level editor
        unsigned int GetTileSize();
        unsigned int GetNumberOfHorizontalTiles();
        unsigned int GetNumberOfVerticalTiles();

    private:
        //Will present the SubSection menu and try to load subsection data
        void Load();

        //Will present the SubSection menu and try to save the edited subsection data
        void Save();

        //Will clear the level editor
        void Clear();

        //Conveniance method to load a button, its position is based off of the index that is passed in
        Button* LoadButton(unsigned int index, const string& text, const string& atlasKey, bool isToggle);

        //Changes the tile's tile type at the position to the currently selected tile and variant
        void ChangeTileType(TileType tileType, vec2 position);

        //Member variables
        SubSectionMenu* m_SubSectionMenu;
        SubSection* m_EditingSubSection;
        Sprite* m_SelectedTile;
        TileData** m_TileData;
        Button* m_SpawnPointButton;
        Button* m_SaveButton;
        Button* m_LoadButton;
        Button* m_ClearButton;
        int m_SelectedIndex;
        unsigned int m_SpawnPointVariant;
        unsigned int m_PickUpVariant;
        unsigned int m_LoadStep;

        //Audio variables
        Audio* m_BackgroundMusic;
        void pickRandomMusic();
        Random* m_Random;//Used to randomly select music
        unsigned int m_MusicIndex;//Index current sound playing
    };

    //Conveniance struct to help organize the TileData (Button*, Tile* Variant)
    struct TileData
    {
        //Member variables
        Button* button;
        Tile* tile;
        unsigned int tileVariant;

        //Constructor
        TileData(Button* button, Tile* tile)
        {
            this->button = button;
            this->tile = tile;
            if (this->tile) this->tile->SetIsEnabled(false);
            this->tileVariant = 0;
        }

        //Destructor
        ~TileData()
        {
            //The tile object needs to be deleted, the button was added to the Scene so it doesn't need to be
            SafeDelete(tile);
        }

        //Increments the tile variant and sets the Button's background to that of the tile variant
        void IncrementVariant()
        {
            //Ensure there is at least 2 variants for the tile
            if (tile->GetVariantCount() > 1)
            {
                //Increment the tile variant, bounds check it
                tileVariant++;
                if (tileVariant >= tile->GetVariantCount())
                {
                    tileVariant = 0;
                }

                //Get the atlas key for the tile variant
                string atlasKey;
                tile->GetAtlasKeyForVariant(tileVariant, atlasKey);

                //Set the button's background to that of the tile variant
                button->SetBackground("MainAtlas", atlasKey);
            }
        }
    };
}

#endif /* defined(__GameDev2D__LevelEditor__) */
