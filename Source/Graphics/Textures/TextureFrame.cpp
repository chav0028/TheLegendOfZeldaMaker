//
//  TextureFrame.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-08.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "TextureFrame.h"
#include "../Core/Shader.h"
#include "../../Platforms/PlatformLayer.h"
#include "../../Services/ServiceLocator.h"
#include "../../Services/ShaderManager/ShaderManager.h"


namespace GameDev2D
{
    TextureFrame::TextureFrame(const string& aFilename, const string& aAtlasKey, const string& aDirectory) : BaseObject("TextureFrame"),
        m_Texture(nullptr),
        m_Shader(nullptr),
        m_Color(Color::WhiteColor()),
        m_SourceFrame(SourceFrame(0.0f, 0.0f, 0.0f, 0.0f)),
        m_OwnsTexture(true),
        m_VertexArrayObject(0),
        m_VertexBufferObject(0),
        m_UvCoordinatesBufferObject(0),
        m_ColorsBufferObject(0)
    {
        //Load the texture via the texture manager, this will ensure only one texture with this filename will be loaded
        ServiceLocator::GetTextureManager()->AddTexture(aFilename, aDirectory, this);
        
        //Safety check the atlas key
        if(aAtlasKey != "")
        {
            //Load the atlas frame
            SourceFrame* sourceFrame = ServiceLocator::GetTextureManager()->LoadAtlasFrame(aFilename, aAtlasKey, aDirectory);
            m_SourceFrame.position = sourceFrame->position;
            m_SourceFrame.size = sourceFrame->size;
        }
        else
        {
            //Set the source frame's size
            m_SourceFrame.size = m_Texture->GetSize();
        }
    }
    
    TextureFrame::TextureFrame(unsigned int aWidth, unsigned int aHeight) : BaseObject("TextureFrame"),
        m_Texture(nullptr),
        m_Shader(nullptr),
        m_Color(Color::WhiteColor()),
        m_SourceFrame(SourceFrame(0.0f, 0.0f, (float)aWidth, (float)aHeight)),
        m_OwnsTexture(true),
        m_VertexArrayObject(0),
        m_VertexBufferObject(0),
        m_UvCoordinatesBufferObject(0),
        m_ColorsBufferObject(0)
    {
        //Create a new texture object for the width and height
        m_Texture = new Texture(aWidth, aHeight);
    }
    
    TextureFrame::TextureFrame(Texture* aTexture, bool aOwnsTexture) : BaseObject("TextureFrame"),
        m_Texture(aTexture),
        m_Shader(nullptr),
        m_Color(Color::WhiteColor()),
        m_SourceFrame(SourceFrame(0.0f, 0.0f, (float)m_Texture->GetSize().x, (float)m_Texture->GetSize().y)),
        m_OwnsTexture(aOwnsTexture),
        m_VertexArrayObject(0),
        m_VertexBufferObject(0),
        m_UvCoordinatesBufferObject(0),
        m_ColorsBufferObject(0)
    {

    }
    
    TextureFrame::~TextureFrame()
    {
        //Unload the texture
        if(m_OwnsTexture == true && m_Texture != nullptr)
        {
            //Remove the Texture from the TextureManager and delete the Texture object
            if(ServiceLocator::GetTextureManager()->RemoveTexture(this) == false)
            {
                delete m_Texture;
            }
            m_Texture = nullptr;
        }
        
        //Set the Shader to null
        m_Shader = nullptr;
        
        //Delete the VBO
        if(m_VertexBufferObject != 0)
        {
            glDeleteBuffers(1, &m_VertexBufferObject);
            m_VertexBufferObject = 0;
        }
        
        //Delete the UV Coordinate buffer object
        if(m_UvCoordinatesBufferObject != 0)
        {
            glDeleteBuffers(1, &m_UvCoordinatesBufferObject);
            m_UvCoordinatesBufferObject = 0;
        }
        
        //Delete the color buffer object
        if(m_ColorsBufferObject != 0)
        {
            glDeleteBuffers(1, &m_ColorsBufferObject);
            m_ColorsBufferObject = 0;
        }

        //Delete the VAO
        if(m_VertexArrayObject != 0)
        {
            glDeleteVertexArrays(1, &m_VertexArrayObject);
            m_VertexArrayObject = 0;
        }
    }
    
