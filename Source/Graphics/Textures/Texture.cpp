//
//  Texture.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2013-08-01.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "Texture.h"
#include "../../Services/ServiceLocator.h"
#include "../../Services/Graphics/Graphics.h"


namespace GameDev2D
{
    Texture::Texture(const string& aFilename) : BaseObject("Texture"),
        m_Size(uvec2(0, 0)),
        m_MipmapLevel(0),
        m_Format(0),
        m_Id(0),
        m_Filename(string(aFilename))
    {
        
    }
    
    Texture::Texture(unsigned int aWidth, unsigned int aHeight, unsigned int aFormat, void* aData) : BaseObject("Texture"),
        m_Size(uvec2(aWidth, aHeight)),
        m_MipmapLevel(0),
        m_Format(aFormat),
        m_Id(0),
        m_Filename("")
    {
        GenerateTexture(aData);
    }
    
    Texture::Texture(unsigned int aWidth, unsigned int aHeight) : BaseObject("Texture"),
        m_Size(uvec2(aWidth, aHeight)),
        m_MipmapLevel(0),
        m_Format(GL_RGBA),
        m_Id(0),
        m_Filename("")
    {
        GenerateTexture(0);
    }
    
    Texture::~Texture()
    {
        //Safety check the texture id
        if(m_Id != 0)
        {
            //If the texture we are about to delete is bound, we need to unbind it
            if(m_Id == ServiceLocator::GetGraphics()->GetBoundTextureId())
            {
                ServiceLocator::GetGraphics()->BindTexture(nullptr);
            }
        
            //Delete the texture
            glDeleteTextures(1, &m_Id);
            m_Id = 0;
        }
    }
    
    uvec2 Texture::GetSize()
    {
        return m_Size;
    }
    
    unsigned int Texture::GetFormat()
    {
        return m_Format;
    }
    
    unsigned int Texture::GetId()
    {
        return m_Id;
    }
    
    string Texture::GetFilename()
    {
        return m_Filename;
    }
    
    bool Texture::LoadFromPath(const string& aPath)
    {
        png_structp png_ptr;
        png_infop info_ptr;
        unsigned int sig_read = 0;
        FILE *file;
        
        //Open the image file
        if((file = fopen(aPath.c_str(), "rb")) == NULL)
        {
            return false;
        }
        
        //Create and initialize the png_struct with the desired error handler
        //functions.  If you want to use the default stderr and longjump method,
        //you can supply NULL for the last three parameters.  We also supply the
        //the compiler header file version, so that we know if the application
        //was compiled with a compatible version of the library.  REQUIRED
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if(png_ptr == NULL)
        {
            fclose(file);
            return false;
        }
        
        //Allocate/initialize the memory for image information.  REQUIRED.
        info_ptr = png_create_info_struct(png_ptr);
        if(info_ptr == NULL)
        {
            fclose(file);
            png_destroy_read_struct(&png_ptr, NULL, NULL);
            return false;
        }
        
        //Set error handling if you are using the setjmp/longjmp method (this is
        //the normal method of doing things with libpng).  REQUIRED unless you
        //set up your own error handlers in the png_create_read_struct() earlier.
        if(setjmp(png_jmpbuf(png_ptr)))
        {
            //Free all of the memory associated with the png_ptr and info_ptr
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(file);
            
            //If we get here, we had a problem reading the file
            return false;
        }
        
        //Set up the input control if you are using standard C streams
        png_init_io(png_ptr, file);
        
        //If we have already read some of the signature
        png_set_sig_bytes(png_ptr, sig_read);
        
        //If you have enough memory to read in the entire image at once,
        //and you need to specify only transforms that can be controlled
        //with one of the PNG_TRANSFORM_* bits (this presently excludes
        //quantizing, filling, setting background, and doing gamma
        //adjustment), then you can read the entire image (including
        //pixels) into the info structure with this call:
        png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
        
        //Set the texture data
        SetPngData(png_ptr, info_ptr);
        
        //Clean up after the read, and free any memory allocated - REQUIRED
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        
        //Close the file
        fclose(file);
        
        //If we reached here, the loading was successful
        return true;
    }
    
    void Texture::SetPngData(png_structp aPngPointer, png_infop aInfoPointer)
    {
        //Get the data, width and height of the image.
        png_size_t rowBytes = png_get_rowbytes(aPngPointer, aInfoPointer);
        png_bytepp rowPointers = png_get_rows(aPngPointer, aInfoPointer);
        m_Size.x = png_get_image_width(aPngPointer, aInfoPointer);
        m_Size.y = png_get_image_height(aPngPointer, aInfoPointer);
        unsigned char colorType = png_get_color_type(aPngPointer, aInfoPointer);
        
        //Set the texture color format
        m_Format = colorType == PNG_COLOR_TYPE_RGB ? GL_RGB : GL_RGBA;
        
        //Allocate the image data
        unsigned char* imageData = (unsigned char*)malloc(rowBytes * m_Size.y);
        
        //The png image is ordered top to bottom, but OpenGL expect it bottom to top so the order or swapped
        for (unsigned int i = 0; i < m_Size.y; i++)
        {
            memcpy(imageData + (rowBytes * (m_Size.y - 1 - i)), rowPointers[i], rowBytes);
        }
        
        //Generate the OpenGL texture with the image data
        GenerateTexture(imageData);
        
        //Free the image data it has served its purpose
        SafeFree(imageData);
    }
    
    void Texture::GenerateTexture(void* data)
    {
        //Use OpenGL ES to generate a name for the texture.
        glGenTextures(1, &m_Id);
        
        //Bind the texture name.
        ServiceLocator::GetGraphics()->BindTexture(this);
        
        //Set the texture parameters to use a minifying filter and a linear filer (weighted average)
        if(m_MipmapLevel > 0)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        //Specify a 2D texture image, provideing the a pointer to the image data in memory
        glTexImage2D(GL_TEXTURE_2D, m_MipmapLevel, m_Format, m_Size.x, m_Size.y, 0, m_Format, GL_UNSIGNED_BYTE, data);
    }
}
