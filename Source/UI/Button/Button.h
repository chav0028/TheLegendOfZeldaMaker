//
//  Button.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-27.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Button__
#define __GameDev2D__Button__

#include "../../Core/GameObject.h"
#include "../../Events/Input/TouchEvent.h"
#include "../../Events/Input/MouseClickEvent.h"
#include "../../Events/Input/MouseMovementEvent.h"
#include "../../Events/EventHandler.h"
#include "../../Events/Input/TouchEvent.h"
#include "../../Graphics/Core/Color.h"
#include "../../Animation/Tween.h"
#include "../../Animation/Easing.h"


using namespace glm;

namespace GameDev2D
{    
    //Conveniance struct to manage the button padding, ie: how much space from the
    //edges of the label or foreground image to the outside of the button, if no
    //backgroud image is set or if no preferred size is set.
    struct ButtonPadding
    {
        ButtonPadding(float left, float right, float bottom, float top)
        {
            this->left = left;
            this->right = right;
            this->bottom = bottom;
            this->top = top;
        }
        
        float left;
        float right;
        float bottom;
        float top;
    };
    
    //Button state enum, Normal means the button is idle and not being selected.
    //The selected state means the button is selected and can send a BUTTON_ACTION
    //event if the input merits the event be sent. The disable input but default
    //turns the font gray and the outline if set gray and disables all input
    //from affecting the button state
    enum ButtonState
    {
        ButtonStateNormal = 0,
        ButtonStateSelected,
        ButtonStateDisabled,
        ButtonStateCount
    };
    
    //Button constants
    const double BUTTON_SELECTED_SCALING_DURATION = 0.1;
    const float BUTTON_SELECTED_SCALE = 1.2f;

    //Forward Declarations
    class Label;
    class Sprite;
    class Rect;
    class Shader;


    //The Button class inherits from GameObject, it is quite a versitile UI class. It can display a
    //bitmap OR true type font as it's label OR a foreground image, in addition to a background image
    //OR a background color mixed with an outline color. The true type font color, foreground image,
    //background image, background color and outline color can be set for each button state (Normal,
    //Selected, Disabled). The button can be set to be toggled, and by default will scale 1.25 times
    //on selection.
    //This class dispatches the following events:
    // BUTTON_ACTION
    // BUTTON_STATE_CHANGED
    //The EventData* contains a pointer to the Button* that disptached the events
    class Button : public GameObject, public EventDispatcher
    {
    public:
        //Creates a button with a BitmapFont Label
        Button(const string& bitmapFont);
        
        //Creates a button with a TrueTypeFont Label
        Button(const string& trueTypeFont, const string& extension, const unsigned int fontSize);
        
        //Creates a button with a non-animated Sprite image as the foreground image
        Button(const string& filename, const string& atlasKey);
        
        //Creates a button with an animated Sprite image as the foreground image
        Button(const string& imageFile, const string& jsonFile, bool doesLoop, float frameSpeed);
        ~Button();
        
        //Updates the Button
        void Update(double delta);
        
        //Sets the current state of the button
        void SetState(ButtonState state);
        
        //Returns the current state of the button
        ButtonState GetState();
        
        //Sets the Button's Label text
        void SetText(const string& text);
        
        //Returns the Button's Label text
        string GetText();
        
        //Returns the Label used to display the Button's text
        Label* GetLabel() const;
    
        //Sets a background image for the button state, if a background image is set, the button will
        //automatically set it's size to that of the background image
        void SetBackground(const string& aFilename, const string& aAtlasKey, ButtonState buttonState = ButtonStateNormal);
        
        //Sets a background image for the button state, if a background image is set, the button will
        //automatically set it's size to that of the background image. The Button takes ownership of
        //the Sprite object and will delete it from memory
        void SetBackground(Sprite* background, ButtonState buttonState = ButtonStateNormal);
        
        //Sets a foreground image for the button state, a foreground image can ONLY be set if there is NO Label set
        void SetImage(const string& filename, const string& atlasKey, ButtonState buttonState = ButtonStateNormal);
        
        //Sets a foreground image for the button state, a foreground image can ONLY be set if there is NO Label set.
        //The Button takes ownership of the Sprite object and will delete it from memory
        void SetImage(Sprite* image, ButtonState buttonState = ButtonStateNormal);
    
        //Sets the text color for a button state
        void SetTextColor(Color textColor, ButtonState buttonState = ButtonStateNormal);
        
        //Returns the text color for the button state
        Color GetTextColor(ButtonState buttonState);
        
        //Set the background color for a button state, if a background texture is set,
        //then the background color is ignored.
        void SetBackgroundColor(Color backgroundColor, ButtonState buttonState = ButtonStateNormal);
        
        //Returns the background color, for the button state
        Color GetBackgroundColor(ButtonState buttonState);
        
