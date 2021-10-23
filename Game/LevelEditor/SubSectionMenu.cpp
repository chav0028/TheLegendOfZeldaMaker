//
//  SubSectionMenu.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-17.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

/*
ChangeLog
_______________________________________________
-In Handle Event, Button Action case, modified the string being saved
to include the "SaveData/" path so that the files are directly saved to that
folder.THIS REQUIRED MODIFYING THE PROJECT PROERTIES, WORKING DIRECTORY TO BE T
HE ROOT OF THE PROJECT "../../../"
*/

#include "SubSectionMenu.h"
#include "LevelEditor.h"
#include "../World.h"
#include "../SubSection.h"
#include "../../Source/Platforms/PlatformLayer.h"
#include "../../Source/Services/ServiceLocator.h"
#include "../../Source/UI/UI.h"
#include "../../Source/Events/Input/InputEvents.h"


namespace GameDev2D
{
    SubSectionMenu::SubSectionMenu(LevelEditor* aLevelEditor) : Scene("SubSectionMenu"),
        m_LevelEditor(aLevelEditor),
        m_Overlay(nullptr),
        m_Buttons(nullptr),
        m_SaveSubSection(nullptr),
        m_LoadSubSection(nullptr)
    {

    }

    SubSectionMenu::~SubSectionMenu()
    {
        //Delete the buttons array, the button obejcts will be delete by the Scene
        SafeDeleteArray(m_Buttons);
    }

    float SubSectionMenu::LoadContent()
    {
        //Get the screen width and height
        float width = (float)ServiceLocator::GetPlatformLayer()->GetWidth();
        float height = (float)ServiceLocator::GetPlatformLayer()->GetHeight();

        //Create a overlay
        m_Overlay = new Rect(0.0f, 0.0f, width, height);
        m_Overlay->SetColor(Color::OnyxColor(0.0f));
        AddGameObject(m_Overlay);

        //Determine how many buttons we need and allocate an array to keep track of the buttons
        const unsigned int buttonCount = WORLD_NUMBER_OF_SUBSECTIONS.x * WORLD_NUMBER_OF_SUBSECTIONS.y;
        m_Buttons = new Button*[buttonCount];

        //Calculate the available width and height for an inidivual button
        float buttonWidth = width / WORLD_NUMBER_OF_SUBSECTIONS.x;
        float buttonHeight = height / WORLD_NUMBER_OF_SUBSECTIONS.y;

        //Calculate a small spacer between the buttons
        vec2 buttonSpacer = vec2(buttonWidth / 8, buttonHeight / 8);

        //Re-calculate the button width and height, factoring in the spacer
        buttonWidth = (width - (buttonSpacer.x * (WORLD_NUMBER_OF_SUBSECTIONS.x + 1))) / WORLD_NUMBER_OF_SUBSECTIONS.x;
        buttonHeight = (height - (buttonSpacer.y * (WORLD_NUMBER_OF_SUBSECTIONS.y + 1))) / WORLD_NUMBER_OF_SUBSECTIONS.y;

        //Set the button size vector
        vec2 buttonSize = vec2(buttonWidth, buttonHeight);

        //The string stream will be used below to see each's button label text
        stringstream ss;

        //Cycle through the buttonCount and create a new button object
        for (unsigned int i = 0; i < buttonCount; i++)
        {
            //Calculate the coordinates based on the button index
            uvec2 coordinates = uvec2(0, 0);
            coordinates.x = (i % WORLD_NUMBER_OF_SUBSECTIONS.x);
            coordinates.y = ((i - coordinates.x) / WORLD_NUMBER_OF_SUBSECTIONS.x);

            //Use those coordinates to set the button label
            ss.str("");
            ss << "(" << coordinates.x << "," << coordinates.y << ")";

            //Create the button and initialize its properties
            m_Buttons[i] = new Button("Ubuntu-B", "ttf", 48);
            m_Buttons[i]->SetText(ss.str());
            m_Buttons[i]->SetBackgroundColor(Color::CornflowerBlueColor(), ButtonStateNormal);
            m_Buttons[i]->SetBackgroundColor(Color::CornflowerBlueColor(), ButtonStateSelected);
            m_Buttons[i]->SetBackgroundColor(Color::CornflowerBlueColor(), ButtonStateDisabled);
            m_Buttons[i]->SetTextColor(Color::WhiteColor(), ButtonStateNormal);
            m_Buttons[i]->SetTextColor(Color::WhiteColor(), ButtonStateSelected);
            m_Buttons[i]->SetTextColor(Color::WhiteColor(), ButtonStateDisabled);
            m_Buttons[i]->SetPreferredSize(buttonSize);
            m_Buttons[i]->SetAnchorPoint(0.5f, 0.5f);
            m_Buttons[i]->AddEventListener(this, BUTTON_ACTION);

            //Calculate the button's exact position and set it
            float buttonX = buttonSpacer.x + (buttonSize.x / 2.0f) + (buttonSpacer.x + buttonSize.x) * coordinates.x;
            float buttonY = buttonSpacer.y + (buttonSize.y / 2.0f) + (buttonSpacer.y + buttonSize.y) * coordinates.y;
            m_Buttons[i]->SetLocalPosition(buttonX, buttonY);

            //Add the button object to the scene
            AddGameObject(m_Buttons[i]);
        }

        return 1.0f;
    }

    void SubSectionMenu::Draw()
    {
        //Draw the level editor in the background
        if (m_LevelEditor != nullptr)
        {
            m_LevelEditor->Draw();
        }

        Scene::Draw();
    }

