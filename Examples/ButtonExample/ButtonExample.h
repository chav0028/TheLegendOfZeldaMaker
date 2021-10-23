//
//  ButtonExample.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-08-12.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__ButtonExample__
#define __GameDev2D__ButtonExample__

#include "../../Source/Core/Scene.h"
#include <string>


using namespace std;

namespace GameDev2D
{
    //Local constants
    #define BUTTON_COUNT 4
    const string BUTTON_LABELS[] = {"Play", "Options", "High Scores", "Quit"};

    class Button;

    class ButtonExample : public Scene
    {
    public:
        ButtonExample();
        ~ButtonExample();
        
        //Load all Game content in this method
        float LoadContent();

        //Handles input and button events
        void HandleEvent(Event* event);
        
        //Resets everything back to its original state
        void Reset();
        
    private:
        //Member variables
        Button* m_Buttons[BUTTON_COUNT];
    };
}

#endif /* defined(__GameDev2D__ButtonExample__) */
