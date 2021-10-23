//
//  Texture.h
//  GameDev2D
//
//  Created by Bradley Flood on 2013-08-01.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Texture__
#define __GameDev2D__Texture__

#include "../../Core/BaseObject.h"
#include "../OpenGL.h"
#include "png.h"


using namespace glm;
using namespace std;

namespace GameDev2D
{
    //TODO: Add more opengl texture properties and handle different formats
    //TODO: Better mipmap implementation


    //The Texture class is responsible for loading PNG texture data in OpenGL. It can
    //also be used to load image data manually or generate an empty texture.
    class Texture : public BaseObject
    {
    public:
        //Loads a Texture from a file name
        Texture(const string& filename);
        
        //Loads a Texture for a given width and height, format for the supplied texture data
        Texture(unsigned int width, unsigned int height, unsigned int format, void* data);
        
        //Loads an empty Texture, usually used for RenderTarget
        Texture(unsigned int width, unsigned int height);
        virtual ~Texture();

        //Returns the size of the texture
        uvec2 GetSize();
        
        //Returns the format of the texture
        unsigned int GetFormat();
        
        //Returns the id of the texture
        unsigned int GetId();
        
        //Returns the filename of the image used to generate the texture, can return
        //nothing if the texture wasn't generated from a PNG image
        string GetFilename();
        
    protected:
        //Loads the texture from a path, returns true if the loading was successful
        bool LoadFromPath(const string& path);
        
        //Set the png data
        void SetPngData(png_structp pngPointer, png_infop infoPointer);
        
        //Generates an open gl texture for the image data
        void GenerateTexture(void* data);
        
        //Friend classes, that need to access the protected methods
        friend class FontData;
        friend class TextureManager;
        
    private:
        //Member variables
        uvec2 m_Size;
        unsigned int m_MipmapLevel;
        unsigned int m_Format;
        unsigned int m_Id;
        string m_Filename;
    };
}

#endif /* defined(__GameDev2D__Texture__) */
