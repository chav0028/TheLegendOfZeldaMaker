//
//  TrueTypeFont.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-24.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__TrueTypeFont__
#define __GameDev2D__TrueTypeFont__

#include "Font.h"
#include "../Textures/TextureFrame.h"
#include <ft2build.h>
#include FT_FREETYPE_H


namespace GameDev2D
{
    //True type font Constants
    const string CHARACTER_SET_NUMERIC = "0123456789";
    const string CHARACTER_SET_ALPHA_LOWER = "abcdefghijklmnopqrstuvwxyz";
    const string CHARACTER_SET_ALPHA_UPPER = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const string CHARACTER_SET_ALPHA_FULL = CHARACTER_SET_ALPHA_LOWER + CHARACTER_SET_ALPHA_UPPER;
    const string CHARACTER_SET_ALPHA_NUMERIC = CHARACTER_SET_ALPHA_FULL + CHARACTER_SET_NUMERIC;
    const string CHARACTER_SET_ALPHA_NUMERIC_EXT = CHARACTER_SET_ALPHA_NUMERIC + " ,.?!'\"_+-=*:;#/\\@$%&^<>{}[]()";

    //Forward Declarations
    struct GlyphData;
    class RenderTarget;
    class Shader;

    //A TrueType font leverages the FreeType library to dynamically generate a texture atlas for a given font size and
    //character set. It renders the font data to a RenderTarget. This class supports both .TTF and .OTF font extensions.
    class TrueTypeFont : public Font
    {
    public:
        //A true type font should only be generated through the FontManager
        TrueTypeFont(FT_Library aLibrary, const string& filename, const string& extension, unsigned int fontSize, const string& characterSet);
        ~TrueTypeFont();

        //Returns the TextureFrame for a given character
        TextureFrame* GetTextureFrameForCharacter(char character);
        
        //Methods to get the source frame, x advance and bearing data for a given character
        SourceFrame GetSourceFrameForCharacter(char character);
        unsigned short GetAdvanceXForCharacter(char character);
        char GetBearingXForCharacter(char character);
        char GetBearingYForCharacter(char character);
    
    private:
        //Private method, to prevent being accidentally called
        void BuildFontAtlas();
    
        //Member variables
        FT_Face m_Face;
        map<char, GlyphData*> m_GlyphData;
        RenderTarget* m_RenderTarget;
    };
    
    //Struct to hold Glyph Data
    struct GlyphData
    {
        unsigned char width;      // width of the glyph in pixels
        unsigned char height;     // height of the glyph in pixels
        unsigned char advanceX;   // number of pixels to advance on x axis
        char bearingX;            // x offset of top-left corner from x axis
        char bearingY;            // y offset of top-left corner from y axis
        unsigned char* buffer;
        SourceFrame sourceFrame;
    };
}

#endif /* defined(__GameDev2D__TrueTypeFont__) */
