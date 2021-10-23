//
//  TextureManager.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-31.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "TextureManager.h"
#include "../ServiceLocator.h"
#include "../../Graphics/Textures/Texture.h"
#include "../../Graphics/Textures/TextureFrame.h"
#include "../../Platforms/PlatformLayer.h"
#include "../../Graphics/OpenGL.h"


namespace GameDev2D
{
    TextureManager::TextureManager() : GameService("TextureManager"),
        m_PlaceHolder(nullptr)
    {
        //Register for the low memory event
        ServiceLocator::GetPlatformLayer()->AddEventListener(this, LOW_MEMORY_WARNING_EVENT);
    }
    
    TextureManager::~TextureManager()
    {
        //Remove the listener for the low memory event
        ServiceLocator::GetPlatformLayer()->RemoveEventListener(this, LOW_MEMORY_WARNING_EVENT);
    
        //Delete the placeholder texture
        SafeDelete(m_PlaceHolder);
        
        //Unload all the textures
        while(m_TextureMap.size() > 0)
        {
            SafeDelete(m_TextureMap.begin()->second.first);
            m_TextureMap.erase(m_TextureMap.begin());
        }

        //Unload the atlas data
        while (m_AtlasKeys.size() > 0)
        {
            map<string, SourceFrame*> atlasFrames = m_AtlasKeys.begin()->second;
            while (atlasFrames.size() > 0)
            {
                SafeDelete(atlasFrames.begin()->second);
                atlasFrames.erase(atlasFrames.begin());
            }
            m_AtlasKeys.erase(m_AtlasKeys.begin());
        }
    }
    
    void TextureManager::AddTexture(const string& aFilename, const string& aDirectory, TextureFrame* aTextureFrame)
    {
        //If this assert was triggered, the filename passed in didn't contain anything
        assert(aFilename.length() > 0);
    
        //Safety check the filename
        if(aFilename.length() == 0)
        {
            Error(false, "Failed to load texture, the filename has a length of zero");
            return;
        }
        
        //Append the filename to the directory
        string filename = string(aFilename);
        
        //Was .png appended to the filename? If it was, remove it
        size_t found = filename.find(".png");
        if(found != std::string::npos)
        {
            filename.erase(found, 4);
        }
        
        //Get the path for the texture
        string path = ServiceLocator::GetPlatformLayer()->GetPathForResourceInDirectory(filename.c_str(), "png", aDirectory.c_str());
        
        //Does the image exist at the path
        bool doesExist = ServiceLocator::GetPlatformLayer()->DoesFileExistAtPath(path);
        
        //If the path isn't empty, set the place holder texture
        if(doesExist == false)
        {
            Error(false, "Failed to load texture with filename: %s, it doesn't exist", aFilename.c_str());
            
            //Safety check the texture frame and set the placeholder texture
            if(aTextureFrame != nullptr)
            {
                aTextureFrame->SetTexture(GetPlaceHolder(), false);
            }
            return;
        }
    
        //Get the pair from the texture map
        pair<Texture*, unsigned int> texturePair = m_TextureMap[aFilename];
        
        //Is the texture pointer null?
        if(texturePair.first == nullptr)
        {
            //Create a new texture object, set the retain count to 1
            texturePair.first = new Texture(aFilename);
            texturePair.second = 1;
            
            //Load the texture from the path
            if(texturePair.first->LoadFromPath(path) == false)
            {
                //Delete the texture
                SafeDelete(texturePair.first);
                
                //Log and error and return out of this method
                Error("Failed to load texture: %s", aFilename.c_str());
                return;
            }
        }
        else
        {
            //Increment the reference count
            texturePair.second++;
        }
        
        //Set the texture map pair for the filename key
        m_TextureMap[aFilename] = texturePair;
        
        //Set the texture frame's texture
        if(aTextureFrame != nullptr)
        {
            aTextureFrame->SetTexture(texturePair.first, true);
        }
    }
    
