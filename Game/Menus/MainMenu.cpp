//
//  MainMenu.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-16.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "MainMenu.h"
#include "../Game.h"
#include "../LevelEditor/LevelEditor.h"
#include "../../Source/Services/ServiceLocator.h"
#include "../../Source/Platforms/PlatformLayer.h"


namespace GameDev2D
{
    MainMenu::MainMenu() : Menu("MainMenu")
    {

    }

    MainMenu::~MainMenu()
    {
    }

    float MainMenu::LoadContent()
    {
        //Set background
        SetBackground("MainMenuBackground");

        //SetTitle("Main Menu");//For asesthetic reasons, don't show title (game title will be shown in background)
        AddButton("Start Game");
        AddButton("Level Editor");
        AddButton("Exit");

        m_Buttons->SetNormalColor(Color::WhiteColor());
        m_Buttons->SetSelectionColor(Color::OrangeColor());

        //Load music
        m_MenuTheme = new Audio("TitleTheme", "mp3", true, true);//The music will start playing when we transition to the menu

        return 1.0f;
    }

    void MainMenu::WillTransitionOn()
    {
		//Prepare scene
		m_Background->SetAlpha(0);//Start with dark background
		m_MenuTheme->SetVolume(0);//Start with no sound

		//Start transition
		m_MenuTheme->SetVolume(1, MAIN_MENU_TRANSITION_IN_DURATION);//Fade in for music
		StartMusic();		
        m_Background->SetAlpha(1, MAIN_MENU_TRANSITION_IN_DURATION);//Fade out the background

        unsigned int numberButtons = m_Buttons->GetNumberOfButtons();//Get how many buttons are in the list
        for (unsigned int i = 0; i < numberButtons; i++)//Go through all the buttons in the button list
        {
            m_Buttons->GetButtonAtIndex(i)->GetLabel()->SetAlpha(1, MAIN_MENU_TRANSITION_IN_DURATION);//Fade out the labels of each button
        }
    }

    double MainMenu::WillTransitionOff()
    {
		m_MenuTheme->SetVolume(0, MAIN_MENU_TRANSITION_OFF_DURATION);//Fade off for music
		DelaySceneMethod(&Menu::StopMusic, MAIN_MENU_TRANSITION_OFF_DURATION);

        m_Background->SetAlpha(0, MAIN_MENU_TRANSITION_OFF_DURATION);//Fade out the background

        unsigned int numberButtons = m_Buttons->GetNumberOfButtons();//Get how many buttons are in the list
        for (unsigned int i = 0; i < numberButtons; i++)//Go through all the buttons in the button list
        {
            m_Buttons->GetButtonAtIndex(i)->GetLabel()->SetAlpha(0, MAIN_MENU_TRANSITION_OFF_DURATION);//Fade out the labels of each button
        }

        return MAIN_MENU_TRANSITION_OFF_DURATION;
    }

    void MainMenu::HandleButtonAction(unsigned int aButtonIndex, const string& aButtonText)
    {
        switch (aButtonIndex)
        {
        case MainMenuStart:
            ServiceLocator::GetSceneManager()->PushScene(new Game(), true);
            break;

        case MainMenuLevelEditor:
            ServiceLocator::GetSceneManager()->PushScene(new LevelEditor(), true);
            break;

        case MainMenuExit:
            ServiceLocator::GetPlatformLayer()->Shutdown();
            break;

        default:
            break;
        }
    }


}