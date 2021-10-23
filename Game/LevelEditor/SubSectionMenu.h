//
//  SubSectionMenu.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-17.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__SubSectionMenu__
#define __GameDev2D__SubSectionMenu__

#include "../../Source/Core/Scene.h"

namespace GameDev2D
{
    //Local constants
    const double SUBSECTION_MENU_TRANSITION_ON_DURATION = 1.0;
    const double SUBSECTION_MENU_TRANSITION_OFF_DURATION = 1.5;
    const double SUBSECTION_MENU_BUTTON_SCALE_DURATION = 0.75;

    //Forward declarations
    class LevelEditor;
    class SubSection;
    class Rect;
    class Button;

    //The SubSection menu, will save and load a subsection when the Save() and Load() methods are called.
    //The SubSection menu will draw the LevelEditor in the background as a neat little bit of polish.
    class SubSectionMenu : public Scene
    {
    public:
        SubSectionMenu(LevelEditor* levelEditor);
        ~SubSectionMenu();

        //Loads the menu content
        float LoadContent();

        //Used to draw the Level editor in the background
        void Draw();

        //Handle's button action and keyboard events
        void HandleEvent(Event* event);

        //Used to notify when the menu will be transitioned on and off
        void WillTransitionOn();
        double WillTransitionOff();

        //Used to save a subsection, the parameter is the subsection that will be saved
        void Save(SubSection* subSection);

        //Used to load a subsection, the parameter is the subsection that will be loaded
        void Load(SubSection* subSection);

    private:
        //Conveniance methods to enable and disable button input
        void EnableButtons();
        void DisableButtons();

        //Member variables
        LevelEditor* m_LevelEditor;
        Rect* m_Overlay;
        Button** m_Buttons;
        SubSection* m_SaveSubSection;
        SubSection* m_LoadSubSection;
    };
}

#endif /* defined(__GameDev2D__SubSectionMenu__) */
