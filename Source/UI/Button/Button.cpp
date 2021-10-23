//
//  Button.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-27.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Button.h"
#include "../Sprite/Sprite.h"
#include "../Label/Label.h"
#include "../../Graphics/Core/Shader.h"
#include "../../Animation/Tween.h"
#include "../../Services/ServiceLocator.h"
#include "../../Services/InputManager/InputManager.h"
#include "../../Platforms/PlatformLayer.h"


namespace GameDev2D
{
    Button::Button(const string& aBitmapFont) : GameObject("Button"),
        m_Label(nullptr),
        m_Rect(nullptr),
        m_Outline(nullptr),
        m_ActualSize(0.0f, 0.0f),
        m_PreferredSize(0.0f, 0.0f),
        m_AnchorPoint(0.0f, 0.0f),
        m_TweenAnchorX(),
        m_TweenAnchorY(),
        m_State(ButtonStateNormal),
        m_Padding(10.0f, 10.0f, 10.0f, 10.0f),
        m_IsToggled(false),
        m_ScaleOnSelection(true),
        m_SetStateToNormalOnMouseExit(true)
    {
        //Initialize the member's button initial state data
        InitializeStateData();

        //Create the Label and add it as a child
        m_Label = (Label*)AddChild(new Label(aBitmapFont), false, 2);
    }
    
    Button::Button(const string& aTrueTypeFont, const string& aExtension, const unsigned int aFontSize) : GameObject("Button"),
        m_Label(nullptr),
        m_Rect(nullptr),
        m_Outline(nullptr),
        m_ActualSize(0.0f, 0.0f),
        m_PreferredSize(0.0f, 0.0f),
        m_AnchorPoint(0.0f, 0.0f),
        m_TweenAnchorX(),
        m_TweenAnchorY(),
        m_State(ButtonStateNormal),
        m_Padding(10.0f, 10.0f, 10.0f, 10.0f),
        m_IsToggled(false),
        m_ScaleOnSelection(true),
        m_SetStateToNormalOnMouseExit(true)
    {
        //Initialize the member's button initial state data
        InitializeStateData();

        //Create the Label and add it as a child
        m_Label = (Label*)AddChild(new Label(aTrueTypeFont, aExtension, aFontSize), false, 2);
    }
    
    Button::Button(const string& aFilename, const string& aAtlasKey) : GameObject("Button"),
        m_Label(nullptr),
        m_Rect(nullptr),
        m_Outline(nullptr),
        m_ActualSize(0.0f, 0.0f),
        m_PreferredSize(0.0f, 0.0f),
        m_AnchorPoint(0.0f, 0.0f),
        m_TweenAnchorX(),
        m_TweenAnchorY(),
        m_State(ButtonStateNormal),
        m_Padding(0.0f, 0.0f, 0.0f, 0.0f),
        m_IsToggled(false),
        m_ScaleOnSelection(true),
        m_SetStateToNormalOnMouseExit(true)
    {
        //Initialize the member's button initial state data
        InitializeStateData();

        //Create the button image and add it as a child
        m_Image[ButtonStateNormal] = (Sprite*)AddChild(new Sprite(aFilename, aAtlasKey), false, 2);
        
        //Lastly, calculate the size of the button
        CalculateSize();
    }
    
    Button::Button(const string& aImageFile, const string& aJsonFile, bool aDoesLoop, float aFrameSpeed) : GameObject("Button"),
        m_Label(nullptr),
        m_Rect(nullptr),
        m_Outline(nullptr),
        m_ActualSize(0.0f, 0.0f),
        m_PreferredSize(0.0f, 0.0f),
        m_AnchorPoint(0.0f, 0.0f),
        m_TweenAnchorX(),
        m_TweenAnchorY(),
        m_State(ButtonStateNormal),
        m_Padding(0.0f, 0.0f, 0.0f, 0.0f),
        m_IsToggled(false),
        m_ScaleOnSelection(true),
        m_SetStateToNormalOnMouseExit(true)
    {
        //Initialize the member's button initial state data
        InitializeStateData();

        //Create the button image and add it as a child
        m_Image[ButtonStateNormal] = (Sprite*)AddChild(new Sprite(aImageFile, aJsonFile, aDoesLoop, aFrameSpeed), false, 2);
        
        //Lastly, calculate the size of the button
        CalculateSize();
    }
    
