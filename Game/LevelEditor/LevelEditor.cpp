//
//  LevelEditor.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-14.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

/*
ChangeLog
_______________________________________________
-Added  background music to LevelEditor.cpp.
-A random song is played when the Level Editor is started.
-When quitting level editor the game will now push a new main menu instead of just popping te previous one. Since this preventd the animations from playing.
*/

#include "LevelEditor.h"
#include "SubSectionMenu.h"
#include "../SubSection.h"
#include "../World.h"
#include "../../Source/Services/ServiceLocator.h"
#include "../../Source/Platforms/PlatformLayer.h"
#include "../../Source/Events/Input/InputEvents.h"
#include "../../Source/Audio/Audio.h"
#include "../Pickups/HeartPickup.h"
#include "../Pickups/GreenRupeePickup.h"
#include "../Pickups/BlueRupeePickup.h"
#include "../Menus/MainMenu.h"

namespace GameDev2D
{
    LevelEditor::LevelEditor() : Scene("LevelEditor"),
        m_SubSectionMenu(nullptr),
        m_EditingSubSection(nullptr),
        m_SelectedTile(nullptr),
        m_TileData(nullptr),
        m_SpawnPointButton(nullptr),
        m_SaveButton(nullptr),
        m_LoadButton(nullptr),
        m_ClearButton(nullptr),
        m_SelectedIndex(-1),
        m_SpawnPointVariant(0),
        m_PickUpVariant(0),
        m_LoadStep(0),
        m_BackgroundMusic(nullptr),
        m_Random(nullptr),
        m_MusicIndex(-1)//Start to -1, in that way we can pick a random song, and ensure that the same song isn't picked twice (if it was initialized at 0, we could never start with song at 0
    {

    }

    LevelEditor::~LevelEditor()
    {
        //Delete the tile data
        if (m_TileData != nullptr)
        {
            for (unsigned int i = 0; i < LEVEL_EDITOR_NUMBER_OF_TILES; i++)
            {
                SafeDelete(m_TileData[i]->tile);
                SafeDelete(m_TileData[i]);
            }
            SafeDeleteArray(m_TileData);
        }

        SafeDelete(m_SubSectionMenu);
        SafeDelete(m_EditingSubSection);

        //Delete audio variables
        SafeDelete(m_BackgroundMusic);
        SafeDelete(m_Random);
    }

