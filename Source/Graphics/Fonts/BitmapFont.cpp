//
//  BitmapFont.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2013-08-23.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//


#include "BitmapFont.h"
#include "../../IO/File.h"
#include "../Textures/TextureFrame.h"


namespace GameDev2D
{
    BitmapFont::BitmapFont(const string& aFilename) : Font("BitmapFont", aFilename),
        m_IsBold(false),
        m_IsItalic(false),
        m_IsUnicode(false),
        m_StretchH(0),
        m_IsSmooth(false),
        m_IsAntiAliased(false),
        m_Padding(SourceFrame(0.0f, 0.0f, 0.0f, 0.0f)),
        m_Spacing(vec2(0.0f, 0.0f)),
        m_TextureWidth(0),
        m_TextureHeight(0),
        m_NumberOfPages(0)
    {

    }
    
    BitmapFont::~BitmapFont()
    {
        //Cycle through and delete all the textures
        if (m_TextureFrames != nullptr)
        {
            for(unsigned int i = 0; i < m_Pages.size(); i++)
            {
                SafeDelete(m_TextureFrames[i]);
            }
            delete [] m_TextureFrames;
        }
        
        //Cycle through and delete the character set
        while (m_CharacterData.size() > 0)
        {
            SafeDelete(m_CharacterData.at(0));
            m_CharacterData.erase(m_CharacterData.begin());
        }
    }
    
    TextureFrame* BitmapFont::GetTextureFrameForCharacter(char aCharacter)
    {
        TextureFrame* textureFrame = nullptr;
        CharacterData* characterData = m_CharacterData[aCharacter];
        
        //Safety check the character data
        if(characterData != nullptr)
        {
            textureFrame = m_TextureFrames[characterData->m_PageId];
            textureFrame->SetSourceFrame(characterData->m_SourceFrame);
        }
        
        //Return the texture frame
        return textureFrame;
    }
    
    SourceFrame BitmapFont::GetSourceFrameForCharacter(char aCharacter)
    {
        CharacterData* characterData = m_CharacterData[aCharacter];
        if(characterData != nullptr)
        {
            return characterData->GetSourceFrame();
        }
        return SourceFrame(0.0f, 0.0f, 0.0f, 0.0f);
    }
    
    unsigned short BitmapFont::GetAdvanceXForCharacter(char aCharacter)
    {
        CharacterData* characterData = m_CharacterData[aCharacter];
        if(characterData != nullptr)
        {
            return characterData->GetAdvanceX();
        }
        return 0;
    }
    
    char BitmapFont::GetBearingXForCharacter(char aCharacter)
    {
        CharacterData* characterData = m_CharacterData[aCharacter];
        if(characterData != nullptr)
        {
            return (char)characterData->GetOffset().x;
        }
        return 0;
    }
    
    char BitmapFont::GetBearingYForCharacter(char aCharacter)
    {
        CharacterData* characterData = m_CharacterData[aCharacter];
        if(characterData != nullptr)
        {
            return (char)characterData->GetOffset().y;
        }
        return 0;
    }
    
    bool BitmapFont::IsBold()
    {
        return m_IsBold;
    }
    
    bool BitmapFont::IsItalic()
    {
        return m_IsItalic;
    }
    
    bool BitmapFont::IsUnicode()
    {
        return m_IsUnicode;
    }
    
    unsigned int BitmapFont::GetStretchH()
    {
        return m_StretchH;
    }
    
    bool BitmapFont::IsSmooth()
    {
        return m_IsSmooth;
    }
    
    bool BitmapFont::IsAntiAliased()
    {
        return m_IsAntiAliased;
    }
    
    SourceFrame BitmapFont::GetPadding()
    {
        return m_Padding;
    }
    
    vec2 BitmapFont::GetSpacing()
    {
        return m_Spacing;
    }
    
    unsigned int BitmapFont::GetTextureWidth()
    {
        return m_TextureWidth;
    }
    
    unsigned int BitmapFont::GetTextureHeight()
    {
        return m_TextureHeight;
    }
    
    unsigned int BitmapFont::GetNumberOfPages()
    {
        return m_NumberOfPages;
    }
    