        //Set the outline color for a button state, if a background texture is set,
        //then the outline color is ignored.
        void SetOutlineColor(Color outlineColor, ButtonState buttonState = ButtonStateNormal);
        
        //Returns the outline color, for the button state
        Color GetOutlineColor(ButtonState buttonState);
        
        //If a background texture is NOT set for the button AND the preferred size is NOT set,
        //then you can set how much padding there should be from the edges of the button to
        //the bounds of the label or image
        void SetPadding(float left, float right, float bottom, float top);
        
        //Returns the button padding
        ButtonPadding GetPadding();
        
        //If a background texture is NOT set for the button, but rather a background rect
        //is being used you can set the preferred size of the background rect. If a
        //background texture is set for the button this method will have no effect.
        void SetPreferredSize(vec2 preferredSize);
        
        //Set the button to be toggle-able or not
        void SetIsToggled(bool isToggled);
        
        //Returns wether the button is toggle-able or not
        bool IsToggled();
        
        //Sets wether the button should automatically increase it's scale when it is selected
        void SetScaleOnSelection(bool shouldScale);
        
        //Returns wether the button should automatically increase it's scale when it is selected
        bool ShouldScaleOnSelection();
        
        //Returns the size of the button
        vec2 GetSize();
        float GetWidth();
        float GetHeight();
        
        //Sets the anchor point for the Button, can be set to animate over a duration, an easing function can be applied, can be revered and repeated
        void SetAnchorX(float anchorX, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetAnchorY(float anchorY, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetAnchorPoint(float anchorX, float anchorY, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetAnchorPoint(vec2 anchorPoint, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Returns the anchor point of the Button
        vec2 GetAnchorPoint();
        
        //Sets the local position for the Button, can be set to animate over a duration, an easing function can be applied, can be revered and repeated
        void SetLocalX(float x, double duration = 0.0, EasingFunction easing = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetLocalY(float y, double duration = 0.0, EasingFunction easing = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Sets the local angle for the Button, can be set to animate over a duration, an easing function can be applied, can be revered and repeated
        void SetLocalAngle(float angle, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Sets the local scale for the Button, can be set to animate over a duration, an easing function can be applied, can be revered and repeated
        void SetLocalScaleX(float scaleX, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetLocalScaleY(float scaleY, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Set's the shader to render the button with
        void SetShader(Shader* shader);
        
        //Returns wether this GameObject should be drawn
        bool CanDraw();
        
        //Enable and Disable the Button, if a Button is disabled,
        //it can't dispatch BUTTON_ACTION events
        void SetIsEnabled(bool isEnabled);
        
        //Used to handle input events
        void HandleEvent(Event* event);
        
    protected:
        //Conveniance methods to handle touch and mouse inputs
        void HandleTouchInput(TouchEvent* touchEvent);
        void HandleMouseClickInput(MouseClickEvent* mouseMovementEvent);
        
        //Conveniance method to dispatch a BUTTON_ACTION event
        void HandleButtonAction();
        
        //Conveniance method to check if a point resides in a
        //button, factors in the angle of the button and the scale
        bool ContainsPoint(vec2 point);
        
        //Initialize the state data for the background, image and color arrays
        void InitializeStateData();
        
        //Conveniance methods to reset background, image, color
        //data based on the current state of the button
        void ResetBackground();
        void ResetImage();
        void ResetTextColor();
        void ResetBackgroundColor();
        void ResetOutlineColor();
    
        //Calculates the size of the button based on many factors: is a background texture set or not, is there
        //a preferred size, font size, image size, and it updates the children's position accordingly
        void CalculateSize();
        
        //Resets the Button's model matrix
        void ResetModelMatrix();
        
        //Used to set wether the mouse should set the button state back to normal when the mouse isn't selecting it
        void SetStateToNormalOnMouseExit(bool enabled);
        
        //Friend class needs to access the SetStateToNormalOnMouseExit() and HandleButtonAction() methods
        friend class ButtonList;
        
    private:
        //Member variables
        Label* m_Label;
        Rect* m_Rect;
        Rect* m_Outline;
        Sprite* m_Background[ButtonStateCount];
        Sprite* m_Image[ButtonStateCount];
        Color m_TextColor[ButtonStateCount];
        Color m_BackgroundColor[ButtonStateCount];
        Color m_OutlineColor[ButtonStateCount];
        float m_ScalingState[ButtonStateCount];
        vec2 m_ActualSize;
        vec2 m_PreferredSize;
        vec2 m_AnchorPoint;
        Tween m_TweenAnchorX;
        Tween m_TweenAnchorY;
        ButtonState m_State;
        ButtonPadding m_Padding;
        bool m_ScaleOnSelection;
        bool m_IsToggled;
        bool m_SetStateToNormalOnMouseExit;
    };
}

#endif /* defined(__GameDev2D__Button__) */