    float LevelEditor::LoadContent()
    {
        switch (m_LoadStep)
        {
        case LevelEditorLoadRequired:
        {
            //Get the screen width and height
            float screenWidth = (float)ServiceLocator::GetPlatformLayer()->GetWidth();
            float screenHeight = (float)ServiceLocator::GetPlatformLayer()->GetHeight();
            float y = (float)(GetTileSize() * GetNumberOfVerticalTiles());
            float height = screenHeight - y;

            //Load a black background for the top part of the Level Editor
            Rect* rect = new Rect(0.0f, y, screenWidth, height);
            rect->SetColor(Color::OnyxColor());
            AddGameObject(rect);
        }
        break;

        case LevelEditorLoadSelectionSprite:
        {
            //Load the selected tile reticle
            m_SelectedTile = new Sprite("MainAtlas", "SelectedTile");
            m_SelectedTile->SetAnchorPoint(0.5f, 0.5f);
            m_SelectedTile->SetIsEnabled(false);
            AddGameObject(m_SelectedTile);
        }
        break;

        case LevelEditorLoadSubSection:
        {
            //Load the sub-section - by default an empty subsection is loaded
            m_EditingSubSection = new SubSection(this, "");
        }
        break;

        case LevelEditorLoadButtons:
        {
            //Create an array to hold all the tile data needed for the level editor
            m_TileData = new TileData*[LEVEL_EDITOR_NUMBER_OF_TILES];

            //Cycle through and load the Level Editor buttons
            for (unsigned int i = 0; i < LEVEL_EDITOR_NUMBER_OF_TILES; i++)
            {
                //Create a tile for the tile type
                Tile* tile = m_EditingSubSection->CreateTile(LEVEL_EDITOR_TILE_TYPES[i], 0, uvec2(0, 0));

                //Get the atlas key for the button's image
                string atlasKey = "";
                if (tile != nullptr)
                {
                    tile->GetAtlasKeyForVariant(0, atlasKey);
                }

                //Load a button with the atlas key, and the current label text
                Button* button = LoadButton(i, LEVEL_EDITOR_TILE_BUTTON_TEXT[i], atlasKey, true);

                //Create a Tile Data object, with the button and tile
                m_TileData[i] = new TileData(button, tile);
            }

            //Load the 'utility' buttons
            m_SpawnPointButton = LoadButton(LEVEL_EDITOR_NUMBER_OF_TILES, "Spawn", LEVEL_EDITOR_SPAWN_POINT_ATLAS_KEYS[0], true);
            m_SaveButton = LoadButton(LEVEL_EDITOR_NUMBER_OF_TILES + 1, "Save", "ButtonOutline", false);
            m_LoadButton = LoadButton(LEVEL_EDITOR_NUMBER_OF_TILES + 2, "Load", "ButtonOutline", false);
            m_ClearButton = LoadButton(LEVEL_EDITOR_NUMBER_OF_TILES + 3, "Clear", "ButtonOutline", false);
        }
        break;

        case LevelEditorLoadSubSectionMenu:
        {
            //Load the subsection menu and preload it
            m_SubSectionMenu = new SubSectionMenu(this);
            ServiceLocator::GetSceneManager()->PreloadScene(m_SubSectionMenu);
        }
        break;

        case LevelEditorLoadMusic:
        {
            //Create the random number generator object, for random music
            m_Random = new Random();
            m_Random->RandomizeSeed();

            //Pick a random music to play
            pickRandomMusic();
        }
        break;

        default:
            break;
        }

        //Increment the load step
        m_LoadStep++;

        //Return the percentage loaded
        return (float)m_LoadStep / (float)LevelEditorLoadStepCount;
    }

    void LevelEditor::Update(double aDelta)
    {
        //Check if we want to change music
        if (m_BackgroundMusic != nullptr)
        {
            if (m_BackgroundMusic->IsPlaying() == false)
            {
                pickRandomMusic();//Pick another song
            }

        }

        if (m_EditingSubSection != nullptr)
        {
            m_EditingSubSection->Update(aDelta);
        }
        Scene::Update(aDelta);
    }

    void LevelEditor::Draw()
    {
        if (m_EditingSubSection != nullptr)
        {
            m_EditingSubSection->Draw();
        }

        Scene::Draw();
    }

