//
//  TextureManager.h
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-31.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__TextureManager__
#define __GameDev2D__TextureManager__

#include "../GameService.h"
#include "../../Graphics/Textures/TextureFrame.h"
#include "json.h"

using namespace std;

namespace GameDev2D
{
    //Forward declarations
    class Texture;

    //The TextureManager uses reference counting to ensure that the same texture isn't loaded twice.
    //It will also generate a default checkerboard texture if the texture being loaded doesn't exist.
    class TextureManager : public GameService
    {
    public:
        TextureManager();
        ~TextureManager();
        
        //Loads a texture (if its not already loaded) and increments the reference count if it is loaded.
        //If the texture doesn't exist a default checkerboard texture is loaded
        void AddTexture(const string& filename, const string& directory, TextureFrame* textureFrame = nullptr);
        
        //Decerements the reference count of a texture
        bool RemoveTexture(TextureFrame* textureFrame);
        bool RemoveTexture(const string& filename);

        //Conveniance methods to load a TextureFrame from a texture atlas
        SourceFrame* LoadAtlasFrame(const string& filename, const string& atlasKey, const string& directory);
        
        //Used to determine if the FontManager should be updated and drawn
        bool CanUpdate();
        bool CanDraw();
        
        //Handles events, we need to handle a low memory event
        void HandleEvent(Event* event);
        
    private:
        //
        SourceFrame* LoadFrame(const Json::Value& frame);

        //Will purge any textures with a reference count of zero
        void PurgeUnusedTextures();
        
        //Conveniance method to load and return the placeholder checkerboard texture
        //in the event a texture being loaded doesn't exist
        Texture* GetPlaceHolder();
        
        //Member variables
        map<string, pair<Texture*, unsigned int>> m_TextureMap;
        map<string, map<string, SourceFrame*>> m_AtlasKeys;
        Texture* m_PlaceHolder;
    };
}

#endif /* defined(__GameDev2D__GDTextureManager__) */