    Button::~Button()
    {
        //Stop listening for the input events
        ServiceLocator::GetInputManager()->RemoveAllHandlersForListener(this);
        
        //Delete the label, and primitive objects
        SafeDelete(m_Label);
        SafeDelete(m_Rect);
        SafeDelete(m_Outline);
        
        //Cycle through and delete the backgrounds and images for each state if set
        for(int i = 0; i < ButtonStateCount; i++)
        {
            SafeDelete(m_Background[i]);
            SafeDelete(m_Image[i]);
        }
    }
    
    void Button::Update(double aDelta)
    {
        //Update the anchor point tween's
        UpdateTween(aDelta, &m_TweenAnchorX, static_cast<GameObject::TweenSetMethod>(&Button::SetAnchorX));
        UpdateTween(aDelta, &m_TweenAnchorY, static_cast<GameObject::TweenSetMethod>(&Button::SetAnchorY));
    
        //Update the GameObject, this will update any children
        GameObject::Update(aDelta);
        
        //Determine if the mouse is intersecting the button and handle the state change
        if(m_State != ButtonStateDisabled)
        {
            //Handle mouse movement
            if(ServiceLocator::GetPlatformLayer()->HasMouseInput() == true && IsToggled() == false)
            {
                if(ContainsPoint(ServiceLocator::GetInputManager()->GetMousePosition()) == true)
                {
                    SetState(ButtonStateSelected);
                }
                else
                {
                    if(m_SetStateToNormalOnMouseExit == true)
                    {
                        SetState(ButtonStateNormal);
                    }
                }
            }
        }
    }
    
    void Button::SetState(ButtonState aState)
    {
        //Safety check that the state isn't 
        if(aState != m_State)
        {
            //Set the new button state
            m_State = aState;
            
            //If scaling is enalbed for the selected state, handle the scaling
            if(m_ScaleOnSelection == true)
            {
                SetLocalScale(m_ScalingState[m_State], m_ScalingState[m_State], BUTTON_SELECTED_SCALING_DURATION);
            }
            
            //Dispatch a button state change event
            DispatchEvent(new Event(BUTTON_STATE_CHANGED, this, VerbosityLevel_UI));
            
            //Lastly reset the button
            ResetBackground();
            ResetImage();
            ResetTextColor();
            ResetBackgroundColor();
            ResetOutlineColor();
        }
    }
    
    ButtonState Button::GetState()
    {
        return m_State;
    }
    
    void Button::SetText(const string& aText)
    {
        //Safety check that the label has already been created
        if(m_Label != nullptr)
        {
            //Set the label text
            m_Label->SetText(aText);
            
            //Lastly, calculate the size of the button
            CalculateSize();
        }
    }
    
    string Button::GetText()
    {
        //Safety check the label and return it's text
        if(m_Label != NULL)
        {
            return m_Label->GetText();
        }
        
        //If get got here it means the label is null, there is nothing to return
        return "";
    }
    
    Label* Button::GetLabel() const
    {
        return m_Label;
    }
    
    void Button::SetBackground(const string& aFilename, const string& aAtlasKey, ButtonState aButtonState)
    {
        //Set the background image for the button state
        SetBackground(new Sprite(aFilename, aAtlasKey), aButtonState);
    }
    