    void SubSectionMenu::HandleEvent(Event* aEvent)
    {
        switch (aEvent->GetEventCode())
        {
        case BUTTON_ACTION:
        {
            //If the scene isn't active, then we want to ignore all button action events
            if (ServiceLocator::GetSceneManager()->IsActiveScene(this) == false)
            {
                return;
            }

            //Get the button pointer from the event data
            Button* button = (Button*)aEvent->GetEventData();

            int index = -1;

            //Determine the index of the button that was pressed
            const unsigned int buttonCount = WORLD_NUMBER_OF_SUBSECTIONS.x * WORLD_NUMBER_OF_SUBSECTIONS.y;
            for (unsigned int i = 0; i < buttonCount; i++)
            {
                if (button == m_Buttons[i])
                {
                    index = i;
                    break;
                }
            }

            //Safety check the index
            if (index != -1)
            {
                //Check the coordinates for the
                uvec2 coordinates = uvec2(0, 0);
                coordinates.x = (index % WORLD_NUMBER_OF_SUBSECTIONS.x);
                coordinates.y = ((index - coordinates.x) / WORLD_NUMBER_OF_SUBSECTIONS.x);

                //Set the filename, based on the button's coordinates
                stringstream ss;
                ss << "SaveData/SubSection" << coordinates.x << "-" << coordinates.y << ".bin";

                //Are we saving OR loading?
                if (m_SaveSubSection != nullptr)
                {
                    m_SaveSubSection->Save(ss.str());
                    m_SaveSubSection = nullptr;
                }
                else if (m_LoadSubSection != nullptr)
                {
                    m_LoadSubSection->Load(ss.str());
                    m_LoadSubSection = nullptr;
                }

                //We the save OR load operation is done, pop the sub-section menu
                ServiceLocator::GetSceneManager()->Pop();
            }
        }
        break;

        case KEYBOARD_EVENT:
        {
            KeyEvent* keyEvent = (KeyEvent*)aEvent;
            if (keyEvent->GetKeyEventType() == KeyUp)
            {
                //If the escape key is pressed, pop the sub-section menu
                if (keyEvent->GetKeyCode() == KEY_CODE_ESCAPE)
                {
                    ServiceLocator::GetSceneManager()->Pop();
                }
            }
        }
        break;
        }
    }

    void SubSectionMenu::WillTransitionOn()
    {
        //While the transition is happening, disable the buttons, this prevents them from being pressed
        DisableButtons();

        //Fade in the partially transparent black overlay
        m_Overlay->SetAlpha(0.5f, SUBSECTION_MENU_TRANSITION_ON_DURATION);

        //Do a scale animation for all the buttons
        const unsigned int buttonCount = WORLD_NUMBER_OF_SUBSECTIONS.x * WORLD_NUMBER_OF_SUBSECTIONS.y;
        for (unsigned int i = 0; i < buttonCount; i++)
        {
            m_Buttons[i]->SetLocalScale(0.0f, 0.0f);
            m_Buttons[i]->SetLocalScale(1.0f, 1.0f, SUBSECTION_MENU_BUTTON_SCALE_DURATION, Back::Out);
        }

        //Setup a delay method to re-enable the button
        DelaySceneMethod(&SubSectionMenu::EnableButtons, SUBSECTION_MENU_TRANSITION_ON_DURATION);
    }

    double SubSectionMenu::WillTransitionOff()
    {
        //While the transition is happening, disable the buttons, this prevents them from being pressed
        DisableButtons();

        //Fade out the partially transparent black overlay
        m_Overlay->SetAlpha(0.0f, SUBSECTION_MENU_TRANSITION_OFF_DURATION);

        //Do a scale animation for all the buttons
        const unsigned int buttonCount = WORLD_NUMBER_OF_SUBSECTIONS.x * WORLD_NUMBER_OF_SUBSECTIONS.y;
        for (unsigned int i = 0; i < buttonCount; i++)
        {
            m_Buttons[i]->SetLocalScale(0.0f, 0.0f, SUBSECTION_MENU_BUTTON_SCALE_DURATION, Back::In);
        }

        //Return the length of time this off-transition will take
        return SUBSECTION_MENU_TRANSITION_OFF_DURATION;
    }

    void SubSectionMenu::Save(SubSection* aSubSection)
    {
        //If this assert was hit, it means the subsection pointer that was passed in is null
        assert(aSubSection != nullptr);

        //Disable debug drawing - it will bog down the save operation
        aSubSection->DisableDebugDrawing();

        //Push the SubSection menu onto the screen
        ServiceLocator::GetSceneManager()->PushScene(this, false);

        //Set the subsection to be saved
        m_SaveSubSection = aSubSection;
    }

    void SubSectionMenu::Load(SubSection* aSubSection)
    {
        //If this assert was hit, it means the subsection pointer that was passed in is null
        assert(aSubSection != nullptr);

        //Disable debug drawing - it will bog down the load operation
        aSubSection->DisableDebugDrawing();

        //Push the SubSection menu onto the screen
        ServiceLocator::GetSceneManager()->PushScene(this, false);

        //Set the subsection to be loaded
        m_LoadSubSection = aSubSection;
    }

    void SubSectionMenu::EnableButtons()
    {
        const unsigned int buttonCount = WORLD_NUMBER_OF_SUBSECTIONS.x * WORLD_NUMBER_OF_SUBSECTIONS.y;
        for (unsigned int i = 0; i < buttonCount; i++)
        {
            m_Buttons[i]->SetState(ButtonStateNormal);
        }
    }

    void SubSectionMenu::DisableButtons()
    {
        const unsigned int buttonCount = WORLD_NUMBER_OF_SUBSECTIONS.x * WORLD_NUMBER_OF_SUBSECTIONS.y;
        for (unsigned int i = 0; i < buttonCount; i++)
        {
            m_Buttons[i]->SetState(ButtonStateDisabled);
        }
    }
}