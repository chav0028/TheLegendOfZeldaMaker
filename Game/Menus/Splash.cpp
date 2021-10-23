//
//  Splash.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-08-26.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Splash.h"
#include "MainMenu.h"
#include "../../Source/Services/ServiceLocator.h"
#include "../../Source/Platforms/PlatformLayer.h"
#include "../../Source/Audio/Audio.h"
#include "../../Source/Graphics/Graphics.h"
#include "../../Source/UI/UI.h"
#include "../../Source/Events/Input/InputEvents.h"


namespace GameDev2D
{
    Splash::Splash() : Scene("Splash")
    {

    }

    Splash::~Splash()
    {
        //The base class (Scene) will delete all the GameObjects added to the Scene
    }

    float Splash::LoadContent()
    {
        //Get the width and height of the screen
        float screenWidth = (float)ServiceLocator::GetPlatformLayer()->GetWidth();
        float screenHeight = (float)ServiceLocator::GetPlatformLayer()->GetHeight();

        //Create the splash logo Sprite object and add it to the Scene
        m_Logo = new Sprite("Triforce");
        m_Logo->SetLocalPosition(screenWidth / 2.0f, screenHeight / 2.0f);
        m_Logo->SetAnchorPoint(0.5f, 0.5f);
        m_Logo->SetAlpha(0.0f);
        AddGameObject(m_Logo);

        //Set the clear color to be white
        ServiceLocator::GetGraphics()->SetClearColor(Color::BlackColor());

        //Delay the FadeInLogo method 0.5 seconds
        DelaySceneMethod(&Splash::FadeInLogo, SPLASH_FADE_IN_DELAY);

        //Delay the TransitionToGame method 2.5 seconds
        DelaySceneMethod(&Splash::TransitionToGame, SPLASH_TRANSITION_TO_GAME_DELAY);

        return 1.0f;
    }

    double Splash::WillTransitionOff()
    {
        //Delay the FadeOutLogo method 0.5 seconds
        DelaySceneMethod(&Splash::FadeOutLogo, SPLASH_FADE_OUT_DELAY);

        //Returning 2 seconds, will delay the transition by 2 seconds so
        //that we can perform any closing transition animations
        return SPLASH_TRANSITION_TO_GAME_DELAY;
    }

    void Splash::FadeInLogo()
    {
        //Fade in the logo Sprite
        m_Logo->SetAlpha(1.0f, SPLASH_FADE_IN_DURATION);
    }

    void Splash::FadeOutLogo()
    {
        //Fade out the logo Sprite
        m_Logo->SetAlpha(0.0f, SPLASH_FADE_OUT_DURATION);
    }

    void Splash::TransitionToGame()
    {
        ServiceLocator::GetSceneManager()->PushScene(new MainMenu(), true);
    }
}