    void LevelEditor::HandleEvent(Event* aEvent)
    {
        switch (aEvent->GetEventCode())
        {
        case BUTTON_ACTION:
        {
            //Get the button pointer from the event data
            Button* button = (Button*)aEvent->GetEventData();

            //Handle the button action
            if (button == m_SaveButton)
            {
                Save();
            }
            else if (button == m_LoadButton)
            {
                Load();
            }
            else if (button == m_ClearButton)
            {
                Clear();
            }
            else
            {
                //De-select the previously selected button
                if (m_SelectedIndex >= 0)
                {
                    m_TileData[m_SelectedIndex]->button->SetState(ButtonStateNormal);
                }
                else if (m_SelectedIndex == LEVEL_EDITOR_SPAWN_POINT_INDEX)
                {
                    m_SpawnPointButton->SetState(ButtonStateNormal);
                }

                //Reset the selected button index
                m_SelectedIndex = -1;

                //Ensure the button is selected - we can get a button action event when a toggle-able button is de-selected as well
                if (button->GetState() == ButtonStateSelected)
                {
                    //Cycle through the tile-buttons and compare them to the button pointer,
                    //if a match is found then grab the selected button index
                    for (unsigned int i = 0; i < LEVEL_EDITOR_NUMBER_OF_TILES; i++)
                    {
                        if (button == m_TileData[i]->button)
                        {
                            m_SelectedIndex = i;
                            break;
                        }
                    }

                    //If the selected index is -1, then the spawn point button may be selected
                    if (m_SelectedIndex == -1 && button == m_SpawnPointButton)
                    {
                        m_SelectedIndex = LEVEL_EDITOR_SPAWN_POINT_INDEX;
                    }

                }
            }
        }
        break;

        case MOUSE_CLICK_EVENT:
        {
            MouseClickEvent* clickEvent = (MouseClickEvent*)aEvent;
            if (clickEvent->GetMouseClickEventType() == MouseClickDown && clickEvent->GetMouseButton() == MouseButtonLeft)
            {
                //Change the tile type at the mouse position
                if (m_SelectedIndex == 0)
                {
                    ChangeTileType(LEVEL_EDITOR_TILE_TYPES[m_SelectedIndex], clickEvent->GetPosition());
                }
                else if (m_SelectedIndex == LEVEL_EDITOR_SPAWN_POINT_INDEX)
                {
                    //Get the position of the click event
                    vec2 position = clickEvent->GetPosition();

                    //Ensure the mouse's position is within the level editor's tiles area
                    if (position.x >= 0.0f && position.x < GetNumberOfHorizontalTiles() * GetTileSize() && position.y >= 0.0f && position.y < GetNumberOfVerticalTiles() * GetTileSize())
                    {
                        //Get the tile for the position
                        Tile* tile = m_EditingSubSection->GetTileForPosition(position);

                        //Based on the current spawn point variant set the subsection spawn point at the mouse click location
                        if (tile->IsWalkable() == true)//Checks if the tile is walkable before spawning and object in it
                        {
                            if (m_SpawnPointVariant == 0)//Hero spawn
                            {
                                m_EditingSubSection->SetHeroSpawnPointForTile(tile);
                            }
                            else if (m_SpawnPointVariant == 1)//Enemy Spawn
                            {
                                //tile->SetIsEnemySpawnPoint(!tile->IsEnemySpawnPoint());
                                m_EditingSubSection->SetEnemySpawnPoint(tile);
                            }
                        }
                    }
                }

            }
        }
        break;

        case MOUSE_MOVEMENT_EVENT:
        {
            //Get the position of the mouse cursor
            MouseMovementEvent* moveEvent = (MouseMovementEvent*)aEvent;
            vec2 position = moveEvent->GetPosition();

            //Safety check the sub-section pointer
            if (m_EditingSubSection != nullptr)
            {
                //Safety check that the mouse is on screen
                if (position.x >= 0.0f && position.x < GetTileSize() * GetNumberOfHorizontalTiles() && position.y >= 0.0f && position.y < GetTileSize() * GetNumberOfVerticalTiles())
                {
                    //Get the Tile for the mouse cursor position and get the center of the Tile
                    Tile* tile = m_EditingSubSection->GetTileForPosition(position);
                    vec2 center = tile->GetCenter(true);

                    //Set the position of the selection tile sprite
                    m_SelectedTile->SetLocalPosition(center);
                    m_SelectedTile->SetIsEnabled(true);

                    //If the left mouse button is down then we can also change the tile type
                    bool isMouseDown = ServiceLocator::GetInputManager()->IsMouseButtonPressed(MouseButtonLeft);
                    if (isMouseDown == true)
                    {
                        if (m_SelectedIndex >= 0)
                        {
                            ChangeTileType(LEVEL_EDITOR_TILE_TYPES[m_SelectedIndex], position);
                        }
                    }
                }
                else
                {
                    //If the mouse cursor is off-screen, disable the selected tile sprite
                    m_SelectedTile->SetIsEnabled(false);
                }
            }
        }
        break;

        case KEYBOARD_EVENT:
        {
            KeyEvent* keyEvent = (KeyEvent*)aEvent;
            if (keyEvent->GetKeyEventType() == KeyUp)
            {
                if (keyEvent->GetKeyCode() == KEY_CODE_ESCAPE)
                {
                    //Present a native dialog box, confirming that the user intended to exit the Level Editor
                    if (ServiceLocator::GetPlatformLayer()->PresentNativeDialogBox("Alert", "Are you sure you want to exit the Level Editor? Any unsaved data will be lost.", NativeDialogYesNo) == 0)
                    {
                        //ServiceLocator::GetSceneManager()->Pop();//Doesn't play transition, if we only pop this screen
						m_BackgroundMusic->Stop();//Stop the music.
						ServiceLocator::GetSceneManager()->PushScene(new MainMenu(),true);//So add the new main menu again
                    }
                }
                else if (keyEvent->GetKeyCode() == KEY_CODE_TAB)
                {
                    if (m_SelectedIndex >= 0)
                    {
                        //Increment the regular tile variant
                        m_TileData[m_SelectedIndex]->IncrementVariant();
                    }
                    else if (m_SelectedIndex == LEVEL_EDITOR_SPAWN_POINT_INDEX)
                    {
                        //Increment the spawn point varaint
                        m_SpawnPointVariant++;
                        if (m_SpawnPointVariant >= LEVEL_EDITOR_SPAWN_POINT_VARIANT_COUNT)
                        {
                            m_SpawnPointVariant = 0;
                        }

                        //Set the spawn point button's image to that of the currently selected variant
                        m_SpawnPointButton->SetBackground("MainAtlas", LEVEL_EDITOR_SPAWN_POINT_ATLAS_KEYS[m_SpawnPointVariant]);
                    }

                }
                else if (keyEvent->GetKeyCode() == KEY_CODE_0)
                {
                    m_EditingSubSection->DisableDebugDrawing();
                }
                else if (keyEvent->GetKeyCode() == KEY_CODE_1)
                {
                    m_EditingSubSection->EnableDebugDrawing(m_EditingSubSection->GetDebugDrawFlags() ^ DebugDrawTileIndex);
                }
                else if (keyEvent->GetKeyCode() == KEY_CODE_2)
                {
                    m_EditingSubSection->EnableDebugDrawing(m_EditingSubSection->GetDebugDrawFlags() ^ DebugDrawTileCoordinates);
                }
                else if (keyEvent->GetKeyCode() == KEY_CODE_3)
                {
                    m_EditingSubSection->EnableDebugDrawing(m_EditingSubSection->GetDebugDrawFlags() ^ DebugDrawTilePositionLocal);
                }
            }
        }
        break;

        default:
            break;
        }
    }