    void Button::SetBackground(Sprite* aBackground, ButtonState aButtonState)
    {
        //Safety check that an image hasn't already been set for this state
        if(m_Background[aButtonState] != nullptr)
        {
            //Remove Child
            RemoveChild(m_Background[aButtonState]);
            
            //Delete the background
            SafeDelete(m_Background[aButtonState]);
        }
        
        //Add a child and cache the background pointer
        m_Background[aButtonState] = (Sprite*)AddChild(aBackground, false, 0);
        m_Background[aButtonState]->SetForceUpdate(true);
        
        //Reset the background array based on the current state
        ResetBackground();
        
        //Reset the anchor point
        SetAnchorPoint(m_AnchorPoint);
        
        //Lastly, calculate the size of the button
        CalculateSize();
    }
    
    void Button::SetImage(const string& aFilename, const string& aAtlasKey, ButtonState aButtonState)
    {
        //We can only set an image, if the label has NOT been created, if the assert below is hit, that's why
        assert(m_Label == nullptr);
        SetImage(new Sprite(aFilename, aAtlasKey), aButtonState);
    }
    
    void Button::SetImage(Sprite* aImage, ButtonState aButtonState)
    {
        //We can only set an image, if the label has NOT been created, if the assert below is hit, that's why
        assert(m_Label == nullptr);
        if(m_Label == nullptr)
        {
            //Safety check that an image hasn't already been set for this state
            if(m_Image[aButtonState] != nullptr)
            {
                //Remove Child
                RemoveChild(m_Image[aButtonState]);
                
                //Delete the image
                SafeDelete(m_Image[aButtonState]);
            }
            
            //Add a child and cache the image pointer
            m_Image[aButtonState] = (Sprite*)AddChild(aImage, false, 2);
            m_Image[aButtonState]->SetForceUpdate(true);
            
            //Reset the image array based on the current state
            ResetImage();
            
            //Reset the anchor point
            SetAnchorPoint(m_AnchorPoint);
            
            //Lastly, calculate the size of the button
            CalculateSize();
        }
    }
    
    void Button::SetTextColor(Color aTextColor, ButtonState aButtonState)
    {
        //Set the label color for the state
        m_TextColor[aButtonState] = aTextColor;
    
        //Reset the label color based on the current state
        ResetTextColor();
    }
    
    Color Button::GetTextColor(ButtonState aButtonState)
    {
        return m_TextColor[aButtonState];
    }
    
    void Button::SetBackgroundColor(Color aBackgroundColor, ButtonState aButtonState)
    {
        //Safety check the background rect, create it if one doesn't exist, add it as a child
        if(m_Rect == nullptr)
        {
            m_Rect = (Rect*)AddChild(new Rect(0.0f, 0.0f, GetSize().x, GetSize().y), false, 0);
            m_Rect->SetIsFilled(true);
        }
        
        //Set the background color for the state
        m_BackgroundColor[aButtonState] = aBackgroundColor;
        
        //Reset the background color
        ResetBackgroundColor();
        
        //Set the anchoir point
        SetAnchorPoint(m_AnchorPoint);
        
        //Lastly, calculate the size of the button
        CalculateSize();
    }
    
    Color Button::GetBackgroundColor(ButtonState aButtonState)
    {
        return m_BackgroundColor[aButtonState];
    }
    
    void Button::SetOutlineColor(Color aOutlineColor, ButtonState aButtonState)
    {
        //If the outline rect is null, create a new Rect object
        if(m_Outline == nullptr)
        {
            m_Outline = (Rect*)AddChild(new Rect(0.0f, 0.0f, GetSize().x, GetSize().y), false, 1);
            m_Outline->SetIsFilled(false);
        }
        
        //Set the outline color
        m_OutlineColor[aButtonState] = aOutlineColor;
        
        //Reset the outline color, based on the current state
        ResetOutlineColor();
        
        //Set the anchor point, this updates the outline color
        SetAnchorPoint(m_AnchorPoint);
        
        //Lastly, calculate the size of the button
        CalculateSize();
    }
    
    Color Button::GetOutlineColor(ButtonState aButtonState)
    {
        return m_OutlineColor[aButtonState];
    }
    
