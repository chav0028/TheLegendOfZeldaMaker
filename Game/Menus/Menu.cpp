//
//  Menu.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-08-27.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Menu.h"
#include "../../Source/Services/ServiceLocator.h"
#include "../../Source/Platforms/PlatformLayer.h"
#include "../../Source/Audio/Audio.h"
#include "../../Source/Graphics/Graphics.h"
#include "../../Source/UI/UI.h"
#include "../../Source/Events/Input/InputEvents.h"

namespace GameDev2D
{
    Menu::Menu(const string& aMenu) : Scene(aMenu),
        m_Title(nullptr),
        m_Background(nullptr),
        m_Buttons(nullptr),
        m_MenuTheme(nullptr)
    {

    }

    Menu::~Menu()
    {
        //The base class (Scene) will delete all the GameObjects added to the Scene
        SafeDelete(m_MenuTheme);//Delete the audio
    }

    void Menu::HandleEvent(Event* aEvent)
    {
        switch (aEvent->GetEventCode())
        {
        case BUTTON_ACTION:
        {
            //
            if (ServiceLocator::GetSceneManager()->IsActiveScene(this))
            {
                //
                Button* button = (Button*)aEvent->GetEventData();
                int index = m_Buttons->GetIndexForButton(button);

                //Safety check the index
                if (index != -1)
                {
                    HandleButtonAction(index, button->GetText());
                }
            }
        }
        break;

        default:
            break;
        }
    }

    void Menu::WillTransitionOn()
    {
        //TODO: Add any custom menu transition code here
        StartMusic();//Start the music
    }

    double Menu::WillTransitionOff()
    {
        //TODO: Add any custom menu transition code here
        StopMusic();//Stop the music

        return 0.0f;
    }

    void Menu::SetTitle(const string& aTitle)
    {
        //Get the width and height of the screen
        float screenWidth = (float)ServiceLocator::GetPlatformLayer()->GetWidth();
        float screenHeight = (float)ServiceLocator::GetPlatformLayer()->GetHeight();

        //If the title Label object hasn't been created, create one
        if (m_Title == nullptr)
        {
            m_Title = new Label("slkscr", "ttf", (unsigned int)(screenHeight * MENU_TITLE_FONT_SIZE_PCT));
            AddGameObject(m_Title);
        }

        //Set the title's text
        m_Title->SetText(aTitle);

        //Set the title's anchor point
        m_Title->SetAnchorPoint(0.5f, 1.0f);

        //Set the title's position
        m_Title->SetLocalPosition(screenWidth * MENU_TITLE_PCT.x, screenHeight * MENU_TITLE_PCT.y);

        //Set the title's font color
        m_Title->SetColor(Color::BlackColor());
    }

    void Menu::SetBackground(const string& aBackground)
    {
        //Safety check that a background wasn't already set
        if (m_Background != nullptr)
        {
            RemoveGameObject(m_Background);
            m_Background = nullptr;
        }

        //If the title Label object hasn't been created, create one
        if (m_Background == nullptr)
        {
            m_Background = new Sprite(aBackground);
            AddGameObject(m_Background);
        }
    }

    void Menu::AddButton(const string& aButton)
    {
        //Get the screen width and height
        float screenWidth = (float)ServiceLocator::GetPlatformLayer()->GetWidth();
        float screenHeight = (float)ServiceLocator::GetPlatformLayer()->GetHeight();

        //Have we created a ButtonList yet?
        if (m_Buttons == nullptr)
        {
            m_Buttons = new ButtonList("slkscr", "ttf", (unsigned int)(screenHeight * MENU_BUTTON_FONT_SIZE_PCT));
            AddGameObject(m_Buttons);
        }

        //Add the button to the ButtonList
        m_Buttons->AddButton(this, aButton);

        //Calculate the ButtonList's position based on the constant
        float x = screenWidth * MENU_BUTTON_LIST_PCT.x;
        float y = screenHeight * MENU_BUTTON_LIST_PCT.y;
        m_Buttons->SetLocalPosition(x, y);

        //Reset the ButtonList
        m_Buttons->Reset();
    }


    void Menu::StartMusic()
    {
        if (m_MenuTheme != nullptr)//If we have music
        {
            m_MenuTheme->Play();//Play it
        }
    }

    void Menu::StopMusic()
    {
        if (m_MenuTheme != nullptr)//If we have music
        {
            m_MenuTheme->Stop();//stop it
        }
    }
}