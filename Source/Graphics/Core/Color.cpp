//
//  Color.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-16.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//


#include "Color.h"
#include "../../Platforms/PlatformMacros.h"


namespace GameDev2D
{
    Color::Color() : BaseObject("Color")
    {
        Set(1.0f, 1.0f, 1.0f, 1.0f);
    }
    
    Color::Color(float aRed, float aGreen, float aBlue, float aAlpha) : BaseObject("Color")
    {
        Set(aRed, aGreen, aBlue, aAlpha);
    }
    
    Color::Color(unsigned char aRed, unsigned char aGreen, unsigned char aBlue, unsigned char aAlpha) : BaseObject("Color")
    {
        Set(aRed, aGreen, aBlue, aAlpha);
    }
    
    Color::Color(unsigned int aColor) : BaseObject("Color")
    {
        Set(aColor);
    }
    
    Color::~Color()
    {
        
    }
    
    void Color::SetRed(float aRed)
    {
        m_RGBA.r = fminf(fmaxf(aRed, 0.0f), 1.0f);
    }
    
    void Color::SetGreen(float aGreen)
    {
        m_RGBA.g = fminf(fmaxf(aGreen, 0.0f), 1.0f);
    }
    
    void Color::SetBlue(float aBlue)
    {
        m_RGBA.b = fminf(fmaxf(aBlue, 0.0f), 1.0f);
    }
    
    void Color::SetAlpha(float aAlpha)
    {
        m_RGBA.a = fminf(fmaxf(aAlpha, 0.0f), 1.0f);
    }
    
    void Color::Set(float aRed, float aGreen, float aBlue, float aAlpha)
    {
        SetRed(aRed);
        SetGreen(aGreen);
        SetBlue(aBlue);
        SetAlpha(aAlpha);
    }
    
    void Color::Set(unsigned char aRed, unsigned char aGreen, unsigned char aBlue, unsigned char aAlpha)
    {
        Set((float)aRed / 255.0f, (float)aGreen / 255.0f, (float)aBlue / 255.0f, (float)aAlpha / 255.0f);
    }
    
    void Color::Set(unsigned int aColor)
    {
        float red = (float)((aColor >> 24) & 0xff) / (float)(0xff);
        float green = (float)((aColor >> 16) & 0xff) / (float)(0xff);
        float blue = (float)((aColor >> 8) & 0xff) / (float)(0xff);
        float alpha = (float)((aColor) & 0xff) / (float)(0xff);
        Set(red, green, blue, alpha);
    }
    
    bool Color::operator==(const Color& aColor) const
    {
    	if(m_RGBA.r == aColor.m_RGBA.r && m_RGBA.g == aColor.m_RGBA.g && m_RGBA.b == aColor.m_RGBA.b && m_RGBA.a == aColor.m_RGBA.a)
        {
            return true;
        }

        return false;
    }
    
    bool Color::operator!=(const Color& aColor) const
    {
        return !(*this == aColor);
    }
    
    float Color::Red()
    {
        return m_RGBA.r;
    }
    
    float Color::Green()
    {
        return m_RGBA.g;
    }
    
    float Color::Blue()
    {
        return m_RGBA.b;
    }
    
    float Color::Alpha()
    {
        return m_RGBA.a;
    }
    
    vec4 Color::Get()
    {
        return m_RGBA;
    }
    
    void Color::Invert(bool aInvertAlpha)
    {
        SetRed(1.0f - Red());
        SetGreen(1.0f - Green());
        SetBlue(1.0f - Blue());
        
        if(aInvertAlpha == true)
        {
            SetAlpha(1.0f - Alpha());
        }
    }
    
    Color Color::ClearColor()
    {
        return Color(1.0f, 1.0f, 1.0f, 0.0f);
    }
    
    Color Color::BlackColor(float aAlpha)
    {
        return Color(0.0f, 0.0f, 0.0f, aAlpha);
    }
    
    Color Color::WhiteColor(float aAlpha)
    {
        return Color(1.0f, 1.0f, 1.0f, aAlpha);
    }
    
    Color Color::RedColor(float aAlpha)
    {
        return Color(1.0f, 0.0f, 0.0f, aAlpha);
    }
    
    Color Color::GreenColor(float aAlpha)
    {
        return Color(0.0f, 1.0f, 0.0f, aAlpha);
    }
    
    Color Color::BlueColor(float aAlpha)
    {
        return Color(0.0f, 0.0f, 1.0f, aAlpha);
    }
    
    Color Color::YellowColor(float aAlpha)
    {
        return Color(1.0f, 1.0f, 0.0f, aAlpha);
    }
    
    Color Color::CyanColor(float aAlpha)
    {
        return Color(0.0f, 1.0f, 1.0f, aAlpha);
    }
    