    void Button::SetPadding(float aLeft, float aRight, float aBottom, float aTop)
    {
        //Set the padding variables
        m_Padding.left = aLeft;
        m_Padding.right = aRight;
        m_Padding.bottom = aBottom;
        m_Padding.top = aBottom;
        
        //Set the natural size of the button
        CalculateSize();
    }
    
    ButtonPadding Button::GetPadding()
    {
        return m_Padding;
    }
    
    void Button::SetPreferredSize(vec2 aPreferredSize)
    {
        m_PreferredSize = aPreferredSize;
        CalculateSize();
    }
    
    void Button::SetIsToggled(bool aIsToggled)
    {
        m_IsToggled = aIsToggled;
    }
    
    bool Button::IsToggled()
    {
        return m_IsToggled;
    }
    
    void Button::SetScaleOnSelection(bool aShouldScale)
    {
        m_ScaleOnSelection = aShouldScale;
    }
    
    bool Button::ShouldScaleOnSelection()
    {
        return m_ScaleOnSelection;
    }
    
    vec2 Button::GetSize()
    {
        return m_ActualSize;
    }
    
    float Button::GetWidth()
    {
        return GetSize().x;
    }
    
    float Button::GetHeight()
    {
        return GetSize().y;
    }
    
    void Button::SetAnchorX(float aAnchorX, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aDuration == 0.0)
        {
            m_AnchorPoint.x = fminf(fmaxf(aAnchorX, 0.0f), 1.0f);
        }
        else
        {
            m_TweenAnchorX.Set(m_AnchorPoint.x, aAnchorX, aDuration, aEasingFunction, aReverse, aRepeatCount);
        }
        
        //Safety check the label, then set its anchor point
        if(m_Label != nullptr)
        {
            m_Label->SetAnchorX(aAnchorX, aDuration, aEasingFunction, aReverse, aRepeatCount);
        }
        
        //Safety check the background rect, then set its anchor point
        if(m_Rect != nullptr)
        {
            m_Rect->SetAnchorX(aAnchorX, aDuration, aEasingFunction, aReverse, aRepeatCount);
        }
        
        //Safety check the outline rect, then set its anchor point
        if(m_Outline != nullptr)
        {
            m_Outline->SetAnchorX(aAnchorX, aDuration, aEasingFunction, aReverse, aRepeatCount);
        }
        
        //Cycle through the foreground image and the background images
        for(int i = 0; i < ButtonStateCount; i++)
        {
            //Set the foreground image's anchor point
            if(m_Image[i] != nullptr)
            {
                m_Image[i]->SetAnchorX(aAnchorX, aDuration, aEasingFunction, aReverse, aRepeatCount);
            }
            
            //Set the background image's anchor point
            if(m_Background[i] != nullptr)
            {
                m_Background[i]->SetAnchorX(aAnchorX, aDuration, aEasingFunction, aReverse, aRepeatCount);
            }
        }
        
        //Calculate the button's size
        CalculateSize();
        
