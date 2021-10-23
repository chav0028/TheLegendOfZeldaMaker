//
//  KeyData.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-22.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__KeyData__
#define __GameDev2D__KeyData__

#include "../Core/BaseObject.h"


namespace GameDev2D
{
    //The KeyData class holds all relevant key data for one specific key code,
    //this class is managed by the InputManager.
    class KeyData : public BaseObject
    {
    public:
        KeyData();
        ~KeyData();
        
        //Sets wether the key has been pressed or not
        void SetIsPressed(bool isPressed);
        
        //Returns wether the key has been pressed or not
        bool IsPressed();
        
        //Returns the duration a key has been pressed for (if at all)
        double GetDuration();
        
        //Updates the key data, specifically to update the duration a
        //key has been pressed for.
        void Update(double delta);
        
    private:
        double m_Duration;
        bool m_IsPressed;
    };
}

#endif /* defined(__GameDev2D__KeyData__) */
