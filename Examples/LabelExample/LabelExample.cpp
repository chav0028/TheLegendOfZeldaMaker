//
//  LabelExample.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-08-12.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "LabelExample.h"
#include "../../Source/Services/ServiceLocator.h"
#include "../../Source/UI/Label/Label.h"
#include "../../Source/Events/Input/InputEvents.h"



namespace GameDev2D
{
    LabelExample::LabelExample() : Scene("LabelExample"),
        m_BitmapLabel(nullptr),
        m_TrueTypeLabel(nullptr)
    {

    }
    
    LabelExample::~LabelExample()
    {
        //The Scene takes care of deleting any GameObjects, which includes our Label objects
    }
    
    float LabelExample::LoadContent()
    {
        //Create a Label with a bitmap font created with Glyph Designer
        m_BitmapLabel = (Label*)AddGameObject(new Label("TestFont"));
        m_BitmapLabel->SetText("Hello World?");
        
        //Create a Label with a true type font
        m_TrueTypeLabel = (Label*)AddGameObject(new Label("NewRocker-Regular", "otf", 64));
        m_TrueTypeLabel->SetText("Hello World!");
        m_TrueTypeLabel->SetColor(Color::BlackColor());
        
        //Reset all the labels to their initial state
        Reset();
        
        //Return 1.0f when loading has completed
        return 1.0f;
    }
    
    void LabelExample::HandleEvent(Event* aEvent)
    {
        switch (aEvent->GetEventCode())
        {
            case KEYBOARD_EVENT:
            {
                KeyEvent* keyEvent = (KeyEvent*)aEvent;
                if(keyEvent->GetKeyEventType() == KeyUp)
                {
                    //When the Space key is pressed the label's local angle is animated for 3 second
                    if(keyEvent->GetKeyCode() == KEY_CODE_SPACE)
                    {
                        m_BitmapLabel->SetLocalAngle(360.0f, 3.0);
                    }

                    //When the Tab key is pressed the label's character angle is animated for 3 seconds
                    else if(keyEvent->GetKeyCode() == KEY_CODE_TAB)
                    {
                        m_BitmapLabel->SetCharacterAngle(360.0f, 3.0);
                    }

                    //When the left Shift key is pressed the label's character spacing is animated (and repeated) for 0.5 seconds
                    else if(keyEvent->GetKeyCode() == KEY_CODE_LEFT_SHIFT)
                    {
                        m_TrueTypeLabel->SetCharacterSpacing(10.0f, 0.5, Linear::Interpolation, true);
                    }

                    //When the right Shift key is pressed the label's scale and anchor points are animated for 1 second
                    else if(keyEvent->GetKeyCode() == KEY_CODE_RIGHT_SHIFT)
                    {
                        m_TrueTypeLabel->SetLocalScale(0.5f, 0.5f, 1.0);
                        m_TrueTypeLabel->SetAnchorPoint(0.5f, 0.5f, 1.0);
                    }
                    
                    //When the A key is pressed fade the label alpha to zero over 2 seconds
                    else if(keyEvent->GetKeyCode() == KEY_CODE_A)
                    {
                        m_TrueTypeLabel->SetAlpha(0.0f, 2.0);
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
    
    void LabelExample::Reset()
    {
        m_BitmapLabel->SetLocalPosition(100.0f, 100.0f);
        m_BitmapLabel->SetLocalScale(1.0f, 1.0f);
        m_BitmapLabel->SetLocalAngle(0.0f);
        m_BitmapLabel->SetCharacterAngle(0.0f);
        m_BitmapLabel->SetCharacterSpacing(0.0f);
        m_BitmapLabel->SetAlpha(1.0f);

        m_TrueTypeLabel->SetLocalPosition(100.0f, 300.0f);
        m_TrueTypeLabel->SetLocalScale(1.0f, 1.0f);
        m_TrueTypeLabel->SetLocalAngle(0.0f);
        m_TrueTypeLabel->SetCharacterAngle(0.0f);
        m_TrueTypeLabel->SetCharacterSpacing(0.0f);
        m_TrueTypeLabel->SetAlpha(1.0f);
    }
}