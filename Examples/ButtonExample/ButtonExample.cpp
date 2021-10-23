//
//  ButtonExample.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-08-12.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "ButtonExample.h"


#include "../../Source/Services/ServiceLocator.h"
#include "../../Source/Platforms/PlatformLayer.h"
#include "../../Source/UI/Button/Button.h"
#include "../../Source/Events/Input/InputEvents.h"



namespace GameDev2D
{
    ButtonExample::ButtonExample() : Scene("ButtonExample")
    {
        //Initialize the button pointers to null
        for(int i = 0; i < BUTTON_COUNT; i++)
        {
            m_Buttons[i] = nullptr;
        }
    }
    
    ButtonExample::~ButtonExample()
    {
        //The Scene takes care of deleting any GameObjects, which includes our Button objects
    }
    
    float ButtonExample::LoadContent()
    {
        //Cycle through and create the buttons and set the default properties
        for(int i = 0; i < BUTTON_COUNT; i++)
        {
            m_Buttons[i] = (Button*)AddGameObject(new Button("CaviarDreams", "ttf", 60));
            m_Buttons[i]->AddEventListener(this, BUTTON_ACTION);
            m_Buttons[i]->SetText(BUTTON_LABELS[i]);
            m_Buttons[i]->SetAnchorY(0.5f);
            m_Buttons[i]->SetTextColor(Color::BlackColor(), ButtonStateNormal);
            m_Buttons[i]->SetTextColor(Color::WhiteColor(), ButtonStateSelected);
            m_Buttons[i]->SetOutlineColor(Color::BlackColor(), ButtonStateNormal);
            m_Buttons[i]->SetOutlineColor(Color::WhiteColor(), ButtonStateSelected);
        }
        
        //Reset all the sprites to their initial position
        Reset();
        
        //Return 1.0f when loading has completed
        return 1.0f;
    }
    
    void ButtonExample::HandleEvent(Event* aEvent)
    {
        switch (aEvent->GetEventCode())
        {
            case BUTTON_ACTION:
            {
                //Cast the event data to a button pointer and animate the x position of the button off screen
                Button* button = (Button*)aEvent->GetEventData();
                button->SetLocalX((float)ServiceLocator::GetPlatformLayer()->GetWidth(), 0.5);
            }
            break;
        
            case KEYBOARD_EVENT:
            {
                KeyEvent* keyEvent = (KeyEvent*)aEvent;
                if(keyEvent->GetKeyEventType() == KeyUp)
                {
                    //When the Backspace key is pressed reset all the buttons back to their initial position
                    if(keyEvent->GetKeyCode() == KEY_CODE_BACKSPACE)
                    {
                        Reset();
                    }
                }
            }
            break;
                
            default:
                break;
        }
    }
    
    void ButtonExample::Reset()
    {
        //Set the initial button position
        vec2 position = vec2(100.0f, 520.0f);
        float padding = 12.0f;
        
        //Cycle through and set the button positions
        for(int i = 0; i < BUTTON_COUNT; i++)
        {
            m_Buttons[i]->SetLocalPosition(position);
            position.y -= m_Buttons[i]->GetSize().y + padding;
        }
    }
}