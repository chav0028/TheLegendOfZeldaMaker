//
//  FontManager.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-22.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__FontManager__
#define __GameDev2D__FontManager__

#include "../GameService.h"
#include "../../Graphics/Fonts/BitmapFont.h"
#include "../../Graphics/Fonts/TrueTypeFont.h"

#include <ft2build.h>       
#include FT_FREETYPE_H


using namespace std;

namespace GameDev2D
{
    //The FontManager leverages the FreeType font library to load truetype (.ttf) and open type (.otf) fonts.
    //The FontManager can also load Bitmap fonts that were created with 71Squared's Glyph Designer software.
    //Available at (https://71squared.com/en/glyphdesigner) . The FontManager uses reference counting to ensure
    //that the same font isn't loaded twice for BOTH TrueTypeFonts and BitmapFonts
    class FontManager : public GameService
    {
    public:
        FontManager();
        ~FontManager();
        
        //Creates and adds a bitmap font to the FontManager
        BitmapFont* AddBitmapFont(const string& bitmapFile);
        
        //Creates and adds a true type font to the FontManager
        TrueTypeFont* AddTrueTypeFont(const string& trueTypeFile, const string& extension, unsigned int fontSize, const string& characterSet);
        
        //Used to remove a font
        bool RemoveFont(Font* font);
        
        //Used to determine if the FontManager should be updated and drawn
        bool CanUpdate();
        bool CanDraw();
        
        //Handles events, we need to handle a low memory event
        void HandleEvent(Event* event);
        
    private:
        //Will purge any fonts with a reference count of zero
        void PurgeUnusedFonts();
        
        //The FreeType library used to load .ttf and .otf fonts
        FT_Library m_FreeTypeLibrary;

        //Member variables
        map<string, pair<BitmapFont*, unsigned int>> m_BitmapFontMap;
        multimap<string, pair<TrueTypeFont*, unsigned int>> m_TrueTypeFontMap;
    };
}

#endif /* defined(__GameDev2D__FontManager__) */
