//
//  ButtonList.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-08-26.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "ButtonList.h"
#include "../Button/Button.h"
#include "../Label/Label.h"
#include "../../Services/ServiceLocator.h"
#include "../../Platforms/PlatformLayer.h"
#include "../../Events/Input/InputEvents.h"


namespace GameDev2D
{
    ButtonList::ButtonList(const string& aTrueTypeFont, const string& aExtension, const unsigned int aFontSize) : GameObject("ButtonList"),
        m_TrueTypeFont(aTrueTypeFont),
        m_FontExtension(aExtension),
        m_FontSize(aFontSize),
        m_SelectedIndex(-1),
        m_SelectionKeyCode(BUTTON_LIST_DEFAULT_SELECTION_KEY_CODE),
        m_SelectionColor(BUTTON_LIST_DEFAULT_SELECTION_COLOR),
        m_NormalColor(BUTTON_LIST_DEFAULT_NORMAL_COLOR),
        m_ScaleOnSelection(true),
        m_ShiftOnSelection(false)
    {
        //Add a listener for keyboard input
        ServiceLocator::GetInputManager()->AddEventListener(this, KEYBOARD_EVENT);
    }
    
    ButtonList::~ButtonList()
    {
        //Remove all the input listeners
        ServiceLocator::GetInputManager()->RemoveAllHandlersForListener(this);
    
        //Cycle through all the buttons in the list and delete them
        while(m_Buttons.empty() == false)
        {
            //Remove the child button
            RemoveChild(m_Buttons.back());
            
            //Delete the button object
            SafeDelete(m_Buttons.back());
            
            //Pop back the buttons vector
            m_Buttons.pop_back();
        }
    }
    
    void ButtonList::HandleEvent(Event* aEvent)
    {
        switch (aEvent->GetEventCode())
        {
            case BUTTON_STATE_CHANGED:
            {
                //Cast the event data to a button pointer
                Button* button = (Button*)aEvent->GetEventData();
                
                //Is the button state selected?
                if(button->GetState() == ButtonStateSelected)
                {
                    //Get the index for the button
                    int index = GetIndexForButton(button);
                    
                    //Safety check the index and set the new selected index
                    if(index != -1)
                    {
                        SetSelectedIndex(index);
                    }
                }
            }
            break;
            
            case KEYBOARD_EVENT:
            {
                //Cast the Event pointer to a KeyEvent pointer
                KeyEvent* keyEvent = (KeyEvent*)aEvent;
                
                //Is the key event an key up event?
                if(keyEvent->GetKeyEventType() == KeyUp)
                {
                    //Handle the key code
                    if(keyEvent->GetKeyCode() == KEY_CODE_UP)
                    {
                        if(m_Buttons.at(m_SelectedIndex)->ContainsPoint(ServiceLocator::GetInputManager()->GetMousePosition()) == false)
                        {
                            SetSelectedIndex(m_SelectedIndex - 1);
                        }
                    }
                    else if(keyEvent->GetKeyCode() == KEY_CODE_DOWN)
                    {
                        if(m_Buttons.at(m_SelectedIndex)->ContainsPoint(ServiceLocator::GetInputManager()->GetMousePosition()) == false)
                        {
                            SetSelectedIndex(m_SelectedIndex + 1);
                        }
                    }
                    else if(keyEvent->GetKeyCode() == GetSelectionKeyCode())
                    {
                        if(m_SelectedIndex != -1)
                        {
                            m_Buttons.at(m_SelectedIndex)->HandleButtonAction();
                        }
                    }
                }
            }
            break;

            default:
                break;
        }
    }
    
    void ButtonList::Reset()
    {
        //Update the button colors
        UpdateButtonColors();
        
        //Update the button positions
        UpdateButtonPositions();
    
        //And set the selected index to zero
        SetSelectedIndex(0);
    
        GameObject::Reset();
    }
    
    void ButtonList::AddButton(EventHandler* aEventHandler, const string& aText)
    {
        //If this assert is hit, it means the event handler is null, the event handler can't be null
        assert(aEventHandler != nullptr);
    
        //Safety check the event handler pointer
        if(aEventHandler != nullptr)
        {
            //Create the button object
            Button* button = new Button(m_TrueTypeFont, m_FontExtension, m_FontSize);
        
            //Set the button's text
            button->SetText(aText);
            
            //Add a listener for the button state changed event
            button->AddEventListener(this, BUTTON_STATE_CHANGED);
            
            //Add a listener for the event handler
            button->AddEventListener(aEventHandler, BUTTON_ACTION);
            
            //Set that the button should NOT set the normal state when the mouse exits the button
            button->SetStateToNormalOnMouseExit(false);
            
            //Set wether we should scale the buttons or not
            button->SetScaleOnSelection(m_ScaleOnSelection);
            
            //Set the button's anchor point
            button->SetAnchorPoint(0.5f, 0.5f);
            
            //Remove the button padding
            button->SetPadding(0.0f, 0.0f, 0.0f, 0.0f);
        
            //Add the button to the button's vector
            m_Buttons.push_back(button);
            
            //Add the button as a child
            AddChild(button, false);
            
            //Add a default spacer
            m_Spacers.push_back(BUTTON_LIST_DEFAULT_SPACER);
        }
        
        //Update the button's positions
        UpdateButtonPositions();
    }
    
    void ButtonList::SetSelectionKeyCode(KeyCode aKeyCode)
    {
        m_SelectionKeyCode = aKeyCode;
    }
    
