//
//  FontManager.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-22.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "FontManager.h"
#include "../ServiceLocator.h"
#include "../../Platforms/PlatformLayer.h"
#include "../../IO/File.h"


namespace GameDev2D
{
    FontManager::FontManager() : GameService("FontManager")
    {
        //Initialize the FreeType lib, returns zero if successful
        if(FT_Init_FreeType(&m_FreeTypeLibrary) != 0)
        {
            //The FreeType lib failed to initialize
            Error(true, "The FreeType lib failed to initialize");
        }
        
        //Register for the low memory warning
        ServiceLocator::GetPlatformLayer()->AddEventListener(this, LOW_MEMORY_WARNING_EVENT);
    }

    FontManager::~FontManager()
    {
        //Remove the listener for the low memory warning
        ServiceLocator::GetPlatformLayer()->RemoveEventListener(this, LOW_MEMORY_WARNING_EVENT);
    
        //Cycle through and release any existing system sounds
        while(m_BitmapFontMap.size() > 0)
        {
            SafeDelete(m_BitmapFontMap.begin()->second.first);
            m_BitmapFontMap.erase(m_BitmapFontMap.begin());
        }
        
        //Cycle through and release any existing system sounds
        while(m_TrueTypeFontMap.size() > 0)
        {
            SafeDelete(m_TrueTypeFontMap.begin()->second.first);
            m_TrueTypeFontMap.erase(m_TrueTypeFontMap.begin());
        }
        
        //Free the FreeType library
        FT_Done_FreeType(m_FreeTypeLibrary);
    }

    BitmapFont* FontManager::AddBitmapFont(const string& aBitmapFile)
    {
        //Safety check the filename
        if(aBitmapFile.length() == 0)
        {
            Error(false, "Failed to load BitmapFont, the file has a length of 0");
            return nullptr;
        }
    
        //Get the pair from the texture map
        pair<BitmapFont*, unsigned int> fontPair = m_BitmapFontMap[aBitmapFile];
        
        //Is the texture pointer null?
        if(fontPair.first == nullptr)
        {
            //Was .fnt appended to the filename? If it was, remove it
            string bitmapFile = aBitmapFile;
            size_t found = bitmapFile.find(".fnt");
            if(found != std::string::npos)
            {
                bitmapFile.erase(found, 4);
            }
            
            //Load the font xml file
            string fontPath = ServiceLocator::GetPlatformLayer()->GetPathForResourceInDirectory(bitmapFile.c_str(), "fnt", "Fonts/Bitmap");
            File fontFile(fontPath);
            
            //Safety check the buffer size
            if(fontFile.GetBufferSize() == 0)
            {
                //TODO: log error
                return nullptr;
            }
            
            //Create a new font object
            BitmapFont* font = new BitmapFont(aBitmapFile);
            font->ParseXmlText(const_cast<char*>(fontFile.GetBuffer()));

            //Set the font object, set the retain count to 1
            fontPair.first = font;
            fontPair.second = 1;
        }
        else
        {
            //Increment the reference count
            fontPair.second++;
        }
        
        //Set the font map pair for the filename key
        m_BitmapFontMap[aBitmapFile] = fontPair;

        //Return the font object
        return fontPair.first;
    }
    
    TrueTypeFont* FontManager::AddTrueTypeFont(const string& aTrueTypeFile, const string& aExtension, unsigned int aFontSize, const string& aCharacterSet)
    {
        //Safety check the filename
        if(aTrueTypeFile.length() == 0)
        {
            Error(false, "Failed to load TrueTypeFont, the file has a length of 0");
            return nullptr;
        }
    
        //Cycle through the multimap and verify the font size is the same
        pair<TrueTypeFont*, unsigned int>* foundPair = nullptr;
        for(multimap<string, pair<TrueTypeFont*, unsigned int>>::iterator it = m_TrueTypeFontMap.find(aTrueTypeFile); it != m_TrueTypeFontMap.end(); ++it)
        {
            if(it->second.first->GetFontSize() == aFontSize && it->second.first->GetFontName() == aTrueTypeFile)
            {
                foundPair = &it->second;
            }
        }
        
        //If the font is already loaded, increment the reference count and return the font
        if(foundPair != nullptr)
        {
            foundPair->second++;
            return foundPair->first;
        }

        //Create a new true type font
        TrueTypeFont* font = new TrueTypeFont(m_FreeTypeLibrary, aTrueTypeFile, aExtension, aFontSize, aCharacterSet);
        
        //Create a font pair and set the reference count to 1
        pair<TrueTypeFont*, unsigned int> pair;
        pair.first = font;
        pair.second = 1;
        
        //Insert the true type font pair
        m_TrueTypeFontMap.insert(make_pair(aTrueTypeFile, pair));
        
        //Return the true type font
        return font;
    }

