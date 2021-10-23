//
//  CameraExample.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-08-12.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "CameraExample.h"
#include "../../Source/Services/ServiceLocator.h"
#include "../../Source/Platforms/PlatformLayer.h"
#include "../../Source/UI/Sprite/Sprite.h"
#include "../../Source/Graphics/Primitives/Primitives.h"
#include "../../Source/Events/Input/InputEvents.h"



namespace GameDev2D
{
    CameraExample::CameraExample() : Scene("CameraExample"),
        m_AlgonquinLogo(nullptr),
        m_BackgroundRect(nullptr)
    {

    }
    
    CameraExample::~CameraExample()
    {
    }
    
    float CameraExample::LoadContent()
    {
        //Local variables used for positioning
        float width = (float)ServiceLocator::GetPlatformLayer()->GetWidth();
        float height = (float)ServiceLocator::GetPlatformLayer()->GetHeight();
    
        //The Camera object is part of the Scene, so there it doesn't need to load itself,
        //but we'll load a Sprite and Rect as a frame of reference
        m_BackgroundRect = (Rect*)AddGameObject(new Rect(0.0f, 0.0f, width, height));
        m_BackgroundRect->SetColor(Color::WhiteColor());
        
        m_AlgonquinLogo = (Sprite*)AddGameObject(new Sprite("AC-Logo"));
        m_AlgonquinLogo->SetAnchorPoint(0.5f, 0.5f);
        m_AlgonquinLogo->SetLocalPosition(width / 2.0f, height / 2.0f);
        
        //Return 1.0f when loading has completed
        return 1.0f;
    }
    
    void CameraExample::HandleEvent(Event* aEvent)
    {
        switch (aEvent->GetEventCode())
        {
            case KEYBOARD_EVENT:
            {
                KeyEvent* keyEvent = (KeyEvent*)aEvent;
                if(keyEvent->GetKeyEventType() == KeyUp)
                {
                    //When the Space key is pressed perform a camera shake for 1 second
                    if(keyEvent->GetKeyCode() == KEY_CODE_SPACE)
                    {
                        GetCamera()->Shake(25.0f, 1.0);
                    }

                    //When the Tab key is pressed the rotate the camera 90 degrees
                    else if(keyEvent->GetKeyCode() == KEY_CODE_TAB)
                    {
                        GetCamera()->SetOrientation(GetCamera()->GetOrientation() + 90.0f, 5.0);
                    }

                    //When the left Shift key is pressed set the camera's zoom to 0.5f (zoom out)
                    else if(keyEvent->GetKeyCode() == KEY_CODE_LEFT_SHIFT)
                    {
                        GetCamera()->SetZoom(0.5f, 2.0);
                    }
                    
                    //When the right Shift key is pressed set the camera's zoom to 1.5f (zoom in)
                    else if(keyEvent->GetKeyCode() == KEY_CODE_RIGHT_SHIFT)
                    {
                        GetCamera()->SetZoom(1.5f, 2.0);
                    }
                    
                    //When the left Arrow key is pressed shift the camera 50 pixels to the right
                    else if(keyEvent->GetKeyCode() == KEY_CODE_LEFT)
                    {
                        GetCamera()->SetPositionX(GetCamera()->GetPosition().x - 50.0f, 0.1);
                    }
                    
                    //When the right Arrow key is pressed shift the camera 50 pixels to the left
                    else if(keyEvent->GetKeyCode() == KEY_CODE_RIGHT)
                    {
                        GetCamera()->SetPositionX(GetCamera()->GetPosition().x + 50.0f, 0.1);
                    }
                    
                    //When the up Arrow key is pressed shift the camera 50 pixels down
                    else if(keyEvent->GetKeyCode() == KEY_CODE_UP)
                    {
                        GetCamera()->SetPositionY(GetCamera()->GetPosition().y + 50.0f, 0.1);
                    }
                    
                    //When the down Arrow key is pressed shift the camera 50 pixels up
                    else if(keyEvent->GetKeyCode() == KEY_CODE_DOWN)
                    {
                        GetCamera()->SetPositionY(GetCamera()->GetPosition().y - 50.0f, 0.1);
                    }

                    //When the Backspace key is pressed reset all the labels back
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
    
    void CameraExample::Reset()
    {
        GetCamera()->SetOrientation(0.0f);
        GetCamera()->SetPosition(0.0f, 0.0f);
        GetCamera()->SetZoom(1.0f);
    }
}