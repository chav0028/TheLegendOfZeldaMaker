//
//  LoadingUI.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-04-22.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__LoadingUI__
#define __GameDev2D__LoadingUI__

#include "../GameService.h"
#include "../../Events/EventHandler.h"
#include "../../Graphics/Core/Color.h"


namespace GameDev2D
{
    //Forward declarations
    class Label;
    class Rect;
    
    //Local constants
    const string DEFAULT_LOADING_FONT = "slkscr";
    const float LOADING_BAR_INNER_PADDING = 6.0f;

    //The LoadingUI game service display a loading screen with a loading bar, it is currently leveraged
    //by the SceneManager, and is displayed while a Scene is loading. It is very primitive right now and
    //can be expanded to display a background image instead of a black background or change the positioning
    //of the loading bar and label.
    class LoadingUI : public GameService
    {
    public:
        LoadingUI();
        ~LoadingUI();
        
        //Methods used to show and hide the LoadingUI
        void Show();
        void Hide();
        
        //Draw the LoadingUI, called from the ServiceLocator
        void Draw();
        
        //Sets the amount the loading bar should be loaded
        void SetLoadingBarPercentage(float percentage);
        
        //Sets the background color
        void SetBackgroundColor(Color backgroundColor);
        
        //Sets the loading bar color
        void SetLoadingBarColor(Color loadingBarColor);
        
        //Sets the loading label color
        void SetLoadingLabelColor(Color loadingLabelColor);
        
        //Set's the loading font
        void SetLoadingFont(const string& trueTypeFont, const string& extension, unsigned int fontSize, Color loadingLabelColor = Color::WhiteColor());
        
        //Used to determine if the AudioManager should be updated and drawn
        bool CanUpdate();
        bool CanDraw();
        
        //Used to handle the resize event
        void HandleEvent(Event* event);
        
    private:
        //Member variables
        Label* m_LoadingLabel;
        Rect* m_BackgroundRect;
        Rect* m_LoadingBarOutline;
        Rect* m_LoadingBarInside;
        float m_PercentageLoaded;
        bool m_IsShowing;
    };
}

#endif /* defined(__GameDev2D__LoadingUI__) */
