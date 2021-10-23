//
//  Font.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-24.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//


#include "Font.h"


namespace GameDev2D
{
    Font::Font(const string& aType, const string& aFilename) : BaseObject(aType),
        m_FileName(aFilename),
        m_CharacterSet(""),
        m_FontName(""),
        m_LineHeight(0),
        m_BaseLine(0),
        m_FontSize(0)
    {
    
    }
    
    Font::~Font()
    {
    
    }
    
    string Font::GetFileName()
    {
        return m_FileName;
    }
    
    string Font::GetFontName()
    {
        return m_FontName;
    }
    
    string Font::GetCharacterSet()
    {
        return m_CharacterSet;
    }
    
    unsigned int Font::GetLineHeight()
    {
        return m_LineHeight;
    }
    
    unsigned int Font::GetBaseLine()
    {
        return m_BaseLine;
    }
    
    unsigned int Font::GetFontSize()
    {
        return m_FontSize;
    }
}