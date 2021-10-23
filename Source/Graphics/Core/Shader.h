//
//  Shader.h
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-17.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Shader__
#define __GameDev2D__Shader__

#include "../../Core/BaseObject.h"
#include "../OpenGL.h"


using namespace std;


namespace GameDev2D
{
    //The Shader class is a primitive class for basic shader support. It supports the included passthrough
    //shaders. It is NOT entirely complete and may need more work for more advanced shaders.
    class Shader : public BaseObject
    {
    public:
        //Returns an attribute index for a attribute key
        int GetAttribute(const char* attribute);
        
        //Returns an attribute index for a uniform key
        unsigned int GetUniform(const char* uniform);
        
        //Returns the uniform for the model view projection matrix (used in the passthrough shader)
        unsigned int GetModelViewProjectionUniform();
        
        //Returns the uniform for the texture (used in the passthrough shader)
        unsigned int GetTextureUniform();
        
        //Call to use the shader program
        void Use();
        
        //Validate the Shader, returns true is the validation was successful
        bool Validate();
        
        //Set a key to identify this shader from other shader
        void SetKey(const string& key);
        
        //Returns the key for the shader
        string GetKey();
        
    protected:
        //Since the Shader is created
        Shader(const char* vertexShader, const char* fragmentShader);
        ~Shader();
        
        //Compiles a shader
        bool Compile(GLenum target, GLuint* shader, const GLchar** sources);
        
        //Adds an attribute
        void AddAttribute(const char* attribute);
        
        //Link the Shader program
        void Link();
        
        //Shader log methods
        string GetShaderLog(GLuint *shader);
        string GetProgramLog();
        
        //The ShaderManager needs to access the protected methods
        friend class ShaderManager;
        
    private:
        //Member variables
        GLuint m_Program;
        GLuint m_VertexShader;
        GLuint m_FragmentShader;
        map<string, int> m_AttributeLocations;
        unsigned int m_ModelViewProjectionUniform;
        unsigned int m_TextureUniform;
        string m_VertexShaderString;
        string m_FragmentShaderString;
        string m_Key;
    };
}

#endif /* defined(__GameDev2D__Shader__) */
