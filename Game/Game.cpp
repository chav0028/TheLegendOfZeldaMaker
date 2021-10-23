//
//  Game.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

/*
Polish:
_______________________________________________

ASSIGNMENT3:
-Added all the variants for the 5 tiles required in a single person project.
-Added additionally the tiles, "sand" and "bridge".
-Level designed to reward exploration. The easier path in the beginning will result
a harder end result and vice versa.	Note: That the level may change (and make more sense)
once enemies and pick ups are implement
-Added an image of the level layout in the project folder.
-The levels are now being saved and loaded directly to the "SaveData" folder

ASSIGNMENT 4:
-Added pause option in game when "p" is pressed.
-Added option to quit game when pressing "Escape"
-Added background image in main menu (removed menu title label for aestethic reasons)
-Added an animated fire tile
-Projectiles are made in a pool used object pooling, rather than being deleted when they go offscreen.
-Music is played in the main menu.
-A random song is played in the level editor, with a new song being chosen after the current song has ended.
-When the hero dies, the user is taken back to main menu.
-When the user has full health, he is able to throw a projectile (only a single one, he can't launch another one while his is on the screen) that
collides and kills enemies.
-The projectiles don't go through non-walkable tiles
-Added transitions in and transition out for the main menu.
-Added flickering to the hero when he gets hit.
-A sound is played when the hero gets hit.
*/


/*
ChangeLog
_______________________________________________
-Added game paused variable, so that the game doesn't  update when the variable is true.
-Hero handle events is now called in here
*/

#include "Game.h"
#include "World.h"
#include "SubSection.h"
#include "HeadsUpDisplay.h"
#include "../Source/Services/ServiceLocator.h"
#include "../Source/Platforms/PlatformLayer.h"
#include "../Source/Audio/Audio.h"
#include "../Source/UI/UI.h"
#include "../Source/Graphics/Graphics.h"
#include "../Source/Events/Input/InputEvents.h"
#include "World.h"
#include "Player/Hero.h"

namespace GameDev2D
{
    Game::Game() : Scene("Game"),
        m_World(nullptr),
        m_HeadsUpDisplay(nullptr),
        m_GamePaused(false)
    {

    }

    Game::~Game()
    {
        SafeDelete(m_World);
        SafeDelete(m_HeadsUpDisplay);
    }

    float Game::LoadContent()
    {
        //Create the World object
        if (m_World == nullptr)
        {
            m_World = new World();
        }

        //Create the heads up display object
        if (m_HeadsUpDisplay == nullptr)
        {
            m_HeadsUpDisplay = new HeadsUpDisplay(m_World);
            ServiceLocator::GetSceneManager()->PreloadScene(m_HeadsUpDisplay);
        }

        //Return the percentage loaded
        return m_World->LoadContent();
    }

    void Game::Update(double aDelta)
    {
        if (m_GamePaused == false)//Only update the game if it isn't paused
        {
            //Update the world
            if (m_World != nullptr)
            {
                m_World->Update(aDelta);
            }

            //Update the heads up display
            if (m_HeadsUpDisplay != nullptr)
            {
                m_HeadsUpDisplay->Update(aDelta);
            }

            Scene::Update(aDelta);
        }
    }

    void Game::Draw()
    {
        //Draw the world
        if (m_World != nullptr)
        {
            ServiceLocator::GetSceneManager()->DrawScene(m_World);
        }

        //Draw the Heads up display
        if (m_HeadsUpDisplay != nullptr)
        {
            ServiceLocator::GetSceneManager()->DrawScene(m_HeadsUpDisplay);
        }

        Scene::Draw();
    }