    unsigned int LevelEditor::GetTileSize()
    {
        return WORLD_TILE_SIZE;
    }

    unsigned int LevelEditor::GetNumberOfHorizontalTiles()
    {
        return WORLD_NUMBER_OF_HORIZONTAL_TILES;
    }

    unsigned int LevelEditor::GetNumberOfVerticalTiles()
    {
        return WORLD_NUMBER_OF_VERTICAL_TILES;
    }

    void LevelEditor::Load()
    {
        if (m_SubSectionMenu != nullptr)
        {
            m_SubSectionMenu->Load(m_EditingSubSection);
        }
    }

    void LevelEditor::Save()
    {
        if (m_SubSectionMenu != nullptr)
        {
            m_SubSectionMenu->Save(m_EditingSubSection);
        }
    }

    void LevelEditor::Clear()
    {
        if (m_EditingSubSection != nullptr)//Check there is a valid subsection
        {
            for (unsigned int i = 0; i < m_EditingSubSection->GetNumberOfTiles(); i++)//Go through al lthe tiles in the subsection
            {
                m_EditingSubSection->SetTileTypeAtIndex(i, TileTypeGround, 0);//Set the tile to be the "common" ground type.
                m_EditingSubSection->DeactivateSpawnPointsAtTileIndex(i);//Ensure that the tile doesn't have any spawn point
                m_EditingSubSection->GetTileForIndex(i)->RemovePickup();//Remove any pick up from the tile
            }
        }
    }

