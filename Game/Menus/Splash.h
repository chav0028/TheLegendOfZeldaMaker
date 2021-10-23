//
//  Splash.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-08-26.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Splash__
#define __GameDev2D__Splash__

#include "../../Source/Core/Scene.h"


namespace GameDev2D
{
    //Local constants
    const double SPLASH_TRANSITION_TO_GAME_DELAY = 2.0;
    const double SPLASH_FADE_IN_DELAY = 0.5;
    const double SPLASH_FADE_OUT_DELAY = 0.5;
    const double SPLASH_FADE_IN_DURATION = 0.5;
    const double SPLASH_FADE_OUT_DURATION = 0.5;

    //Forward declarations
    class Sprite;
    class MainMenu;

    //The Splash class inherits from Scene, and is the first Scene that is displayed.
    //It will transition to the Main menu after 2.5 seconds
    class Splash : public Scene
    {
    public:
        Splash();
        ~Splash();

        //Load game content here
        float LoadContent();

        //Used to setup a transition off, where the Algonquin logo fades out
        double WillTransitionOff();

    private:
        //Conveniance methods used to fade the Algonquin logo in and out
        void FadeInLogo();
        void FadeOutLogo();

        //Conveniance method used to transition from the Splash to the Game
        void TransitionToGame();

        //Member variable
        Sprite* m_Logo;
    };
}

#endif /* defined(__GameDev2D__Splash__) */