    void Game::HandleEvent(Event* aEvent)
    {
        //Handle the events for the player, not added the player as a listener, so that no input is handled while the game is paused
        if (m_GamePaused == false) //If the game is not paused
        {
            if (m_World != nullptr)
            {
                Hero* hero = m_World->GetHero();//Get the hero from the world
                if (hero != nullptr)
                {
                    hero->HandleEvent(aEvent);//Handle player input
                }
            }
        }

        switch (aEvent->GetEventCode())
        {
        case MOUSE_CLICK_EVENT:
        {

        }
        break;

        case KEYBOARD_EVENT:
        {

            //Handle keyboard to enable debug drawing
            KeyEvent* keyEvent = (KeyEvent*)aEvent;
            if (keyEvent->GetKeyEventType() == KeyUp)
            {
                //If the user pressed "Escape", check if he wants to quit the game
                if (keyEvent->GetKeyCode() == KEY_CODE_ESCAPE)
                {
                    //Pause the game while the user confirms choice
                    PauseGame(true);

                    //Present a native dialog box, confirming that the user intended to exit the game
                    if (ServiceLocator::GetPlatformLayer()->PresentNativeDialogBox("Alert", "Are you sure you want to exit the Game? Any unsaved data will be lost.", NativeDialogYesNo) == 0)
                    {
                        ServiceLocator::GetSceneManager()->Pop();//Exit the game
                        break;//Break, so that we don't process any of the code below (preventing problems when we try to get the active subsectin)
                    }
                    else
                    {
                        //If the user didn't quit the game, unpause it
                        PauseGame(false);
                    }


                }
                else if (keyEvent->GetKeyCode() == KEY_CODE_P)//If the user pressed P, pause the game
                {
                    PauseGame(!m_GamePaused);//Invert the current value of the paused game
                }

#if DEBUG
                if (m_World != nullptr)
                {
                    if (m_World->GetActiveSubSection() != nullptr)
                    {
                        if (keyEvent->GetKeyCode() == KEY_CODE_0)
                        {
                            m_World->GetActiveSubSection()->DisableDebugDrawing();
                        }
                        else if (keyEvent->GetKeyCode() == KEY_CODE_1)
                        {
                            m_World->GetActiveSubSection()->EnableDebugDrawing(m_World->GetActiveSubSection()->GetDebugDrawFlags() ^ DebugDrawTileIndex);
                        }
                        else if (keyEvent->GetKeyCode() == KEY_CODE_2)
                        {
                            m_World->GetActiveSubSection()->EnableDebugDrawing(m_World->GetActiveSubSection()->GetDebugDrawFlags() ^ DebugDrawTileCoordinates);
                        }
                        else if (keyEvent->GetKeyCode() == KEY_CODE_3)
                        {
                            m_World->GetActiveSubSection()->EnableDebugDrawing(m_World->GetActiveSubSection()->GetDebugDrawFlags() ^ DebugDrawTilePositionLocal);
                        }
                        else if (keyEvent->GetKeyCode() == KEY_CODE_4)
                        {
                            m_World->GetActiveSubSection()->EnableDebugDrawing(m_World->GetActiveSubSection()->GetDebugDrawFlags() ^ DebugDrawTilePositionWorld);
                        }
                        else if (keyEvent->GetKeyCode() == KEY_CODE_5)
                        {
                            m_World->GetActiveSubSection()->EnableDebugDrawing(m_World->GetActiveSubSection()->GetDebugDrawFlags() ^ DebugDrawPathFindingScores);
                        }
                        else if (keyEvent->GetKeyCode() == KEY_CODE_6)
                        {
                            m_World->GetActiveSubSection()->EnableDebugDrawing(m_World->GetActiveSubSection()->GetDebugDrawFlags() ^ DebugDrawSubSectionIndex);
                        }
                        else if (keyEvent->GetKeyCode() == KEY_CODE_7)
                        {
                            m_World->GetActiveSubSection()->EnableDebugDrawing(m_World->GetActiveSubSection()->GetDebugDrawFlags() ^ DebugDrawSubSectionCoordinates);
                        }
                        else if (keyEvent->GetKeyCode() == KEY_CODE_8)
                        {
                            m_World->GetActiveSubSection()->EnableDebugDrawing(m_World->GetActiveSubSection()->GetDebugDrawFlags() ^ DebugDrawSubSectionPosition);
                        }
                    }
                }
            }
#endif
        }
        break;

        default:
            break;
        }
    }

    void Game::PauseGame(bool aPauseStatus)
    {


        //If we want to pause the game
        if (aPauseStatus == true)
        {
            m_GamePaused = true;//set the game as paused so that it doesn't update

            if (m_World != nullptr)
            {
                m_World->PauseMusic();//Pause the music
            }

        }
        else if (aPauseStatus == false)
        {
            m_GamePaused = false;//Set the game as active

            if (m_World != nullptr)
            {
                m_World->StartMusic();//Play the music
            }
        }

    }

    bool Game::GetIsGamePaused()
    {
        return m_GamePaused;
    }
}