    void BitmapFont::ParseXmlText(char* aXmlText)
    {
        //Parse the xml file
        xml_document<> doc;    // character type defaults to char
        doc.parse<0>(aXmlText);
        
        //Get the root font node
        xml_node<> *root = doc.first_node("font");
        
        //If the root is null, log and error and return
        if(root == nullptr)
        {
            Error(false, "Unable to load font: %s, xml file doesn't have a root node", GetFileName().c_str());
            return;
        }
        
        //Parse the font info data
        xml_node<> *info = root->first_node();
        ParseInfoNode(info);
        
        //Parse the font common data
        xml_node<> *common = info->next_sibling();
        ParseCommonNode(common);
        
        //Parse the font image files ('pages')
        xml_node<> *pages = common->next_sibling();
        ParsePagesNode(pages);
        
        //Parse the font characters
        xml_node<> *characters = pages->next_sibling();
        ParseCharactersNode(characters);

        //Now load all the textures used for this font
        m_TextureFrames = new TextureFrame *[m_Pages.size()];
        
        //Cycle through the pages and load each texture used for each page
        for(unsigned int i = 0; i < m_Pages.size(); i++)
        {
            //TODO: Ensure page id, aligns with texture array index
            string texture = m_Pages.at(i).second;
            m_TextureFrames[i] = new TextureFrame(texture, "", "Fonts/Bitmap/");
        }
    }
    
    void BitmapFont::ParseInfoNode(xml_node<> *aInfoNode)
    {
        for (xml_attribute<> *infoAttribute = aInfoNode->first_attribute(); infoAttribute; infoAttribute = infoAttribute->next_attribute())
        {
            string name = string(infoAttribute->name());
            string value = string(infoAttribute->value());
            
            if(name == "face")
            {
                m_FontName = string(value);
            }
            else if(name == "size")
            {
                m_FontSize = atoi(value.c_str());
            }
            else if(name == "bold")
            {
                m_IsBold = atoi(value.c_str()) == 1;
            }
            else if(name == "italic")
            {
                m_IsItalic = atoi(value.c_str()) == 1;
            }
            else if(name == "unicode")
            {
                m_IsUnicode = atoi(value.c_str()) == 1;
            }
            else if(name == "stretchH")
            {
                m_StretchH = atoi(value.c_str());
            }
            else if(name == "smooth")
            {
                m_IsSmooth = atoi(value.c_str()) == 1;
            }
            else if(name == "antialiasing")
            {
                m_IsAntiAliased = atoi(value.c_str()) == 1;
            }
            else if(name == "padding")
            {
                m_Padding = SourceFrameFromString(value);
            }
            else if(name == "spacing")
            {
                m_Spacing = Vec2FromString(value);
            }
        }
    }
    
    void BitmapFont::ParseCommonNode(xml_node<> *aCommonNode)
    {
        for (xml_attribute<> *commonAttribute = aCommonNode->first_attribute(); commonAttribute; commonAttribute = commonAttribute->next_attribute())
        {
            string name = string(commonAttribute->name());
            string value = string(commonAttribute->value());
            
            if(name == "lineHeight")
            {
                m_LineHeight = atoi(value.c_str());
            }
            else if(name == "base")
            {
                m_BaseLine = atoi(value.c_str());
            }
            else if(name == "scaleW")
            {
                m_TextureWidth = atoi(value.c_str());
            }
            else if(name == "scaleH")
            {
                m_TextureHeight = atoi(value.c_str());
            }
            else if(name == "pages")
            {
                m_NumberOfPages = atoi(value.c_str());
            }
        }
    }
    
    void BitmapFont::ParsePagesNode(xml_node<> *aPagesNode)
    {
        //parse the font image file
        for (xml_node<> *page = aPagesNode->first_node(); page; page = page->next_sibling())
        {
            //Pair to hold an id for an image filename
            pair<unsigned int, string> pagePair;
            
            //Cycle through the page attributes and get the id and file
            for (xml_attribute<> *pageAttribute = page->first_attribute(); pageAttribute; pageAttribute = pageAttribute->next_attribute())
            {
                string name = string(pageAttribute->name());
                string value = string(pageAttribute->value());
                
                if(name == "id")
                {
                    pagePair.first = atoi(value.c_str());
                }
                else if(name == "file")
                {
                    pagePair.second = string(value);
                }
            }
            
            //Push the page pair onto the pages vector
            m_Pages.push_back(pagePair);
        }
    }
    