    bool TextureManager::RemoveTexture(TextureFrame* aTextureFrame)
    {
        return RemoveTexture(aTextureFrame->GetFilename());
    }
    
    bool TextureManager::RemoveTexture(const string& aFilename)
    {
        bool success = false;
    
        //Get the pair from the texture map
        pair<Texture*, unsigned int> texturePair = m_TextureMap[aFilename];
        
        //Is the texture pointer null?
        if(texturePair.first != nullptr)
        {
            //Subract from the retain counter
            texturePair.second--;
            
            //Has the counter reacher zero?
            if(texturePair.second <= 0)
            {
                texturePair.second = 0;
            }

            //Set the texture map pair for the filename key
            m_TextureMap[aFilename] = texturePair;
            
            //If the purge unused texture congif is enabled, purge the texture
            //immediately if the retain count reaches zero
            #if PURGE_UNUSED_TEXTURES_IMMEDIATELY
            if(texturePair.second == 0)
            {
                PurgeUnusedTextures();
            }
            #endif
            
            //Set the success flag to true
            success = true;
        }
        
        return success;
    }

    SourceFrame* TextureManager::LoadAtlasFrame(const string& aFilename, const string& aAtlasKey, const string& aDirectory)
    {
        //Get the pair from the texture map
        map<string, SourceFrame*> atlasFrames = m_AtlasKeys[aFilename];
        SourceFrame* sourceFrame = atlasFrames[aAtlasKey];

        //Is the SourceFrame pointer null?
        if (sourceFrame == nullptr)
        {
            //Append the filename to the directory
            string filename = string(aFilename);

            //Was .json appended to the filename? If it was, remove it
            size_t found = filename.find(".json");
            if (found != std::string::npos)
            {
                filename.erase(found, 5);
            }

            //Get the json path
            string jsonPath = ServiceLocator::GetPlatformLayer()->GetPathForResourceInDirectory(filename.c_str(), "json", aDirectory.c_str());

            //Does the json file exist, if it doesn't the assert below will be hit
            bool doesExist = ServiceLocator::GetPlatformLayer()->DoesFileExistAtPath(jsonPath);
            assert(doesExist == true);

            //If the json files exists, load the atlas frae
            if (doesExist == true)
            {
                //Open the input stream
                ifstream inputStream;
                inputStream.open(jsonPath.c_str(), std::ifstream::in);

                //Parse the json data
                Json::Value root;
                Json::Reader reader;
                if (reader.parse(inputStream, root, false) == true)
                {
                    //Get the frames
                    const Json::Value frames = root["frames"];

                    //Cycle through the frames and parse the frame data
                    for (unsigned int index = 0; index < frames.size(); ++index)
                    {
                        //Safety check the filename
                        if (frames[index]["filename"].isString() == true)
                        {
                            //Get the atlas key
                            string atlasKey = string(frames[index]["filename"].asString());

                            //Load the source frame
                            SourceFrame* frame = LoadFrame(frames[index]["frame"]);
                            atlasFrames[atlasKey] = frame;

                            //If the atlaskey matches set the source frame pointer
                            if (atlasKey == aAtlasKey)
                            {
                                sourceFrame = frame;
                            }
                        }
                    }

                    //Set the atlas 
                    m_AtlasKeys[aFilename] = atlasFrames;
                }
                else
                {
                    //The json file does not exist, log an error
                    Error(false, "Failed to parse JSON file: %s for atlas key: %s", aFilename.c_str(), aAtlasKey.c_str());
                }

                //Close the input stream
                inputStream.close();
            }
            else
            {
                //The json file does not exist, log an error
                Error(false, "Failed to parse texture atlas JSON: %s for atlas key: %s, the json file does not exist", aFilename.c_str(), aAtlasKey.c_str());
            }
        }

        //Return the source frame
        return sourceFrame;
    }

