//
//  Color.h
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-16.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Color__
#define __GameDev2D__Color__

#include "../../Core/BaseObject.h"
#include <glm.hpp>


using namespace glm;

namespace GameDev2D
{
    //Basic color class to manage RGBA color data
    class Color : public BaseObject
    {
    public:
        //Create the color white
        Color();
        
        //float range: 0.0f - 1.0f
        Color(float red, float green, float blue, float alpha = 1.0f);
        
        //unsigned char range: 0 - 255
        Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
        
        //Hexadecimal RGBA
        Color(unsigned int color);
        
        ~Color();
        
        //Setter methods
        void SetRed(float red);
        void SetGreen(float green);
        void SetBlue(float blue);
        void SetAlpha(float alpha);
        
        //float range: 0.0f - 1.0f
        void Set(float red, float green, float blue, float alpha);
        
        //unsigned char range: 0 - 255
        void Set(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
        
        //hexadecimal RGBA
        void Set(unsigned int color);
        
        //Overloaded operators
        bool operator==(const Color& color) const;
        bool operator!=(const Color& color) const;
        
        //Accessor methods
        float Red();
        float Green();
        float Blue();
        float Alpha();
        vec4 Get();
        
        //Inverts the color
        void Invert(bool invertAlpha = false);
        
        //Static color presets
        static Color ClearColor();
        static Color BlackColor(float alpha = 1.0f);
        static Color WhiteColor(float alpha = 1.0f);
        static Color RedColor(float alpha = 1.0f);
        static Color GreenColor(float alpha = 1.0f);
        static Color BlueColor(float alpha = 1.0f);
        static Color YellowColor(float alpha = 1.0f);
        static Color CyanColor(float alpha = 1.0f);
        static Color OrangeColor(float alpha = 1.0f);
        static Color GrayColor(float alpha = 1.0f);
        static Color DarkGrayColor(float alpha = 1.0f);
        
        static Color CharcoalColor(float alpha = 1.0f);
        static Color LicoriceColor(float alpha = 1.0f);
        static Color OnyxColor(float alpha = 1.0f);
        static Color OuterSpaceBlackColor(float alpha = 1.0f);
        
        static Color CrimsonRedColor(float alpha = 1.0f);
        static Color FireEngineRedColor(float alpha = 1.0f);
        static Color BarnRedColor(float alpha = 1.0f);
        
        static Color AppleGreenColor(float alpha = 1.0f);
        static Color ForestGreenColor(float alpha = 1.0f);
        static Color ShamrockGreenColor(float alpha = 1.0f);
        static Color LimeColor(float alpha = 1.0f);
        
        static Color CornflowerBlueColor(float alpha = 1.0f);
        static Color EgyptianBlueColor(float alpha = 1.0f);
        static Color PowderBlueColor(float alpha = 1.0f);
        
        static Color MellowYellowColor(float alpha = 1.0f);
        static Color RoyalYellowColor(float alpha = 1.0f);
        static Color SafetyYellowColor(float alpha = 1.0f);
        
        static Color AntiqueWhiteColor(float alpha = 1.0f);
        static Color FloralWhiteColor(float alpha = 1.0f);
        static Color GhostWhiteColor(float alpha = 1.0f);
        
    private:
        //Color array
        vec4 m_RGBA;
    };
}

#endif /* defined(__GameDev2D__Color__) */
