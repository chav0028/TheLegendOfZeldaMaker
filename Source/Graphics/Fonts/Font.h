//
//  Font.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-24.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Font__
#define __GameDev2D__Font__

#include "../../Core/BaseObject.h"
#include "../Textures/TextureFrame.h"


using namespace std;

namespace GameDev2D
{
    //The is an abstract Font class, its an interface for both Bitmap and TrueType fonts
    class Font : public BaseObject
    {
    public:
        Font(const string& type, const string& filename);
        virtual ~Font();
        
        //Returns the filename of the font
        string GetFileName();
        
        //Returns the name of the font
        string GetFontName();
        
        //Returns the character set for the font, the characters that the font can render
        string GetCharacterSet();
        
        //Returns the line height, base line and font size for the font
        unsigned int GetLineHeight();
        unsigned int GetBaseLine();
        unsigned int GetFontSize();
        
        //Returns the TextureFrame for the character on the TextureFrame. //Method is abstract, must be
        //implemented by an inheriting class
        virtual TextureFrame* GetTextureFrameForCharacter(char character) = 0;
        
        //Returns the source frame for the character on the TextureFrame. //Method is abstract, must be
        //implemented by an inheriting class
        virtual SourceFrame GetSourceFrameForCharacter(char character) = 0;
        
        //Returns the x-advance for a character (how much to increase the spacing on the x-axis for said character).
        //Method is abstract, must be implemented by an inheriting class
        virtual unsigned short GetAdvanceXForCharacter(char character) = 0;
        
        //Returns the x and y bearing for a character. Method is abstract, must be implemented by an inheriting class
        virtual char GetBearingXForCharacter(char character) = 0;
        virtual char GetBearingYForCharacter(char character) = 0;
        
    protected:
        //Member variables
        string m_FileName;
        string m_FontName;
        string m_CharacterSet;
        unsigned int m_LineHeight;
        unsigned int m_BaseLine;
        unsigned int m_FontSize;
    };
}

#endif /* defined(__GameDev2D__Font__) */
