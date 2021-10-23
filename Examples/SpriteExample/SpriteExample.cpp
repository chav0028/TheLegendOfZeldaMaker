//
//  SpriteExample.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-08-11.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "SpriteExample.h"
#include "../../Source/Services/ServiceLocator.h"
#include "../../Source/UI/Sprite/Sprite.h"
#include "../../Source/Events/Input/InputEvents.h"



namespace GameDev2D
{
    SpriteExample::SpriteExample() : Scene("SpriteExample"),
        m_AlgonquinLogo1(nullptr),
        m_AlgonquinLogo2(nullptr),
        m_AlgonquinAnimatedLogo(nullptr)
    {

    }
    
    SpriteExample::~SpriteExample()
    {
        //The Scene takes care of deleting any GameObjects, which includes our Sprite objects
    }
    
    float SpriteExample::LoadContent()
    {
        //Create a Sprite from a single png image called "AC-Logo"
        m_AlgonquinLogo1 = (Sprite*)AddGameObject(new Sprite("AC-Logo"));
        
        //Create a Sprite from a texture atlas, using the "5" atlas key.
        //We will go over in class how to build texture atlases in class using Texture Packer
        m_AlgonquinLogo2 = (Sprite*)AddGameObject(new Sprite("AC-Animated", "5"));
        
        //Create an animated Sprite from the "AC-Animated" png and json files generated from Texture Packer
        //This animation repeats and has a framerate of 12 frames per second
        m_AlgonquinAnimatedLogo = (Sprite*)AddGameObject(new Sprite("AC-Animated", "AC-Animated", true, 12.0f));
        
        //Reset all the sprites to their initial position, angle, scale
        Reset();
        
        //Return 1.0f when loading has completed
        return 1.0f;
    }
    
    void SpriteExample::HandleEvent(Event* aEvent)
    {
        switch (aEvent->GetEventCode())
        {
            case KEYBOARD_EVENT:
            {
                KeyEvent* keyEvent = (KeyEvent*)aEvent;
                if(keyEvent->GetKeyEventType() == KeyUp)
                {
                    //When the Space key is pressed the sprite's position and anchor point are animated for 1 second
                    if(keyEvent->GetKeyCode() == KEY_CODE_SPACE)
                    {
                        m_AlgonquinLogo1->SetLocalPosition(924.0f, 100.0f, 1.0);
                        m_AlgonquinLogo1->SetAnchorPoint(1.0f, 0.0f, 1.0);
                    }
                    
                    //When the Tab key is pressed the sprite's angle and scale are animated for 3 seconds
                    else if(keyEvent->GetKeyCode() == KEY_CODE_TAB)
                    {
                        m_AlgonquinLogo2->SetLocalAngle(270.0f, 3.0);
                        m_AlgonquinLogo2->SetLocalScale(1.5f, 1.5f, 3.0);
                    }
                    
                    //When the left Shift key is pressed the sprite's anchor points are animated for 2 seconds
                    else if(keyEvent->GetKeyCode() == KEY_CODE_LEFT_SHIFT)
                    {
                        m_AlgonquinAnimatedLogo->SetAnchorPoint(0.5f, 1.0f, 2.0);
                    }
                    
                    //When the right Shift key is pressed the sprite's anchor points are animated for 2 seconds
                    else if(keyEvent->GetKeyCode() == KEY_CODE_RIGHT_SHIFT)
                    {
                        m_AlgonquinAnimatedLogo->SetAnchorPoint(1.0f, 0.5f, 2.0);
                    }
                    
                    //When the A key is pressed fade the sprite's alpha to zero over 1 second
                    else if(keyEvent->GetKeyCode() == KEY_CODE_A)
                    {
                        m_AlgonquinAnimatedLogo->SetAlpha(0.0f, 1.0);
                    }
                    
                    //When the Backspace key is pressed reset all the sprites back to their initial position, angle, scale
                    else if(keyEvent->GetKeyCode() == KEY_CODE_BACKSPACE)
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
    
    void SpriteExample::Reset()
    {
        m_AlgonquinLogo1->SetLocalPosition(100.0f, 100.0f);
        m_AlgonquinLogo1->SetAnchorPoint(0.0f, 0.0f);
        m_AlgonquinLogo1->SetLocalAngle(0.0f);
        m_AlgonquinLogo1->SetLocalScale(1.0f, 1.0f);
        m_AlgonquinLogo1->SetAlpha(1.0f);
        
        m_AlgonquinLogo2->SetLocalPosition(100.0f, 300.0f);
        m_AlgonquinLogo2->SetAnchorPoint(0.0f, 0.0f);
        m_AlgonquinLogo2->SetLocalAngle(0.0f);
        m_AlgonquinLogo2->SetLocalScale(1.0f, 1.0f);
        m_AlgonquinLogo2->SetAlpha(1.0f);
        
        m_AlgonquinAnimatedLogo->SetLocalPosition(100.0f, 500.0f);
        m_AlgonquinAnimatedLogo->SetAnchorPoint(0.0f, 0.0f);
        m_AlgonquinAnimatedLogo->SetLocalAngle(0.0f);
        m_AlgonquinAnimatedLogo->SetLocalScale(1.0f, 1.0f);
        m_AlgonquinAnimatedLogo->SetAlpha(1.0f);
    }
}