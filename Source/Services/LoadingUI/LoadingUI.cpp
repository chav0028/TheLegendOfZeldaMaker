//
//  LoadingUI.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-04-22.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "LoadingUI.h"
#include "../ServiceLocator.h"
#include "../DebugUI/DebugUI.h"
#include "../../UI/Label/Label.h"
#include "../../Platforms/PlatformLayer.h"
#include "../../Events/Platform/ResizeEvent.h"


namespace GameDev2D
{
    LoadingUI::LoadingUI() : GameService("LoadingUI"),
        m_LoadingLabel(nullptr),
        m_BackgroundRect(nullptr),
        m_LoadingBarOutline(nullptr),
        m_LoadingBarInside(nullptr),
        m_PercentageLoaded(0.0f),
        m_IsShowing(true)
    {
        //Load the default loading font
        SetLoadingFont(DEFAULT_LOADING_FONT, "ttf", 48);
        
        //Create a black background rect
        m_BackgroundRect = new Rect();
        m_BackgroundRect->SetColor(Color::BlackColor());
        
        //Create the loading bar outline rect
        m_LoadingBarOutline = new Rect();
        m_LoadingBarOutline->SetColor(Color::WhiteColor());
        m_LoadingBarOutline->SetAnchorPoint(0.5f, 0.5f);
        m_LoadingBarOutline->SetIsFilled(false);
        
        //Create the loading bar inside (filled) rect
        m_LoadingBarInside = new Rect();
        m_LoadingBarInside->SetColor(Color::WhiteColor());
        m_LoadingBarInside->SetAnchorPoint(0.0f, 0.5f);
        
        //Register for a resize event
        ServiceLocator::GetPlatformLayer()->AddEventListener(this, RESIZE_EVENT);
    }

    LoadingUI::~LoadingUI()
    {
        //Remove the listener for the resize event
        ServiceLocator::GetPlatformLayer()->RemoveEventListener(this, RESIZE_EVENT);
    
        //Delete our member objects
        SafeDelete(m_LoadingLabel);
        SafeDelete(m_BackgroundRect);
        SafeDelete(m_LoadingBarOutline);
        SafeDelete(m_LoadingBarInside);
    }
    
    void LoadingUI::Show()
    {
        //Set the is showing flag to true
        m_IsShowing = true;
        
        //Reset the percentage loaded float
        m_PercentageLoaded = 0.0f;
    }
    
    void LoadingUI::Hide()
    {
        //Set the is showing flag to false
        m_IsShowing = false;
    }
    
    void LoadingUI::Draw()
    {
        if(m_BackgroundRect != nullptr)
        {
            m_BackgroundRect->Draw();
        }

        if(m_LoadingBarOutline != nullptr)
        {
            m_LoadingBarOutline->Draw();
        }
 
        if(m_LoadingBarInside != nullptr)
        {
            m_LoadingBarInside->Draw();
        }

        if(m_LoadingLabel != nullptr)
        {
            m_LoadingLabel->Draw();
        }
    }
    
    void LoadingUI::SetLoadingBarPercentage(float aPercentage)
    {
        //Set the percentage loaded
        m_PercentageLoaded = aPercentage;
    
        //Safety check the inside loading bar
        if(m_LoadingBarInside != nullptr)
        {
            //Calculate the width the loading bar should be
            float width = (float)ServiceLocator::GetPlatformLayer()->GetWidth();
            float padding = 6.0f;
            float loadingBarWidth = ((width * 0.4f) - padding) * m_PercentageLoaded;
            m_LoadingBarInside->SetWidth(loadingBarWidth);
        }
    }

    void LoadingUI::SetBackgroundColor(Color aBackgroundColor)
    {
        if(m_BackgroundRect != nullptr)
        {
            m_BackgroundRect->SetColor(aBackgroundColor);
        }
    }
    
    void LoadingUI::SetLoadingBarColor(Color aLoadingBarColor)
    {
        if(m_LoadingBarOutline != nullptr)
        {
            m_LoadingBarOutline->SetColor(aLoadingBarColor);
        }
        
        if(m_LoadingBarInside != nullptr)
        {
            m_LoadingBarInside->SetColor(aLoadingBarColor);
        }
    }
    
    void LoadingUI::SetLoadingLabelColor(Color aLoadingLabelColor)
    {
        if(m_LoadingLabel != nullptr)
        {
            m_LoadingLabel->SetColor(aLoadingLabelColor);
        }
    }
    
    void LoadingUI::SetLoadingFont(const string& aTrueTypeFont, const string& aExtension, unsigned int aFontSize, Color aLoadingLabelColor)
    {
        //Delete the loading label
        SafeDelete(m_LoadingLabel);
    
        //Create a new loading label
        m_LoadingLabel = new Label(aTrueTypeFont, aExtension, aFontSize);
        m_LoadingLabel->SetText("LOADING");
        m_LoadingLabel->SetColor(aLoadingLabelColor);
        m_LoadingLabel->SetJustification(JustifyCenter);
        m_LoadingLabel->SetAnchorPoint(0.5f, 0.5f);
    }

    bool LoadingUI::CanUpdate()
    {
        return false;
    }
    
    bool LoadingUI::CanDraw()
    {
        return m_IsShowing;
    }
    
    void LoadingUI::HandleEvent(Event* aEvent)
    {
        switch (aEvent->GetEventCode())
        {
            case RESIZE_EVENT:
            {
                //Get the size from the resize event
                ResizeEvent* resizeEvent = (ResizeEvent*)aEvent;
                vec2 size = resizeEvent->GetSize();
            
                //Set the background rect's size
                if(m_BackgroundRect != nullptr)
                {
                    m_BackgroundRect->SetSize(size);
                }
            
                //Set the position of the loading label
                if(m_LoadingLabel != nullptr)
                {
                    m_LoadingLabel->SetLocalPosition(size.x * 0.5f, size.y * 0.4f);
                    m_LoadingLabel->ModelMatrixIsDirty(true);
                }
                
                //Set the new size of the loading bar
                vec2 loadingBarSize = vec2(size.x * 0.4f, size.y * 0.05f);
                
                //Set the position and size of the loading bar outline
                if(m_LoadingBarOutline != nullptr)
                {
                    m_LoadingBarOutline->SetLocalPosition(size.x * 0.5f, size.y * 0.5f);
                    m_LoadingBarOutline->SetSize(loadingBarSize);
                }
                
                //Set the position and size of the loading bar inside
                if(m_LoadingBarInside != nullptr)
                {
                    float x = (size.x - (loadingBarSize.x - LOADING_BAR_INNER_PADDING)) / 2.0f;
                    m_LoadingBarInside->SetLocalPosition(x, size.y * 0.5f);
                    m_LoadingBarInside->SetSize(vec2((loadingBarSize.x - LOADING_BAR_INNER_PADDING) * m_PercentageLoaded, loadingBarSize.y - LOADING_BAR_INNER_PADDING));
                }
            }
            break;
                
            default:
                break;
        }
    }
}