    SourceFrame* TextureManager::LoadFrame(const Json::Value& aFrame)
    {
        //Get the values from the frame
        unsigned int x = aFrame["x"].asInt();
        unsigned int y = aFrame["y"].asInt();
        unsigned int width = aFrame["w"].asInt();
        unsigned int height = aFrame["h"].asInt();

        //Return the source frame
        SourceFrame* sourceFrame = new SourceFrame((float)x, (float)y, (float)width, (float)height);
        return sourceFrame;
    }
    
    bool TextureManager::CanUpdate()
    {
        return false;
    }
    
    bool TextureManager::CanDraw()
    {
        return false;
    }
    
    void TextureManager::HandleEvent(Event* aEvent)
    {
        if(aEvent->GetEventCode() == LOW_MEMORY_WARNING_EVENT)
        {
            PurgeUnusedTextures();
        }
    }
    
    void TextureManager::PurgeUnusedTextures()
    {
        //Cycle through the texture map and purge any textures with a reference count of zero
        for(map<string, pair<Texture*, unsigned int>>::iterator it = m_TextureMap.begin(); it != m_TextureMap.end();)
        {
            //Is the reference count zero
            if(it->second.second == 0)
            {
                //Delete the texture object
                SafeDelete(it->second.first);
                
                //Erase the pointer from the texture map
                map<string, pair<Texture*, unsigned int>>::iterator eraseIt = it;
                ++it;
                m_TextureMap.erase(eraseIt);
            }
            else
            {
                it++;
            }
        }
    }
    
    Texture* TextureManager::GetPlaceHolder()
    {
        if(m_PlaceHolder == nullptr)
        {
            //Local constants
            const int placeHolderSize = 128;
            const int colorSize = 4;

            //Local variables used below
            int checkerSize = placeHolderSize >> 1;
            int rowBytes = placeHolderSize * colorSize;
            Color colorA(0x555555c8);
            Color colorB(0xbbbbbbc8);
            
            //Create our image buffer
            unsigned int dataSize = colorSize * placeHolderSize * placeHolderSize;
            unsigned char* data = new unsigned char[dataSize];
            
            //Cycle through and set the checkered board pattern
            int index = 0;
            for (int i = 0; i < checkerSize; ++i)
            {
                for (int j = 0; j < checkerSize; ++j)
                {
                    //Red
                    data[index] = data[index + checkerSize * (colorSize + rowBytes)] = (unsigned char)(colorA.Red() * 255.0f);
                    data[index + checkerSize * colorSize] = data[index + checkerSize * rowBytes] = (unsigned char)(colorB.Red() * 255.0f);
                    index++;
                    
                    //Green
                    data[index] = data[index + checkerSize * (colorSize + rowBytes)] = (unsigned char)(colorA.Green() * 255.0f);
                    data[index + checkerSize * colorSize] = data[index + checkerSize * rowBytes] = (unsigned char)(colorB.Green() * 255.0f);
                    index++;
                    
                    //Blue
                    data[index] = data[index + checkerSize * (colorSize + rowBytes)] = (unsigned char)(colorA.Blue() * 255.0f);
                    data[index + checkerSize * colorSize] = data[index + checkerSize * rowBytes] = (unsigned char)(colorB.Blue() * 255.0f);
                    index++;
                    
                    //Alpha
                    data[index] = data[index + checkerSize * (colorSize + rowBytes)] = (unsigned char)(colorA.Alpha() * 255.0f);
                    data[index + checkerSize * colorSize] = data[index + checkerSize * rowBytes] = (unsigned char)(colorB.Alpha() * 255.0f);
                    index++;
                }
                index += checkerSize * colorSize;
            }
            
            //Create the texture object
            m_PlaceHolder = new Texture(placeHolderSize, placeHolderSize, GL_RGBA, data);
            
            //Delete the data, we are done with it now
            SafeDeleteArray(data);
        }
        
        //Return the placeholder texture
        return m_PlaceHolder;
    }
}