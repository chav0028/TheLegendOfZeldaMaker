//
//  TrueTypeFont.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-24.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//


#include "TrueTypeFont.h"
#include "../Core/RenderTarget.h"
#include "../../Platforms/PlatformLayer.h"
#include "../../Services/ServiceLocator.h"
#include "../../Services/FontManager/FontManager.h"


namespace GameDev2D
{
    TrueTypeFont::TrueTypeFont(FT_Library aLibrary, const string& aFilename, const string& aExtension, unsigned int aFontSize, const string& aCharacterSet) : Font("TrueTypeFont", aFilename),
        m_Face(nullptr),
        m_RenderTarget(nullptr)
    {
        string filename = string(aFilename);
    
        //Was the extension appended to the filename? If it was, remove it
        string extension = "." + aExtension;
        size_t found = filename.find(extension);
        if(found != std::string::npos)
        {
            filename.erase(found, aExtension.size() + 1);
        }
    
        //Get the json path
        string path = ServiceLocator::GetPlatformLayer()->GetPathForResourceInDirectory(filename.c_str(), aExtension.c_str(), "Fonts/TTF");
        
        //Verify the font exists, if it doesn't the assert below will be hit
        bool doesExist = ServiceLocator::GetPlatformLayer()->DoesFileExistAtPath(path);
        assert(doesExist == true);
        
        //If the font doesn't exist log an error and return
        if(doesExist == false)
        {
            Error(false, "Failed to find file: %s, for TrueTypeFont, the file doesn't exist", aFilename.c_str());
            return;
        }
        
        //Generate a font face for the desired ttf font
        if(FT_New_Face(aLibrary, path.c_str(), 0, &m_Face) != 0)
        {
            //Did the font face get successfully created? Log an error if it didn't
            Error(false, "Failed to create a font face for font file: %s", aFilename.c_str());
            return;
        }
        
        //Initialize the font name, size and character set
        m_FontName = string(m_Face->family_name);
        m_FontSize = aFontSize;
        m_CharacterSet = aCharacterSet;
        
        //Lastly, build the font atlas
        BuildFontAtlas();
    }
    
    TrueTypeFont::~TrueTypeFont()
    {
        //Cycle through and delete the character set
        while (m_GlyphData.size() > 0)
        {
            SafeDelete(m_GlyphData.begin()->second);
            m_GlyphData.erase(m_GlyphData.begin());
        }
    
        //Delete the Render Target
        SafeDelete(m_RenderTarget);
    
        //Cleanup the font face
        FT_Done_Face(m_Face);
    }
    
    TextureFrame* TrueTypeFont::GetTextureFrameForCharacter(char aCharacter)
    {
        //Texture frame we will return
        TextureFrame* textureFrame = nullptr;
        
        //Safety check the
        if(m_RenderTarget != nullptr)
        {
            //Get the texure frame from the render target and set the source frame for the desired character
            textureFrame = m_RenderTarget->GetTextureFrame();
            textureFrame->SetSourceFrame(GetSourceFrameForCharacter(aCharacter));
        }
        
        //Return the texture frame
        return textureFrame;
    }
    
    SourceFrame TrueTypeFont::GetSourceFrameForCharacter(char aCharacter)
    {
        GlyphData* glyphData = m_GlyphData[aCharacter];
        if(glyphData != nullptr)
        {
            return m_GlyphData[aCharacter]->sourceFrame;
        }
        return SourceFrame(0.0f, 0.0f, 0.0f, 0.0f);
    }
    
    unsigned short TrueTypeFont::GetAdvanceXForCharacter(char aCharacter)
    {
        GlyphData* glyphData = m_GlyphData[aCharacter];
        if(glyphData != nullptr)
        {
            return m_GlyphData[aCharacter]->advanceX;
        }
        return 0;
    }
    
    char TrueTypeFont::GetBearingXForCharacter(char aCharacter)
    {
        GlyphData* glyphData = m_GlyphData[aCharacter];
        if(glyphData != nullptr)
        {
            return m_GlyphData[aCharacter]->bearingX;
        }
        return 0;
    }
    