    Color Color::OrangeColor(float aAlpha)
    {
        return Color(1.0f, 0.6471f, 0.0f, aAlpha);
    }
    
    Color Color::GrayColor(float aAlpha)
    {
        return Color(0.5f, 0.5f, 0.5f, aAlpha);
    }
    
    Color Color::DarkGrayColor(float aAlpha)
    {
        return Color(0.262745098f, 0.2705882353f, 0.2588235294f, aAlpha);
    }
    
    Color Color::CharcoalColor(float aAlpha)
    {
        return Color((unsigned char)54, (unsigned char)69, (unsigned char)79, (unsigned char)(255.0f * aAlpha));
    }
    
    Color Color::LicoriceColor(float aAlpha)
    {
        return Color((unsigned char)26, (unsigned char)17, (unsigned char)16, (unsigned char)(255.0f * aAlpha));
    }
    
    Color Color::OnyxColor(float aAlpha)
    {
        return Color((unsigned char)53, (unsigned char)56, (unsigned char)57, (unsigned char)(255.0f * aAlpha));
    }
    
    Color Color::OuterSpaceBlackColor(float aAlpha)
    {
        return Color((unsigned char)65, (unsigned char)74, (unsigned char)76, (unsigned char)(255.0f * aAlpha));
    }
    
    Color Color::CrimsonRedColor(float aAlpha)
    {
        return Color((unsigned char)220, (unsigned char)20, (unsigned char)60, (unsigned char)(255.0f * aAlpha));
    }
    
    Color Color::FireEngineRedColor(float aAlpha)
    {
        return Color((unsigned char)206, (unsigned char)32, (unsigned char)41, (unsigned char)(255.0f * aAlpha));
    }
    
    Color Color::BarnRedColor(float aAlpha)
    {
        return Color((unsigned char)124, (unsigned char)10, (unsigned char)2, (unsigned char)(255.0f * aAlpha));
    }
    
    Color Color::AppleGreenColor(float aAlpha)
    {
        return Color((unsigned char)141, (unsigned char)182, (unsigned char)0, (unsigned char)(255.0f * aAlpha));
    }
    
    Color Color::ForestGreenColor(float aAlpha)
    {
        return Color((unsigned char)34, (unsigned char)139, (unsigned char)34, (unsigned char)(255.0f * aAlpha));
    }
    
    Color Color::ShamrockGreenColor(float aAlpha)
    {
        return Color((unsigned char)0, (unsigned char)158, (unsigned char)96, (unsigned char)(255.0f * aAlpha));
    }
    
    Color Color::LimeColor(float aAlpha)
    {
        return Color((unsigned char)191, (unsigned char)255, (unsigned char)0, (unsigned char)(255.0f * aAlpha));
    }
    
    Color Color::CornflowerBlueColor(float aAlpha)
    {
        return Color((unsigned char)100, (unsigned char)149, (unsigned char)237, (unsigned char)(255.0f * aAlpha));
    }
    
    Color Color::EgyptianBlueColor(float aAlpha)
    {
        return Color((unsigned char)16, (unsigned char)52, (unsigned char)166, (unsigned char)(255.0f * aAlpha));
    }
    
    Color Color::PowderBlueColor(float aAlpha)
    {
        return Color((unsigned char)176, (unsigned char)224, (unsigned char)230, (unsigned char)(255.0f * aAlpha));
    }
    
    Color Color::MellowYellowColor(float aAlpha)
    {
        return Color((unsigned char)248, (unsigned char)222, (unsigned char)126, (unsigned char)(255.0f * aAlpha));
    }
    
    Color Color::RoyalYellowColor(float aAlpha)
    {
        return Color((unsigned char)250, (unsigned char)218, (unsigned char)94, (unsigned char)(255.0f * aAlpha));
    }
    
    Color Color::SafetyYellowColor(float aAlpha)
    {
        return Color((unsigned char)238, (unsigned char)210, (unsigned char)2, (unsigned char)(255.0f * aAlpha));
    }
    
    Color Color::AntiqueWhiteColor(float aAlpha)
    {
        return Color((unsigned char)250, (unsigned char)235, (unsigned char)215, (unsigned char)(255.0f * aAlpha));
    }
    
    Color Color::FloralWhiteColor(float aAlpha)
    {
        return Color((unsigned char)255, (unsigned char)250, (unsigned char)240, (unsigned char)(255.0f * aAlpha));
    }
    
    Color Color::GhostWhiteColor(float aAlpha)
    {
        return Color((unsigned char)248, (unsigned char)248, (unsigned char)255, (unsigned char)(255.0f * aAlpha));
    }
}