    KeyCode ButtonList::GetSelectionKeyCode()
    {
        return m_SelectionKeyCode;
    }
    
    void ButtonList::SetSelectionColor(Color aColor)
    {
        //Set the selection color
        m_SelectionColor = aColor;
        
        //Then update the button colors
        UpdateButtonColors();
    }
    
    Color ButtonList::GetSelectionColor()
    {
        return m_SelectionColor;
    }
    
    void ButtonList::SetNormalColor(Color aColor)
    {
        //Set the normal color
        m_NormalColor = aColor;
        
        //Then update the button colors
        UpdateButtonColors();
    }
    
    Color ButtonList::GetNormalColor()
    {
        return m_NormalColor;
    }
    
    void ButtonList::SetButtonSpacer(float aSpacer, unsigned int aIndex)
    {
        //Safety check
        if(aIndex < m_Spacers.size())
        {
            //Set the button spacer
            m_Spacers.at(aIndex) = aSpacer;
        
            //Update the buttons positions
            UpdateButtonPositions();
        }
    }
    
    float ButtonList::GetButtonSpacer(unsigned int aIndex)
    {
        //Safety check
        if(aIndex < m_Spacers.size())
        {
            return m_Spacers.at(aIndex);
        }
        return 0.0f;
    }
    
    void ButtonList::SetScaleOnSelection(bool aShouldScale)
    {
        //Set the scale of selection flag
        m_ScaleOnSelection = aShouldScale;
        
        //Then update all the button objects
        for(unsigned int i = 0; i < m_Buttons.size(); i++)
        {
            m_Buttons.at(i)->SetScaleOnSelection(m_ScaleOnSelection);
        }
    }
    
    bool ButtonList::ShouldScaleOnSelection()
    {
        return m_ScaleOnSelection;
    }
    
    void ButtonList::SetShiftOnSelection(bool aShouldShift)
    {
        m_ShiftOnSelection = aShouldShift;
    }
    
    bool ButtonList::ShouldShiftOnSelection()
    {
        return m_ShiftOnSelection;
    }
    
    unsigned long ButtonList::GetNumberOfButtons()
    {
        return m_Buttons.size();
    }
    
    vec2 ButtonList::GetSize()
    {
        vec2 size = vec2(0.0f, 0.0f);
        
        //Cycle through and determine the size of the button list
        for(unsigned int i = 0; i < m_Buttons.size(); i++)
        {
            size.x = fmaxf(m_Buttons.at(i)->GetSize().x, size.x);
            size.y += m_Buttons.at(i)->GetSize().y + (i < m_Buttons.size() - 1 ? GetButtonSpacer(i) : 0.0f);
        }
        
        //Return the size of the button list
        return size;
    }
    
    Button* ButtonList::GetButtonAtIndex(unsigned int aIndex) const
    {
        //Safety check the index
        if(aIndex < m_Buttons.size())
        {
            return m_Buttons.at(aIndex);
        }
        
        return nullptr;
    }
    
    int ButtonList::GetIndexForButton(Button* aButton)
    {
        if(aButton != nullptr)
        {
            for(unsigned int i = 0; i < m_Buttons.size(); i++)
            {
                if(m_Buttons.at(i) == aButton)
                {
                    return i;
                }
            }
        }
        
        return -1;
    }
    

    void ButtonList::SetSelectedIndex(unsigned int aIndex)
    {
        //Safety check the index bounds and that the desired index isn't already set
        if(aIndex < GetNumberOfButtons() && aIndex != m_SelectedIndex)
        {
            //Safety check the selected index
            if(m_SelectedIndex != -1)
            {
                m_Buttons.at(m_SelectedIndex)->SetState(ButtonStateNormal);
                if(m_ShiftOnSelection == true)
                {
                    m_Buttons.at(m_SelectedIndex)->SetLocalX(0.0f, BUTTON_LIST_SHIFT_DURATION);
                }
                SetChildDepth(m_Buttons.at(m_SelectedIndex), 0);
            }

            //Set the selected index
            m_SelectedIndex = aIndex;
            
            //Safety check the selected index
            if(m_SelectedIndex != -1)
            {
                m_Buttons.at(m_SelectedIndex)->SetState(ButtonStateSelected);
                if(m_ShiftOnSelection == true)
                {
                    float shiftAmount = ServiceLocator::GetPlatformLayer()->GetWidth() * BUTTON_LIST_SHIFT_AMOUNT_PCT;
                    m_Buttons.at(m_SelectedIndex)->SetLocalX(shiftAmount, BUTTON_LIST_SHIFT_DURATION);
                }
                SetChildDepth(m_Buttons.at(m_SelectedIndex), 1);
            }
        }
    }
    
    void ButtonList::UpdateButtonColors()
    {
        for(unsigned int i = 0; i < m_Buttons.size(); i++)
        {
            m_Buttons.at(i)->SetTextColor(m_SelectionColor, ButtonStateSelected);
            m_Buttons.at(i)->SetTextColor(m_NormalColor, ButtonStateNormal);
        }
    }
    
    void ButtonList::UpdateButtonPositions()
    {
        //Start at the top
        vec2 position = vec2(0.0f, GetSize().y);
        
        //Cycle through all the buttons and set their position
        for(unsigned int i = 0; i < m_Buttons.size(); i++)
        {
            position.y -= m_Buttons.at(i)->GetSize().y * 0.5f;
            m_Buttons.at(i)->SetLocalPosition(position);
            position.y -= (m_Buttons.at(i)->GetSize().y * 0.5f) + GetButtonSpacer(i);
        }
    }
}