    void TextureFrame::Draw(mat4 aModelMatrix)
    {
        //Set the shader to be used
        m_Shader->Use();

        //Multiply the model matrix by the projection and view matrices
        Graphics* graphics = ServiceLocator::GetGraphics();
        
        //Bind the vertex array object
        graphics->BindVertexArray(m_VertexArrayObject);

        //Set the model view projection matrix
        mat4 mvp = graphics->GetProjectionMatrix() * graphics->GetViewMatrix() * aModelMatrix;
        glUniformMatrix4fv(m_Shader->GetModelViewProjectionUniform(), 1, 0, &mvp[0][0]);
        glUniform1i(m_Shader->GetTextureUniform(), 0);
        
        //Validate the shader, return if it fails
        if(m_Shader->Validate() == false)
        {
            return;
        }
        
        //Bind the texture
        graphics->BindTexture(GetTexture());
        
        //Enable blending if the texture has an alpha channel
        if(GetFormat() == GL_RGBA || m_Color.Alpha() != 1.0f)
        {
            graphics->EnableBlending();
        }
        
        //Draw the texture
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        //Unbind the vertex array
        graphics->BindVertexArray(0);
        
        //Disable blending, if we did in fact have it enabled
        if(GetFormat() == GL_RGBA || m_Color.Alpha() != 1.0f)
        {
            graphics->DisableBlending();
        }
    }
    
    unsigned int TextureFrame::GetFormat()
    {
        if(m_Texture != nullptr)
        {
            return m_Texture->GetFormat();
        }
        return 0;
    }
        
    string TextureFrame::GetFilename()
    {
        if(m_Texture != nullptr)
        {
            return m_Texture->GetFilename();
        }
        return "";
    }
    
    void TextureFrame::SetSourceFrame(SourceFrame aSourceFrame)
    {
        //Set the source frame
        m_SourceFrame.position = aSourceFrame.position;
        m_SourceFrame.size = aSourceFrame.size;
        
        //Update the VBO and Uv coordinates
        UpdateVertexBuffer(BufferObjectVertex | BufferObjectUvCoordinates);
    }
    
    SourceFrame TextureFrame::GetSourceFrame()
    {
        return m_SourceFrame;
    }
    
    void TextureFrame::SetShader(Shader* aShader)
    {
        //Safety check the shader
        if(aShader != nullptr && aShader != m_Shader)
        {
            //Get the new Shader from the ShaderManager
            m_Shader = aShader;
            
            //Update the vertex buffer
            UpdateVertexBuffer(BufferObjectAll);
        }
    }
    
    Shader* TextureFrame::GetShader()
    {
        return m_Shader;
    }
    
    void TextureFrame::SetColor(Color aColor)
    {
        if(aColor != m_Color)
        {
            //Set the color
            m_Color = aColor;
            
            //And update the vertex buffer
            UpdateVertexBuffer(BufferObjectColors);
        }
    }
    
    Color TextureFrame::GetColor()
    {
        return m_Color;
    }
    
    Texture* TextureFrame::GetTexture() const
    {
        return m_Texture;
    }
    
    void TextureFrame::SetTexture(Texture* aTexture, bool aOwnsTexture)
    {
        //Safety check the texture
        if(aTexture != nullptr)
        {
            //Unload the texture
            if(m_OwnsTexture == true && m_Texture != nullptr)
            {
                //Remove the Texture from the TextureManager and delete the texture object
                if(ServiceLocator::GetTextureManager()->RemoveTexture(this) == false)
                {
                    delete m_Texture;
                }
                m_Texture = nullptr;
            }
        
            //Set the new texture and the ownership
            m_Texture = aTexture;
            m_OwnsTexture = aOwnsTexture;
            
            //Set the source frame's size
            m_SourceFrame.size = m_Texture->GetSize();
            
            //Update the vertex buffer
            UpdateVertexBuffer(BufferObjectVertex | BufferObjectUvCoordinates);
        }
    }
    
    void TextureFrame::UpdateVertexBuffer(unsigned int aBufferObjectType)
    {
        //If the shader hasn't been set we can't update the vertex buffer, return
        if(m_Shader == nullptr)
        {
            return;
        }
    
        //If the VAO hasn't been generated yet, generate one
        if(m_VertexArrayObject == 0)
        {
            glGenVertexArrays(1, &m_VertexArrayObject);
        }
        
        //Bind the VAO
        ServiceLocator::GetGraphics()->BindVertexArray(m_VertexArrayObject);

        //Use the shader
        m_Shader->Use();

        //Do we need to update the vertex buffer?
        if((aBufferObjectType & BufferObjectVertex) == BufferObjectVertex)
        {
            //Has the VBO been generated, if not generate one
            if(m_VertexBufferObject == 0)
            {
                glGenBuffers(1, &m_VertexBufferObject);
            }
            
            //Bind the VBO
            glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);

            //Create an array for the vertices
            const int vertexSize = 2;
            const int vertexCount = 4;
            float* vertices = new float[vertexSize * vertexCount];
            
            //Initialize the vertices array
            vertices[0] = 0.0f;                     vertices[1] = 0.0f;
            vertices[2] = m_SourceFrame.size.x;     vertices[3] = 0.0f;
            vertices[4] = 0.0f;                     vertices[5] = m_SourceFrame.size.y;
            vertices[6] = m_SourceFrame.size.x;     vertices[7] = m_SourceFrame.size.y;
            
            //Set the vertex buffer data
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * vertexSize, vertices, GL_DYNAMIC_DRAW);
            
