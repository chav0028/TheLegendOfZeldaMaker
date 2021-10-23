//
//  ButtonList.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-08-26.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__ButtonList__
#define __GameDev2D__ButtonList__

#include "../../Core/GameObject.h"
#include "../../Input/KeyCodes.h"
#include "../../Graphics/Core/Color.h"


namespace GameDev2D
{
    //Local constants
    const float BUTTON_LIST_DEFAULT_SPACER = 10.0f;
    const KeyCode BUTTON_LIST_DEFAULT_SELECTION_KEY_CODE = KEY_CODE_RETURN;
    const Color BUTTON_LIST_DEFAULT_SELECTION_COLOR = Color::OuterSpaceBlackColor();
    const Color BUTTON_LIST_DEFAULT_NORMAL_COLOR = Color::GrayColor();
    const double BUTTON_LIST_SHIFT_DURATION = 0.15;
    const float BUTTON_LIST_SHIFT_AMOUNT_PCT = 0.0390625f;

    //Forward declarations
    class Button;

    //Creates a list of Button objects with the same type of font and size, the list handles keyboard input, selection scaling and color.
    //Currently the ButtonList only supports true type font Button objects
    class ButtonList : public GameObject, public EventDispatcher
    {
    public:
        //Creates a list of Button objects with the same type of font and size
        ButtonList(const string& trueTypeFont, const string& extension, const unsigned int fontSize);
        ~ButtonList();
        
        //Used to handle input events
        void HandleEvent(Event* event);
        
        //Resets the button list to its initial state
        void Reset();
        
        //Adds a button to the button list
        void AddButton(EventHandler* eventHandler, const string& text);
        
        //Sets and gets key code that initiates a button selection, by default its KEY_CODE_RETURN
        void SetSelectionKeyCode(KeyCode keyCode);
        KeyCode GetSelectionKeyCode();
        
        //Sets and gets the selected button color, by default its antique white
        void SetSelectionColor(Color color);
        Color GetSelectionColor();
        
        //Sets and gets the normal button color, by default its white
        void SetNormalColor(Color color);
        Color GetNormalColor();
        
        //Sets and gets the space between each button, by default its zero
        void SetButtonSpacer(float spacer, unsigned int index);
        float GetButtonSpacer(unsigned int index);
        
        //Sets wether the buttons in the button list should automatically increase it's scale when it is selected
        void SetScaleOnSelection(bool shouldScale);
        
        //Returns wether the buttons in the button list should automatically increase it's scale when it is selected
        bool ShouldScaleOnSelection();
        
        //Sets wether the buttons in the button list should automatically shift right when it is selected
        void SetShiftOnSelection(bool shouldShift);
        
        //Returns wether the buttons in the button list should automatically shift right it is selected
        bool ShouldShiftOnSelection();
        
        //Returns the size of the button list
        vec2 GetSize();
        
        //Returns the number of buttons in the button list
        unsigned long GetNumberOfButtons();
        
        //Returns the button object for a specific index
        Button* GetButtonAtIndex(unsigned int index) const;
        
        //Returns a button's index in the button list, returns -1 if the button isn't in the list
        int GetIndexForButton(Button* button);
        
        //Set the button at the index parameter as the selected button, also unselects the previously selected button
        void SetSelectedIndex(unsigned int index);
        
    private:
        //Conveniance method to update the buttons colors based on the selected index
        void UpdateButtonColors();
        
        //Conveniance method to update the buttons positions
        void UpdateButtonPositions();
    
        //Member variables
        string m_TrueTypeFont;
        string m_FontExtension;
        unsigned int m_FontSize;
        vector<Button*> m_Buttons;
        int m_SelectedIndex;
        vector<float> m_Spacers;
        KeyCode m_SelectionKeyCode;
        Color m_SelectionColor;
        Color m_NormalColor;
        bool m_ScaleOnSelection;
        bool m_ShiftOnSelection;
    };
}

#endif /* defined(__GameDev2D__ButtonList__) */
