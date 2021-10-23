//
//  Menu.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-08-27.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Menu__
#define __GameDev2D__Menu__

#include "../../Source/Core/Scene.h"


using namespace glm;

namespace GameDev2D
{
    //Local constants
    const float MENU_TITLE_FONT_SIZE_PCT = 0.09375f;
    const float MENU_BUTTON_FONT_SIZE_PCT = 0.0546875f;
    const vec2 MENU_TITLE_PCT = vec2(0.5f, 0.9609375f);
    const vec2 MENU_BUTTON_LIST_PCT = vec2(0.5f, 0.1302083333f);

    //Forward declarations
    class ButtonList;
    class Label;
    class Sprite;
    class Button;
    class Audio;

    //The Menu class is an abstract class that is designed to be the interface
    //for all menus in the game. Since it is abstract it MUST be inherited from.
    //It will manage the title, background sprite and buttons.
    class Menu : public Scene
    {
    public:
        Menu(const string& menu);
        virtual ~Menu();

        //Handles the Button Action event
        virtual void HandleEvent(Event* event);

        //Used to setup any menu on transitions
        virtual void WillTransitionOn();

        //Used to setup any menu off transitions
        virtual double WillTransitionOff();

        //Set's the title of the menu
        virtual void SetTitle(const string& title);

        //Set's the background sprite of the menu
        virtual void SetBackground(const string& background);

        //Adds a button to the menu
        virtual void AddButton(const string& button);


		//Functions to start and stop music
		void StartMusic();
		void StopMusic();

    protected:
        //Must be inheritted (pure virtual), is called when a button action event is recieved
        virtual void HandleButtonAction(unsigned int buttonIndex, const string& buttonText) = 0;

        //Member variables
        Label* m_Title;
        Sprite* m_Background;
        ButtonList* m_Buttons;

        Audio* m_MenuTheme;
    };
}

#endif /* defined(__GameDev2D__Menu__) */