    bool FontManager::RemoveFont(Font* aFont)
    {
        bool success = false;
    
        //Get the name attached to the shader
        string name = aFont->GetFileName();
    
        //What type of font are we unloading
        if(aFont->GetType() == "BitmapFont")
        {
            //Get the pair from the bitmap font map
            pair<BitmapFont*, unsigned int> fontPair = m_BitmapFontMap[name];
            
            //Is the font pointer null?
            if(fontPair.first == aFont)
            {
                //Subract from the retain counter
                fontPair.second--;
                
                //Has the counter reacher zero?
                if(fontPair.second <= 0)
                {
                    fontPair.second = 0;
                }
    
                //Set the bitmap font map pair for the name key
                m_BitmapFontMap[name] = fontPair;

                //Do we purge unused fonts immediately?
                #if PURGE_UNUSED_FONTS_IMMEDIATELY
                if(fontPair.second == 0)
                {
                    PurgeUnusedFonts();
                }
                #endif
                
                //Set the success flag to true
                success = true;
            }
        }
        else if(aFont->GetType() == "TrueTypeFont")
        {
            //Cycle through the multimap and decrease the reference counter
            for(multimap<string, pair<TrueTypeFont*, unsigned int>>::iterator it = m_TrueTypeFontMap.find(name); it != m_TrueTypeFontMap.end(); ++it)
            {
                //Did we find the font
                if(it->second.first == aFont)
                {
                    //Subract from the retain counter
                    it->second.second--;
                
                    //Has the counter reacher zero?
                    if(it->second.second <= 0)
                    {
                       it->second.second = 0;
                       
                        #if PURGE_UNUSED_FONTS_IMMEDIATELY
                        PurgeUnusedFonts();
                        #endif
                    }

                    //Set the success flag to true
                    success = true;
                    
                    //Break out of the loop
                    break;
                }
            }
        }
        
        //Return the success flag
        return success;
    }

    bool FontManager::CanUpdate()
    {
        return false;
    }
    
    bool FontManager::CanDraw()
    {
        return false;
    }
    
    void FontManager::HandleEvent(Event* aEvent)
    {
        if(aEvent->GetEventCode() == LOW_MEMORY_WARNING_EVENT)
        {
            PurgeUnusedFonts();
        }
    }
    
    void FontManager::PurgeUnusedFonts()
    {
        //Cycle through and purge the unused Bitmap fonts
        for(map<string, pair<BitmapFont*, unsigned int>>::iterator it = m_BitmapFontMap.begin(); it != m_BitmapFontMap.end();)
        {
            //If the reference count is zero we can delete the font
            if(it->second.second == 0)
            {
                //Delete the font
                SafeDelete(it->second.first);
                
                //Remove it from the font map
                map<string, pair<BitmapFont*, unsigned int>>::iterator eraseIt = it;
                ++it;
                m_BitmapFontMap.erase(eraseIt);
            }
            else
            {
                it++;
            }
        }
        
        //Cycle through and purge the unused TrueType fonts
        for(multimap<string, pair<TrueTypeFont*, unsigned int>>::iterator it = m_TrueTypeFontMap.begin(); it != m_TrueTypeFontMap.end();)
        {
            //If the reference count is zero we can delete the font
            if(it->second.second == 0)
            {
                //Delete the font
                SafeDelete(it->second.first);
                
                //Remove it from the font multimap
                multimap<string, pair<TrueTypeFont*, unsigned int>>::iterator eraseIt = it;
                ++it;
                m_TrueTypeFontMap.erase(eraseIt);
            }
            else
            {
                it++;
            }
        }
    }
}