            //Enabled the vertices attribute for the shader
            int verticesIndex = m_Shader->GetAttribute("a_vertices");
            glEnableVertexAttribArray(verticesIndex);

            //Set the shader's attribute for the vertices
            glVertexAttribPointer(verticesIndex, vertexSize, GL_FLOAT, GL_FALSE, 0, 0);
            
            //Delete the vertices array
            SafeDeleteArray(vertices);
        }
        
        //Do we need to update the uv coordinates?
        if((aBufferObjectType & BufferObjectUvCoordinates) == BufferObjectUvCoordinates)
        {
            //Has the uv coordinates buffer been generated, if not generate one
            if(m_UvCoordinatesBufferObject == 0)
            {
                glGenBuffers(1, &m_UvCoordinatesBufferObject);
            }
            
            //Bind the uv coordinates buffer
            glBindBuffer(GL_ARRAY_BUFFER, m_UvCoordinatesBufferObject);
            
            //Build the UV Coordinates
            float x1 = (float)m_SourceFrame.position.x / (float)m_Texture->GetSize().x;
            float y1 = 1.0f - (((float)m_SourceFrame.position.y + m_SourceFrame.size.y) / (float)m_Texture->GetSize().y);
            float x2 = (float)(m_SourceFrame.position.x + m_SourceFrame.size.x) / (float)m_Texture->GetSize().x;
            float y2 = 1.0f - (m_SourceFrame.position.y / (float)m_Texture->GetSize().y);
            
            //Create the uv coordinates array
            const int uvSize = 2;
            const int uvCount = 4;
            float* uvCoordinates = new float[uvSize * uvCount];
            
            //Initialize the uv coordinates array
            uvCoordinates[0] = x1;  uvCoordinates[1] = y1;
            uvCoordinates[2] = x2;  uvCoordinates[3] = y1;
            uvCoordinates[4] = x1;  uvCoordinates[5] = y2;
            uvCoordinates[6] = x2;  uvCoordinates[7] = y2;

            //Set the uv coordinate data
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uvCount * uvSize, uvCoordinates, GL_DYNAMIC_DRAW);
        
            //Enable the texture coordinates attributes for the shader
            int uvIndex = m_Shader->GetAttribute("a_textureCoordinates");
            glEnableVertexAttribArray(uvIndex);
            
            //Set the shader's attribute for the uv coordinates
            glVertexAttribPointer(uvIndex, uvSize, GL_FLOAT, GL_FALSE, 0, 0);
            
            //Delete the uv coordinates array
            SafeDeleteArray(uvCoordinates);
        }
        
        //Do we need to update the colors?
        if((aBufferObjectType & BufferObjectColors) == BufferObjectColors)
        {
            //Has the colors buffer been generated, if not generate one
            if(m_ColorsBufferObject == 0)
            {
                glGenBuffers(1, &m_ColorsBufferObject);
            }
            
            //Bind the colors buffer
            glBindBuffer(GL_ARRAY_BUFFER, m_ColorsBufferObject);
            
            //Create the colors array
            const int colorSize = 4;
            const int colorCount = 4;
            float* colors = new float[colorSize * colorCount];
            
            //Initialize the colors array
            for(int i = 0; i < colorCount; i++)
            {
                int offset = i * colorSize;
                colors[offset] = m_Color.Red();
                colors[offset+1] = m_Color.Green();
                colors[offset+2] = m_Color.Blue();
                colors[offset+3] = m_Color.Alpha();
            }
            
            //Set the colors data
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colorCount * colorSize, colors, GL_DYNAMIC_DRAW);
        
            //Enable the colors attributes for the shader
            int colorIndex = m_Shader->GetAttribute("a_textureColor");
            glEnableVertexAttribArray(colorIndex);
            
            //Set the shader's attribute for the colors
            glVertexAttribPointer(colorIndex, colorSize, GL_FLOAT, GL_FALSE, 0, 0);
            
            //Delete the colors array
            SafeDeleteArray(colors);
        }
        
        //Unbind the VAO
        ServiceLocator::GetGraphics()->BindVertexArray(0);
    }
}