//
//  TextureFrame.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-08.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__TextureFrame__
#define __GameDev2D__TextureFrame__

#include "../../Core/BaseObject.h"
#include "../../Services/Graphics/Graphics.h"
#include "json.h"


namespace GameDev2D
{
    //TextureFrame constant
    const unsigned int TEXTURE_FRAME_VERTEX_COUNT = 4;
    
    //Forward declaration
    class Shader;

    //The SourceFrame struct holds position and
    //size data for a frame inside a Texture
    struct SourceFrame
    {
        SourceFrame()
        {
            position = vec2(0.0f, 0.0f);
            size = vec2(0.0f, 0.0f);
        }
        
        SourceFrame(vec2 p, vec2 s)
        {
            position = vec2(p.x, p.y);
            size = vec2(s.x, s.y);
        }
        
        SourceFrame(float x, float y, float width, float height)
        {
            position = vec2(x, y);
            size = vec2(width, height);
        }
        
        vec2 position;
        vec2 size;
    };


    //The TextureFrame class contains a Texture object as well a specific source frame of the Texture to render
    class TextureFrame : public BaseObject
    {
    public:
        //Loads a TextureFrame from a texture atlas, in a specific directory
        TextureFrame(const string& filename, const string& atlasKey, const string& directory);
        
        //Loads a TextureFrame for a specific width and height, usually used with the RenderTarget
        TextureFrame(unsigned int width, unsigned int height);
        
        //Loads a TextureFrame for a specific Texture object, flag to mark ownership of the Texture object
        TextureFrame(Texture* texture, bool ownsTexture);
        ~TextureFrame();
        
        //Draw the TextureFrame for a given model matrix
        void Draw(mat4 modelMatrix);
        
        //Returns the format of the Texture
        unsigned int GetFormat();
        
        //Returns the filename of the Texture
        string GetFilename();
        
        //Sets the source frame of the Texture to render
        void SetSourceFrame(SourceFrame sourceFrame);
        
        //Returns the source frame of the Texture to render
        SourceFrame GetSourceFrame();
        
        //Set the Shader that you want this TextureFrame to render with
        void SetShader(Shader* shader);
        
        //Returns the Shader that the TextureFrame is going to use to render with
        Shader* GetShader();
        
        //Sets the color to render the TextureFrame with
        void SetColor(Color color);
        
        //Returns the color that the TextureFrame will be rendered with
        Color GetColor();
        
    protected:
        //Called from the TextureManager
        Texture* GetTexture() const;
        void SetTexture(Texture* texture, bool ownsTexture);
        
        //Friend classes that need to access protected methods
        friend class TextureManager;
        friend class RenderTarget;
        friend class Graphics;
    
    private:
        //Enum of the vertex buffer elements
        enum
        {
            BufferObjectVertex = 1,
            BufferObjectUvCoordinates = 2,
            BufferObjectColors = 4,
            BufferObjectAll = BufferObjectVertex | BufferObjectUvCoordinates | BufferObjectColors
        };
    
        //Conveniance method to update part of the vertex buffer
        virtual void UpdateVertexBuffer(unsigned int bufferObjectType);

        //Member variables
        Texture* m_Texture;
        Shader* m_Shader;
        Color m_Color;
        SourceFrame m_SourceFrame;
        bool m_OwnsTexture;
        
        //VAO and VBO member variables
        unsigned int m_VertexArrayObject;
        unsigned int m_VertexBufferObject;
        unsigned int m_UvCoordinatesBufferObject;
        unsigned int m_ColorsBufferObject;
    };
}

#endif /* defined(__GameDev2D__TextureFrame__) */
