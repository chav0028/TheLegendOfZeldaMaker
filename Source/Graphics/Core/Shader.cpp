//
//  Shader.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-17.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//


#include "Shader.h"
#include "../../IO/File.h"


namespace GameDev2D
{    
    Shader::Shader(const char* aVertexShader, const char* aFragmentShader) : BaseObject("Shader"),
        m_Program(0),
        m_VertexShader(0),
        m_FragmentShader(0),
        m_ModelViewProjectionUniform(0),
        m_TextureUniform(0),
        m_VertexShaderString(aVertexShader),
        m_FragmentShaderString(aFragmentShader),
        m_Key("")
    {
        //Create the shader program
        m_Program = glCreateProgram();
    
        //If this assert is hit, that means the vertex shader that was passed in has no content
        assert(m_VertexShaderString.length() > 0);

        //Log the vertex shader
        Log(VerbosityLevel_Shaders, "Vertex shader\n%s", m_VertexShaderString.c_str());
        
        //Compile the vertex shader, if successful, attach it to the program
        if(Compile(GL_VERTEX_SHADER, &m_VertexShader, &aVertexShader) == false)
        {
            Error("Failed to compile %s.vsh", aVertexShader);
        }
        else
        {
            glAttachShader(m_Program, m_VertexShader);
        }

        //If this assert is hit, that means the fragment shader that was passed in has no content
        assert(m_FragmentShaderString.length() > 0);

        //Log the fragment shader
        Log(VerbosityLevel_Shaders, "Fragment shader\n%s", m_FragmentShaderString.c_str());

        //Compile the fragment shader, if successful, attach it to the program
        if(Compile(GL_FRAGMENT_SHADER, &m_FragmentShader, &aFragmentShader) == false)
        {
            Error("Failed to compile %s.fsh", aFragmentShader);
        }
        else
        {
            glAttachShader(m_Program, m_FragmentShader);
        }
    }
    
    Shader::~Shader()
    {
        //Delete the shader program
        if(m_Program != 0)
        {
            glDeleteProgram(m_Program);
            m_Program = 0;
        }
    }
    
    void Shader::AddAttribute(const char* aAttribute)
    {
        int location = (int)m_AttributeLocations.size();
        glBindAttribLocation(m_Program, location, aAttribute);
        m_AttributeLocations[string(aAttribute)] = location;
    }
    
    int Shader::GetAttribute(const char* aAttribute)
    {
        int index = m_AttributeLocations[string(aAttribute)];
        return index;
    }
    
    unsigned int Shader::GetUniform(const char* aUniform)
    {
        return 0; //TODO:
    }
    
    unsigned int Shader::GetModelViewProjectionUniform()
    {
        return m_ModelViewProjectionUniform;
    }
    
    unsigned int Shader::GetTextureUniform()
    {
        return m_TextureUniform;
    }
    
    void Shader::Link()
    {
        //Link the program
        glLinkProgram(m_Program);
        
        //Check the program log, and log any info
        string programLog = GetProgramLog();
        if(programLog.length() > 0)
        {
            if(string::npos != programLog.find("error"))
            {
                Error(false, programLog.c_str());
            }
            else
            {
                Log(VerbosityLevel_Shaders, programLog.c_str());
            }
        }
        
        //Get the link status of the program
        GLint status;
        glGetProgramiv(m_Program, GL_LINK_STATUS, &status);
        
        //Log if there is an error
        if(status == GL_FALSE)
        {
            Error(false, "Failed to link shader program %i", m_Program);
        }
        
        //Delete the vertex shader
        if(m_VertexShader != 0)
        {
            glDeleteShader(m_VertexShader);
            m_VertexShader = 0;
        }
        
        //Delete the fragment shader
        if(m_FragmentShader != 0)
        {
            glDeleteShader(m_FragmentShader);
            m_FragmentShader = 0;
        }
        
        //Get the uniform location of the modelview matrix
        m_ModelViewProjectionUniform = glGetUniformLocation(m_Program, "u_modelViewProjection");
        
        //Get the uniform location of the texture
        m_TextureUniform = glGetUniformLocation(m_Program, "u_texture");
    }
    
    void Shader::Use()
    {
        if(m_Program != 0)
        {
            glUseProgram(m_Program);
        }
    }
    
    bool Shader::Validate()
    {
        //Validate the shader program
        if(m_Program != 0)
        {
            glValidateProgram(m_Program);
        }
        
        //Check the program log, and log any info
        string programLog = GetProgramLog();
        if(programLog.length() > 0)
        {
            if(string::npos != programLog.find("error"))
            {
                Error(false, programLog.c_str());
            }
            else
            {
                Log(VerbosityLevel_Shaders, programLog.c_str());
            }
        }
        
        //Check the status of the program
        int status;
        glGetProgramiv(m_Program, GL_VALIDATE_STATUS, &status);
        if (status == GL_FALSE)
        {
            Error(false, "Failed to validate shader program %d", m_Program);
        }
        
        //Return the status
        return status != GL_FALSE;
    }
    
    void Shader::SetKey(const string& aKey)
    {
        m_Key = string(aKey);
    }
    
    string Shader::GetKey()
    {
        return m_Key;
    }
    
    bool Shader::Compile(GLenum aTarget, GLuint* aShader, const GLchar** aSources)
    {
        //Compile the shader
        *aShader = glCreateShader(aTarget);
        glShaderSource(*aShader, 1, aSources, NULL);
        glCompileShader(*aShader);
        
        //Check the shader log, and log any info
        string shaderLog = GetShaderLog(aShader);
        if(shaderLog.length() > 0)
        {
            if(string::npos != shaderLog.find("error"))
            {
                Error(false, shaderLog.c_str());
            }
            else
            {
                Log(VerbosityLevel_Shaders, shaderLog.c_str());
            }
        }
        
        //Return the compile status of the shader
        GLint status;
        glGetShaderiv(*aShader, GL_COMPILE_STATUS, &status);
        
        //If this assert was hit, then the shader failed to compile
        assert(status != GL_FALSE);
        
        //
        return status != GL_FALSE;
    }
    
    string Shader::GetShaderLog(GLuint *aShader)
    {
        //Local variable, used below
        string shaderLog = "";
        
        //Safety check the shader
        if(*aShader != 0)
        {
            //Get the length of the log
            GLint bufferSize = 0;
            glGetShaderiv(*aShader, GL_INFO_LOG_LENGTH, &bufferSize);
            if(bufferSize > 0)
            {
                //Allocate a buffer for our shader log
                char* buffer = (char*)malloc(bufferSize);
                GLint logLength = 0;
                
                //Get the shader log
                glGetShaderInfoLog(*aShader, bufferSize, &logLength, buffer);
                if(logLength > 0)
                {
                    shaderLog = string(buffer);
                }
                
                //Free the buffer
                free(buffer);
            }
        }
        
        //Return the shader log
        return shaderLog;
    }
    
    string Shader::GetProgramLog()
    {
        //Local variable, used below
        string programLog = "";
        
        //Safety check the program log
        if(m_Program != 0)
        {
            //Get the length of the log
            GLint bufferSize = 0;
            glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &bufferSize);
            if(bufferSize > 0)
            {
                //Allocate a buffer for the program log
                char* buffer = (char*)malloc(bufferSize);
                GLint logLength = 0;
                
                //Get the program log
                glGetProgramInfoLog(m_Program, bufferSize, &logLength, buffer);
                if(logLength > 0)
                {
                    programLog = string(buffer);
                }
                
                //Free the buffer
                free(buffer);
            }
        }
        
        //Return the program log
        return programLog;
    }
}