    char TrueTypeFont::GetBearingYForCharacter(char aCharacter)
    {
        GlyphData* glyphData = m_GlyphData[aCharacter];
        if(glyphData != nullptr)
        {
            return m_GlyphData[aCharacter]->bearingY;
        }
        return 0;
    }
    
    void TrueTypeFont::BuildFontAtlas()
    {
        //If this assert is hit, that means the font face couldn't be generated, 
        //that probably means the font doesn't exist
        assert(m_Face != nullptr);
        
        //Set the pixel size for the font
        FT_Set_Pixel_Sizes(m_Face, 0, m_FontSize);
    
        //Local variables used below
        unsigned int cellWidth = 0;
        unsigned int cellHeight = 0;
        int maxBearing = 0;
        int minHang = 0;
        
        //Reset the line height
        m_LineHeight = 0;
    
        //Cycle through all the characters in the character set
        for(unsigned int i = 0; i < m_CharacterSet.size(); i++)
        {
            //Get the character for the current index
            char character = m_CharacterSet.at(i);
        
            //Load the character
            if(FT_Load_Char(m_Face, character, FT_LOAD_RENDER) != 0)
            {
                //Did the font load the character? Log an error if it didn't
                Error(false, "Failed to load character: %c for font %s", character, m_FileName.c_str());
                
                //Continue loading characters
                continue;
            }

            //Get the glyph data for the character
            GlyphData* glyphData = m_GlyphData[character];

            //If the glyph data is null, create a new GlyphData object
            if(glyphData == nullptr)
            {
                glyphData = new GlyphData();
            }
            
            //Calculate the buffer size needed for the glyph
            size_t size = m_Face->glyph->bitmap.rows * m_Face->glyph->bitmap.width;
            glyphData->buffer = new unsigned char[size];
            
            //Cycle through and copy glyph data
            for (int j = 0; j < m_Face->glyph->bitmap.rows; j++)
            {
                int offsetA = (m_Face->glyph->bitmap.width * ((int)m_Face->glyph->bitmap.rows - 1 - j));
                int offsetB = (int)(size - offsetA - m_Face->glyph->bitmap.width);
                memcpy(glyphData->buffer + offsetA, m_Face->glyph->bitmap.buffer + offsetB, m_Face->glyph->bitmap.width);
            }

            //Set the glyph data properties
            glyphData->width = m_Face->glyph->bitmap.width;
            glyphData->height = m_Face->glyph->bitmap.rows;
            glyphData->advanceX = (unsigned char)(m_Face->glyph->advance.x / 64);
            glyphData->bearingX = m_Face->glyph->bitmap_left;
            glyphData->bearingY = m_Face->glyph->bitmap_top;
            
            //Calculate max bearing
            if(glyphData->bearingY > maxBearing)
            {
                maxBearing = glyphData->bearingY;
            }

            //Calculate max width
            if(glyphData->width > cellWidth)
            {
                cellWidth = glyphData->width;
            }
            
            //Calculate the line height
            if(glyphData->height > m_LineHeight)
            {
                m_LineHeight = glyphData->height;
            }

            //Calculate gylph hang
            int glyphHang = glyphData->bearingY - glyphData->height;
            if(glyphHang < minHang)
            {
                minHang = glyphHang;
            }
        
            //Log the glyph data details
            #if LOG_TRUE_TYPE_FONT_GLYPH_DATA
            Log("Glyph Data for Character: %c - width: %i - height: %i - bearingX: %i - bearingY: %i - advance: %i", character, glyphData->width, glyphData->height, glyphData->bearingX, glyphData->bearingY, glyphData->advanceX);
            #endif
            
            //Set the glyph data
            m_GlyphData[character] = glyphData;
        }
        
        //Create bitmap font
        cellHeight = maxBearing - minHang;
        
        //Set the baseline
        m_BaseLine = maxBearing;
        
        //Local variables used below
        unsigned int numberOfCells = (unsigned int)m_CharacterSet.size();
        unsigned int cellsPerRow = (unsigned int)ceil(sqrt(numberOfCells));
        unsigned int cellsPerColumn = cellsPerRow;
    
        //Set the render target size
        vec2 renderTargetSize(cellWidth * cellsPerRow, cellHeight * cellsPerColumn);
        
        //Delete the render target
        SafeDelete(m_RenderTarget);
        
        //Create a new render target, if the assert is hit, that means the render target failed to be created
        m_RenderTarget = new RenderTarget((int)renderTargetSize.x, (int)renderTargetSize.y);
        assert(m_RenderTarget != nullptr);

        //Safety check the render target
        if(m_RenderTarget != nullptr)
        {
            //Create the render target's frame buffer
            m_RenderTarget->CreateFrameBuffer();
            
            //Cache the render target
            RenderTarget* renderTarget = ServiceLocator::GetGraphics()->GetActiveRenderTarget();
            
            //Set the render target as the active render target
            ServiceLocator::GetGraphics()->SetActiveRenderTarget(m_RenderTarget);
            ServiceLocator::GetGraphics()->Clear();
        
            //Begin creating bitmap font
            SourceFrame sourceFrame(0.0f, 0.0f, (float)cellWidth, (float)cellHeight);

            //Local variables used below
            int offsetX = 0;
            int offsetY = 0;
            unsigned int rows = 0;
            unsigned int columns = 0;
            
            //Get the default passthrough shader
            Shader* shader = ServiceLocator::GetShaderManager()->GetPassthroughTextureShader();

            //Cycle through all the characters in the character set and render the glyph data to the render target
            for(unsigned int i = 0; i < m_CharacterSet.size(); i++)
            {
                //Get the character for the index
                char character = m_CharacterSet.at(i);
                
                //Get the glyph data for the character
                GlyphData* glyphData = m_GlyphData[character];

                //Safety check that the glyph data isn't null
                if(glyphData == nullptr)
                {
                    continue;
                }
    
                //Set base offsets
                offsetX = cellWidth * columns;
                offsetY = cellHeight * rows;

                //set the sourcce frame for the glyph
                sourceFrame.position.x = (float)offsetX;
                sourceFrame.position.y = renderTargetSize.y - offsetY - glyphData->height;
                sourceFrame.size.x = glyphData->width;
                sourceFrame.size.y = glyphData->height;
                glyphData->sourceFrame = sourceFrame;

                //Calculate the rgba data buffer size and then allocate it
                const size_t size = glyphData->width * glyphData->height;
                const int colorSize = 4;
                unsigned char* rgbaData = new unsigned char[size * colorSize];
                
                //Cycle through the glyph buffer data and set the rgba data and convert it to black and white
                int index = 0;
                for(unsigned int j = 0; j < size * 4; j += 4)
                {
                    unsigned char value = glyphData->buffer[index++];
                    
                    if(value > 0)
                    {
                        rgbaData[j] = 255;
                        rgbaData[j+1] = 255;
                        rgbaData[j+2] = 255;
                        rgbaData[j+3] = value;
                    }
                    else
                    {
                        rgbaData[j] = 0;
                        rgbaData[j+1] = 0;
                        rgbaData[j+2] = 0;
                        rgbaData[j+3] = 0;
                    }
                }
                
                //We can now delete the glyph buffer data
                SafeDeleteArray(m_GlyphData[character]->buffer);

                //Create a Texture object (on the stack) with the rgba data
                Texture texture(m_GlyphData[character]->width, m_GlyphData[character]->height, GL_RGBA, rgbaData);
                
                //We can now delete the rgba data
                SafeDeleteArray(rgbaData);
                
                //Create a TextureFrame object (on the stack) with the Texture object
                TextureFrame textureFrame(&texture, false);
                textureFrame.SetShader(shader);
                textureFrame.SetColor(Color::WhiteColor());
                
                //Initialize the model matrix for the texture frame and draw the rgba data
                mat4 modelMatrix = translate(mat4(1.0f), vec3(offsetX, offsetY, 0.0f));
                textureFrame.Draw(modelMatrix);

                //Increment the columns (and rows if necessary)
                columns++;
                if(columns == cellsPerColumn)
                {
                    rows++;
                    columns = 0;
                }
            }
            
            //Set the Graphic's original render target back
            ServiceLocator::GetGraphics()->SetActiveRenderTarget(renderTarget);
        }
    }
}