        //If no animation is set, set the model matrix as dirty
        if(aDuration == 0.0)
        {
            ModelMatrixIsDirty();
        }
    }
    
    void Button::SetAnchorY(float aAnchorY, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aDuration == 0.0)
        {
            m_AnchorPoint.y = fminf(fmaxf(aAnchorY, 0.0f), 1.0f);
        }
        else
        {
            m_TweenAnchorY.Set(m_AnchorPoint.y, aAnchorY, aDuration, aEasingFunction, aReverse, aRepeatCount);
        }
        
        //Safety check the label and set the anchor
        if(m_Label != nullptr)
        {
            m_Label->SetAnchorY(aAnchorY, aDuration, aEasingFunction, aReverse, aRepeatCount);
        }
        
        //Safety check the background rect and set the anchor
        if(m_Rect != nullptr)
        {
            m_Rect->SetAnchorY(aAnchorY, aDuration, aEasingFunction, aReverse, aRepeatCount);
        }
        
        //Safety check the outline rect and set the anchor
        if(m_Outline != nullptr)
        {
            m_Outline->SetAnchorY(aAnchorY, aDuration, aEasingFunction, aReverse, aRepeatCount);
        }
        
        //Cycle through the foreground image and the background images
        for(int i = 0; i < ButtonStateCount; i++)
        {
            //Set the foreground image's anchor point
            if(m_Image[i] != nullptr)
            {
                m_Image[i]->SetAnchorY(aAnchorY, aDuration, aEasingFunction, aReverse, aRepeatCount);
            }
            
            //Set the background image's anchor point
            if(m_Background[i] != nullptr)
            {
                m_Background[i]->SetAnchorY(aAnchorY, aDuration, aEasingFunction, aReverse, aRepeatCount);
            }
        }
        
        //Calculate the button's size
        CalculateSize();
        
        //If no animation is set, set the model matrix as dirty
        if(aDuration == 0.0)
        {
            ModelMatrixIsDirty();
        }
    }
    
    void Button::SetAnchorPoint(float aAnchorX, float aAnchorY, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        SetAnchorX(aAnchorX, aDuration, aEasingFunction, aReverse, aRepeatCount);
        SetAnchorY(aAnchorY, aDuration, aEasingFunction, aReverse, aRepeatCount);
    }
    
    void Button::SetAnchorPoint(vec2 aAnchorPoint, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        SetAnchorPoint(aAnchorPoint.x, aAnchorPoint.y, aDuration, aEasingFunction, aReverse, aRepeatCount);
    }
    
    vec2 Button::GetAnchorPoint()
    {
        return m_AnchorPoint;
    }
    
    void Button::SetLocalX(float aX, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        GameObject::SetLocalX(aX, aDuration, aEasingFunction, aReverse, aRepeatCount);
        CalculateSize();
    }
    
    void Button::SetLocalY(float aY, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        GameObject::SetLocalY(aY, aDuration, aEasingFunction, aReverse, aRepeatCount);
        CalculateSize();
    }
    
    void Button::SetLocalAngle(float aAngle, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        GameObject::SetLocalAngle(aAngle, aDuration, aEasingFunction, aReverse, aRepeatCount);
        CalculateSize();
    }
    
    void Button::SetLocalScaleX(float aScaleX, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        GameObject::SetLocalScaleX(aScaleX, aDuration, aEasingFunction, aReverse, aRepeatCount);
        CalculateSize();
    }
    
    void Button::SetLocalScaleY(float aScaleY, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        GameObject::SetLocalScaleY(aScaleY, aDuration, aEasingFunction, aReverse, aRepeatCount);
        CalculateSize();
    }
    
    void Button::SetShader(Shader* aShader)
    {
        //Safety check the label and set the shader
        if(m_Label != nullptr)
        {
            m_Label->SetShader(aShader);
        }
        
        //Cycle through the foreground image and the background images
        for(int i = 0; i < ButtonStateCount; i++)
        {
            //Set the foreground image's shader
            if(m_Image[i] != nullptr)
            {
                m_Image[i]->SetShader(aShader);
            }
            
            //Set the background image's shader
            if(m_Background[i] != nullptr)
            {
                m_Background[i]->SetShader(aShader);
            }
        }
    }
    
    bool Button::CanDraw()
    {
        return true;
    }
    
    void Button::SetIsEnabled(bool aIsEnabled)
    {
        //Set that the GameObject is enabled
        GameObject::SetIsEnabled(aIsEnabled);
        
        //Handle the Button state based on if the button is enabled
        SetState(aIsEnabled == true ? ButtonStateNormal : ButtonStateDisabled);
    }
    
    void Button::HandleEvent(Event* aEvent)
    {
        if(IsEnabled() == true)
        {
            switch (aEvent->GetEventCode())
            {
                case TOUCH_EVENT:
                {
                    HandleTouchInput((TouchEvent*)aEvent);
                }
                break;
                
                case MOUSE_CLICK_EVENT:
                {
                    HandleMouseClickInput((MouseClickEvent*)aEvent);
                }
                break;

              default:
                break;
            }
        }
    }
    
    void Button::HandleTouchInput(TouchEvent* aTouchEvent)
    {
        switch (aTouchEvent->GetTouchType())
        {
            case TouchBegan:
            {
                if(ContainsPoint(aTouchEvent->GetTouchLocation()) == true)
                {
                    if(IsToggled() == false)
                    {
                        SetState(ButtonStateSelected);
                    }
                    else
                    {
                        SetState(GetState() == ButtonStateNormal ? ButtonStateSelected : ButtonStateNormal);
                    }
                }
            }
            break;
            
            case TouchMoved:
            {
                if(IsToggled() == false)
                {
                    if(ContainsPoint(aTouchEvent->GetTouchLocation()) == true)
                    {
                        SetState(ButtonStateSelected);
                    }
                    else
                    {
                        SetState(ButtonStateNormal);
                    }
                }
            }
            break;
            
            case TouchEnded:
            {
                if(IsToggled() == false)
                {
                    SetState(ButtonStateNormal);
                    HandleButtonAction();
                }
            }
            break;
            
            case TouchCancelled:
            {
                if(IsToggled() == false)
                {
                    SetState(ButtonStateNormal);
                }
            }
            break;
        }
    }
    
    void Button::HandleMouseClickInput(MouseClickEvent* aMouseClickEvent)
    {
        if(ContainsPoint(aMouseClickEvent->GetPosition()) == true && aMouseClickEvent->GetMouseClickEventType() == MouseClickUp&& aMouseClickEvent->GetMouseButton() == MouseButtonLeft)
        {
            //
            if(IsToggled() == false)
            {
                SetState(ButtonStateSelected);
            }
            else
            {
                SetState(GetState() == ButtonStateNormal ? ButtonStateSelected : ButtonStateNormal);
            }
            
            //
            HandleButtonAction();
        }
    }

    void Button::HandleButtonAction()
    {
        DispatchEvent(new Event(BUTTON_ACTION, this, VerbosityLevel_UI));
    }
    
    void Button::InitializeStateData()
    {
        //Cycle through and initialize all the member's button states
        for(int i = 0; i < ButtonStateCount; i++)
        {
            m_Background[i] = nullptr;
            m_Image[i] = nullptr;
            m_TextColor[i] = Color::WhiteColor();
            m_BackgroundColor[i] = Color::BlackColor();
            m_OutlineColor[i] = Color::BlackColor();
            m_ScalingState[i] = 1.0f;
        }
        
        //Set the disabled state data
        m_TextColor[ButtonStateDisabled] = Color::DarkGrayColor();
        m_BackgroundColor[ButtonStateDisabled] = Color::GrayColor();
        m_OutlineColor[ButtonStateDisabled] = Color::DarkGrayColor();
        
        //Set the scaling state data
        m_ScalingState[ButtonStateSelected] = BUTTON_SELECTED_SCALE;
        
        //Lastly register for input events
        ServiceLocator::GetInputManager()->AddEventListener(this, TOUCH_EVENT);
        ServiceLocator::GetInputManager()->AddEventListener(this, MOUSE_CLICK_EVENT);
    }
    
    void Button::ResetBackground()
    {
        //Cycle through the background images and set them all to be disabled
        for(int i = 0; i < ButtonStateCount; i++)
        {
            if(m_Background[i] != nullptr)
            {
                m_Background[i]->SetIsEnabled(false);
            }
        }
    
        //Set the current state to be enabled, unless the current state is null, in that case set the normal state
        if(m_Background[m_State] != nullptr)
        {
            m_Background[m_State]->SetIsEnabled(true);
        }
        else
        {
            if(m_Background[ButtonStateNormal] != nullptr)
            {
                m_Background[ButtonStateNormal]->SetIsEnabled(true);
            }
        }
    }
    
    void Button::ResetImage()
    {
        //Cycle through the foreground images and set them all to be disabled
        for(int i = 0; i < ButtonStateCount; i++)
        {
            if(m_Image[i] != nullptr)
            {
                m_Image[i]->SetIsEnabled(false);
            }
        }
    
        //Set the current state to be enabled, unless the current state is null, in that case set the normal state
        if(m_Image[m_State] != nullptr)
        {
            m_Image[m_State]->SetIsEnabled(true);
        }
        else
        {
            if(m_Image[ButtonStateNormal] != nullptr)
            {
                m_Image[ButtonStateNormal]->SetIsEnabled(true);
            }
        }
    }
    
    void Button::ResetTextColor()
    {
        if(m_Label != nullptr)
        {
            m_Label->SetColor(m_TextColor[m_State]);
        }
    }
    
    void Button::ResetBackgroundColor()
    {
        if(m_Rect != nullptr)
        {
            m_Rect->SetColor(m_BackgroundColor[m_State]);
        }
    }
    
    void Button::ResetOutlineColor()
    {
        if(m_Outline != nullptr)
        {
            m_Outline->SetColor(m_OutlineColor[m_State]);
        }
    }
    
    bool Button::ContainsPoint(vec2 aPoint)
    {
        float c = cosf(-GetWorldAngle() * (float)M_PI / 180.0f);
        float s = sinf(-GetWorldAngle() * (float)M_PI / 180.0f);

        //Unrotate the point depending on the rotation of the rectangle
        float rotatedX = GetWorldX() + c * (aPoint.x - GetWorldX()) - s * (aPoint.y - GetWorldY());
        float rotatedY = GetWorldY() + s * (aPoint.x - GetWorldX()) + c * (aPoint.y - GetWorldY());

        //perform a normal check if the new point is inside the
        //bounds of the UNrotated rectangle
        float leftX = GetWorldX() - GetWidth() * GetWorldScale().x * m_AnchorPoint.x;
        float rightX = leftX + GetWidth() * GetWorldScale().x;
        float bottomY = GetWorldY() - GetHeight() * GetWorldScale().y * m_AnchorPoint.y;
        float topY = bottomY + GetHeight() * GetWorldScale().y;
        
        //Is the point inside the button?
        bool hit = leftX <= rotatedX && rotatedX <= rightX && bottomY <= rotatedY && rotatedY <= topY;
        return hit;
    }
    
    void Button::CalculateSize()
    {
        float backgroundWidth = 0;
        float backgroundHeight = 0;
    
        //If a background image is set, get its size
        if(m_Background[ButtonStateNormal] != nullptr)
        {
            backgroundWidth = m_Background[ButtonStateNormal]->GetSize().x;
            backgroundHeight = m_Background[ButtonStateNormal]->GetSize().y;
        }
        
        //If a background image isn't set, we need to determine how big the button should be
        if(backgroundWidth == 0 && backgroundHeight == 0)
        {
            //Is the preferred size set
            if(m_PreferredSize.x != 0 && m_PreferredSize.y != 0)
            {
                backgroundWidth = m_PreferredSize.x;
                backgroundHeight = m_PreferredSize.y;
            }
            else
            {
                //Is the padding set? Factor that in
                backgroundWidth = m_Padding.left + m_Padding.right;
                backgroundHeight = m_Padding.top + m_Padding.bottom;
            
                //Add the label OR foreground image size
                if(m_Label != nullptr)
                {
                    vec2 labelSize = m_Label->GetSize();
                    backgroundWidth += labelSize.x;
                    backgroundHeight += labelSize.y;
                }
                else
                {
                    if(m_Image[ButtonStateNormal] != nullptr)
                    {
                        vec2 imageSize = m_Image[ButtonStateNormal]->GetSize();
                        backgroundWidth += imageSize.x;
                        backgroundHeight += imageSize.y;
                    }
                }
            }
        }

        //Set the actual size of the button
        m_ActualSize = vec2(backgroundWidth, backgroundHeight);
        
        //Set the background rect's size
        if(m_Rect != nullptr)
        {
            m_Rect->SetSize(m_ActualSize);
        }
        
        //Set the button's outline size
        if(m_Outline != nullptr)
        {
            m_Outline->SetSize(m_ActualSize);
        }
        
        //Reset the local position
        vec2 localPosition = vec2(0.0f, 0.0f);
        
        //Calculate the label's local position, based on the angle, anchor and scale of the button
        if(m_Label != nullptr)
        {
            float angleCos = cosf(GetWorldAngle() * (float)M_PI / 180.0f);
            float angleSin = sinf(GetWorldAngle() * (float)M_PI / 180.0f);
            vec2 labelSize = m_Label->GetSize();
            vec2 anchor = vec2(0.5f - GetAnchorPoint().x, 0.5f - GetAnchorPoint().y);
            
            //Calculate the width of the label
            float width = (backgroundWidth - labelSize.x) * anchor.x * GetWorldScale().x;
            float height = (backgroundHeight - labelSize.y) * anchor.y * GetWorldScale().y;
            
            //Calculate the local position of the label
            localPosition.x = angleCos * width - angleSin * height;
            localPosition.y = angleSin * width + angleCos * height;
        }
        else
        {
            //Calculate the foreground image's local position, based on the angle, anchor and scale of the button
            if(m_Image[ButtonStateNormal] != nullptr)
            {
                float angleCos = cosf(GetWorldAngle() * (float)M_PI / 180.0f);
                float angleSin = sinf(GetWorldAngle() * (float)M_PI / 180.0f);
                vec2 imageSize = m_Image[ButtonStateNormal]->GetSize();
                vec2 anchor = vec2(0.5f - GetAnchorPoint().x, 0.5f - GetAnchorPoint().y);
                
                //Calculate the width of the label
                float width = (backgroundWidth - imageSize.x) * anchor.x * GetWorldScale().x;
                float height = (backgroundHeight - imageSize.y) * anchor.y * GetWorldScale().y;
                
                //Calculate the local position of the label
                localPosition.x = angleCos * width - angleSin * height;
                localPosition.y = angleSin * width + angleCos * height;
            }
        }
        
        //Cycle through the children and set the local position, remember ONLY the label OR the foreground image can be set
        for(unsigned int i = 0; i < GetNumberOfChildren(); i++)
        {
            GameObject* child = GetChildAtIndex(i);
            if(GetDepthForChildAtIndex(i) > 1)
            {
                child->SetLocalPosition(localPosition);
            }
        }
        
        //Reset the model matrix
        ResetModelMatrix();
    }
    
    void Button::ResetModelMatrix()
    {
        //Translate the anchor, then translate the position
        mat4 anchor = translate(mat4(1.0f), vec3(-GetWidth() * GetWorldScale().x * m_AnchorPoint.x, -GetHeight() * GetWorldScale().y * m_AnchorPoint.y, 0.0f));
        mat4 viewTranslate = translate(anchor, vec3(GetWorldPosition().x, GetWorldPosition().y, 0.0f));

        //Calculate the rotation based on the anchor point
        mat4 halfTranslate1 = translate(viewTranslate, vec3(GetWidth() * GetWorldScale().x * m_AnchorPoint.x, GetHeight() * GetWorldScale().y * m_AnchorPoint.y, 0.0f));
        mat4 viewRotation = rotate(halfTranslate1, GetWorldAngle(), vec3(0.0f, 0.0f, 1.0f));
        mat4 halfTranslate2 = translate(viewRotation, vec3(-GetWidth() * GetWorldScale().x * m_AnchorPoint.x, -GetHeight() * GetWorldScale().y * m_AnchorPoint.y, 0.0f));
        
        //Lastly the scale
        m_ModelMatrix = scale(halfTranslate2, vec3(GetWorldScale().x, GetWorldScale().y, 0.0f));
        
        //Lastly, reset the GameObject's model matrix, this resets the model matrix of the children
        GameObject::ResetModelMatrix();
    }
    
    void Button::SetStateToNormalOnMouseExit(bool aEnabled)
    {
        m_SetStateToNormalOnMouseExit = aEnabled;
    }
}