    void BitmapFont::ParseCharactersNode(xml_node<> *aCharactersNode)
    {
        //Cycle through the characters and add them to the character map
        for (xml_node<> *character = aCharactersNode->first_node(); character; character = character->next_sibling())
        {
            //Create a new character data object
            CharacterData* characterData = new CharacterData();
            
            //Parse the character
            char characterId = characterData->ParseCharacterNode(character, this);
            
            #if LOG_BITMAP_FONT_GLYPH_DATA
            Log("Character Data for Character: %c - width: %f - height: %f - bearingX: %f - bearingY: %f - advance: %u", characterId, characterData->GetSourceFrame().size.x, characterData->GetSourceFrame().size.y, characterData->GetOffset().x, characterData->GetOffset().y, characterData->GetAdvanceX());
            #endif
            
            //Push back the character onto the character set vector
            m_CharacterSet.push_back(characterId);
            
            //Set the character data
            m_CharacterData[characterId] = characterData;
        }
    }
    
    SourceFrame BitmapFont::SourceFrameFromString(const string& aString)
    {
        SourceFrame sourceFrame;
        
        //Does the string contain a comma?
        size_t found = aString.find(",");
        if(found != std::string::npos)
        {
            size_t found2nd = aString.find(",", found + 1);
            if(found2nd != std::string::npos)
            {
                //Get the position and size vectors and set the return frame
                sourceFrame.position = Vec2FromString(aString.substr(0, found2nd));;
                sourceFrame.size = Vec2FromString(aString.substr(found2nd + 1, aString.size()));;
            }
        }
        
        //Return the return frame
        return sourceFrame;
    }
    
    vec2 BitmapFont::Vec2FromString(const string& aString)
    {
        vec2 returnPosition;
        
        //Does the string contain a comma?
        size_t found = aString.find(",");
        if(found != std::string::npos)
        {
            //Parse the x value
            std::string x = aString.substr(0, found);
            returnPosition.x = (float)atoi(x.c_str());
            
            //Parse the y value
            std::string y = aString.substr(found + 1, aString.size());
            returnPosition.y = (float)atoi(y.c_str());
        }
        
        //Return the return position
        return returnPosition;
    }
    
    
    CharacterData::CharacterData() :
        m_SourceFrame(SourceFrame(0.0f, 0.0f, 0.0f, 0.0f)),
        m_Offset(vec2(0.0f, 0.0f)),
        m_AdvanceX(0),
        m_PageId(0)
    {
        
    }
    
    CharacterData::~CharacterData()
    {
        
    }
    
    SourceFrame CharacterData::GetSourceFrame()
    {
        return m_SourceFrame;
    }
    
    vec2 CharacterData::GetOffset()
    {
        return m_Offset;
    }
    
    unsigned int CharacterData::GetAdvanceX()
    {
        return m_AdvanceX;
    }
    
    char CharacterData::ParseCharacterNode(xml_node<> *aCharacterNode, BitmapFont* aBitmapFont)
    {
        char character = '\0';
        
        //Cycle through the character node and load the font data
        for(xml_attribute<> *characterAttribute = aCharacterNode->first_attribute(); characterAttribute; characterAttribute = characterAttribute->next_attribute())
        {
            string name = string(characterAttribute->name());
            string value = string(characterAttribute->value());
            
            if(name == "id")
            {
                character = atoi(value.c_str());
            }
            else if(name == "x")
            {
                m_SourceFrame.position.x = (float)atof(value.c_str());
            }
            else if(name == "y")
            {
                m_SourceFrame.position.y = (float)atof(value.c_str());
            }
            else if(name == "width")
            {
                m_SourceFrame.size.x = (float)atof(value.c_str());
            }
            else if(name == "height")
            {
                m_SourceFrame.size.y = (float)atof(value.c_str());
            }
            else if(name == "xoffset")
            {
                m_Offset.x = (float)atof(value.c_str());
            }
            else if(name == "yoffset")
            {
                m_Offset.y = aBitmapFont->GetBaseLine() - (float)atof(value.c_str());
            }
            else if(name == "xadvance")
            {
                m_AdvanceX = atoi(value.c_str());
            }
            else if(name == "page")
            {
                m_PageId = atoi(value.c_str());
            }
        }
        
        return character;
    }
}