    Button* LevelEditor::LoadButton(unsigned int aIndex, const string& aText, const string& aAtlasKey, bool aIsToggle)
    {
        //Create the button object and initialize its properties
        Button* button = new Button("slkscr", "ttf", 14);
        button->SetText(aText);
        button->SetBackground("MainAtlas", aAtlasKey);
        button->SetAnchorPoint(0.5f, 0.5f);
        button->SetIsToggled(aIsToggle);
        button->AddEventListener(this, BUTTON_ACTION);

        //Calculate the position of the button
        float screenWidth = (float)ServiceLocator::GetPlatformLayer()->GetWidth();
        float screenHeight = (float)ServiceLocator::GetPlatformLayer()->GetHeight();
        float totalWidth = LEVEL_EDITOR_TOTAL_BUTTONS * WORLD_TILE_SIZE;
        float availableWidth = screenWidth - totalWidth;
        float spacerX = availableWidth / (LEVEL_EDITOR_TOTAL_BUTTONS + 1);
        float x = spacerX + (WORLD_TILE_SIZE / 2.0f) + ((spacerX + WORLD_TILE_SIZE) * aIndex);
        float y = (GetTileSize() * GetNumberOfVerticalTiles()) + (screenHeight - (GetTileSize() * GetNumberOfVerticalTiles())) / 2.0f;
        button->SetLocalPosition(x, y);

        //Add the button to the Scene
        return (Button*)AddGameObject(button);
    }

    void LevelEditor::ChangeTileType(TileType aTileType, vec2 aPosition)
    {
        //Safety check the sub-section pointer
        if (m_EditingSubSection != nullptr)
        {
            //Get the tile index for the position
            unsigned int tileIndex = m_EditingSubSection->GetTileIndexForPosition(aPosition);

            //Get the tile variant
            unsigned int tileVariant = m_SelectedIndex >= 0 ? m_TileData[m_SelectedIndex]->tileVariant : 0;

            //Set the tile type at the tile index
            m_EditingSubSection->SetTileTypeAtIndex(tileIndex, aTileType, tileVariant);
        }
    }
    void LevelEditor::pickRandomMusic()
    {
        if (m_Random != nullptr)//If we can generate random numbers
        {
            unsigned int previousMusicIndex = m_MusicIndex;//Save the index of the previous song

            //Loop until we have picked a different song than the one we had before
            do
            {
                m_MusicIndex = m_Random->RandomRange(0, LEVEL_EDITOR_MAX_NUMBER_OF_MUSIC);//Get a number in the songs we have available.	
            } while (m_MusicIndex == previousMusicIndex);	 //While we have the same song as before


            //If we have music before
            if (m_BackgroundMusic != nullptr)
            {
                SafeDelete(m_BackgroundMusic);//Delete the old music
            }

            //Load music
            m_BackgroundMusic = new Audio(LEVEL_EDITOR_BACKGROUND_MUSIC_FILE_NAMES[m_MusicIndex], "mp3", true, false);//Don't loop, so that when the song is over we play another 


            //Play the music

            //The functions "IsPlaying" seems to always be set to true even if the length of the audio file has already been exceeded. Since we are not looping.
            //So to actually change the "IsPlaying" result when it gets called in the update method. We will set a Stop.
            //This will tell the audio file that it is no longer playing, and it will have a delay that is equal to the lenght of the audio file. So once all the audio
            //has played, we will tell it to stop so that it changes it isPlaying value.
            m_BackgroundMusic->Stop(m_BackgroundMusic->GetLength() / 1000.0);//Divide by 1000 to convert milliseconds to seconds

            m_BackgroundMusic->Play();
        }
    }
}