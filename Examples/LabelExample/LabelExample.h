//
//  LabelExample.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-08-12.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__LabelExample__
#define __GameDev2D__LabelExample__

#include "../../Source/Core/Scene.h"


namespace GameDev2D
{
    class Label;

    class LabelExample : public Scene
    {
    public:
        LabelExample();
        ~LabelExample();
        
        //Load all Game content in this method
        float LoadContent();

        //Handles input events used to trigger Label animations
        void HandleEvent(Event* event);
        
        //Resets everything back to its original state
        void Reset();
        
    private:
        //Member variables
        Label* m_BitmapLabel;
        Label* m_TrueTypeLabel;
    };
}

#endif /* defined(__GameDev2D__LabelExample__) */
