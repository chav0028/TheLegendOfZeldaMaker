//
//  ShaderManager.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-01-26.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "ShaderManager.h"
#include "../ServiceLocator.h"
#include "../../Platforms/PlatformLayer.h"
#include "../../IO/File.h"


namespace GameDev2D
{
    ShaderManager::ShaderManager() : GameService("ShaderManager")
    {
        //Load the pass through shader
        vector<string> attributes;
        attributes.push_back("a_vertices");
        attributes.push_back("a_sourceColor");
        attributes.push_back("a_pointSize");
        LoadShader("passThrough", "passThrough", attributes);
        
        //Load the texture pass through shader
        attributes.clear();
        attributes.push_back("a_vertices");
        attributes.push_back("a_textureCoordinates");
        attributes.push_back("a_textureColor");
        LoadShader("passThrough-tex", attributes);
    }

    ShaderManager::~ShaderManager()
    {
        //Unload all the shaders
        while(m_ShaderMap.size() > 0)
        {
            SafeDelete(m_ShaderMap.begin()->second);
            m_ShaderMap.erase(m_ShaderMap.begin());
        }
    }

    Shader* ShaderManager::LoadShader(const string& aShader, vector<string> aAttributes)
    {
        return LoadShader(aShader, aShader, aAttributes);
    }
    
    Shader* ShaderManager::LoadShader(const string& aVertexShader, const string& aFragmentShader, vector<string> aAttributes)
    {
        //Safety check the filenames
        if(aVertexShader.length() == 0 || aFragmentShader.length() == 0)
        {
            Error(false, "Failed to load the shader, either the Vertex shader or fragment shader length is 0");
            return nullptr;
        }
        
        //If the asserts are hit either the vertex shader or the fragment shader files have no length
        assert(aVertexShader.length() > 0);
        assert(aFragmentShader.length() > 0);
        
        //Check to see if they v and f are the same
        string key = aVertexShader == aFragmentShader ? aVertexShader : aVertexShader + aFragmentShader;

        //Get the pair from the texture map
        Shader* shader = m_ShaderMap[key];
        
        //Is the shader pointer null?
        if(shader == nullptr)
        {
        	//Determine if GLSL version 140 is supported by this context.
            //We'll use this info to generate a GLSL shader source string
            //with the proper version preprocessor string prepended
            float glLanguageVersion;
            
            if(ServiceLocator::GetPlatformLayer()->GetPlatformType() == PlatformType_iOS)
            {
                sscanf((char *)glGetString(GL_SHADING_LANGUAGE_VERSION), "OpenGL ES GLSL ES %f", &glLanguageVersion);
            }
            else
            {
                sscanf((char *)glGetString(GL_SHADING_LANGUAGE_VERSION), "%f", &glLanguageVersion);	
            }
            
            //GL_SHADING_LANGUAGE_VERSION returns the version standard version form
            //with decimals, but the GLSL version preprocessor directive simply
            //uses integers (thus 1.10 should 110 and 1.40 should be 140, etc.)
            //We multiply the floating point number by 100 to get a proper
            //number for the GLSL preprocessor directive
            GLuint version = (GLuint)(100 * glLanguageVersion);
        	
            //Get the size of the version preprocessor string info so we know
            //how much memory to allocate for our sourceString
            GLsizei versionStringSize = sizeof("#version 123\n");
            
            //
            if(ServiceLocator::GetPlatformLayer()->GetPlatformType() == PlatformType_iOS && version == 300)
            {
                versionStringSize = sizeof("#version 123 es\n");
            }

            //Was .vsh appended to the filename? If it was, remove it
            string vertexShader = string(aVertexShader);
            size_t found = vertexShader.find(".vsh");
            if(found != std::string::npos)
            {
                vertexShader.erase(found, 4);
            }
            
            //Get the path for the vertex shader file
            string vertexPath = ServiceLocator::GetPlatformLayer()->GetPathForResourceInDirectory(vertexShader.c_str(), "vsh", "Shaders");
            
            //Load the vertex shader
            File vertexShaderFile(vertexPath);
            
            //Safety check the vertex shader
            if(vertexShaderFile.GetBufferSize() == 0)
            {
                Error(false, "Failed to load the vertex shader");
                return nullptr;
            }
            
            //Allocate memory for the source string including the version preprocessor information
            GLchar* vertexSource = (GLchar*)malloc(vertexShaderFile.GetBufferSize() + versionStringSize);
	
            //Prepend our vertex shader source string with the supported GLSL version so
            //the shader will work on ES, Legacy, and OpenGL 3.2 Core Profile contexts
            if(ServiceLocator::GetPlatformLayer()->GetPlatformType() == PlatformType_iOS && version == 300)
            {
                sprintf(vertexSource, "#version %d es\n%s", version, vertexShaderFile.GetBuffer());
            }
            else
            {
                sprintf(vertexSource, "#version %d\n%s", version, vertexShaderFile.GetBuffer());
            }
            
            //Was .fsh appended to the filename? If it was, remove it
            string fragmentShader = string(aFragmentShader);
            found = fragmentShader.find(".fsh");
            if(found != std::string::npos)
            {
                fragmentShader.erase(found, 4);
            }
            
            //Get the path for the fragment shader file
            string fragmentPath = ServiceLocator::GetPlatformLayer()->GetPathForResourceInDirectory(fragmentShader.c_str(), "fsh", "Shaders");
            
            //Load the fragment shader
            File fragmentShaderFile(fragmentPath);
            
            //Safety check the fragment shader
            if(fragmentShaderFile.GetBufferSize() == 0)
            {
                Error(false, "Failed to load the fragment shader");
                return nullptr;
            }
            
            //Allocate memory for the source string including the version preprocessor information
            GLchar* fragmentSource = (GLchar*)malloc(fragmentShaderFile.GetBufferSize() + versionStringSize);
	
            //Prepend our fragment shader source string with the supported GLSL version so
            //the shader will work on ES, Legacy, and OpenGL 3.2 Core Profile contexts
            if(ServiceLocator::GetPlatformLayer()->GetPlatformType() == PlatformType_iOS && version == 300)
            {
                sprintf(fragmentSource, "#version %d es\n%s", version, fragmentShaderFile.GetBuffer());
            }
            else
            {
                sprintf(fragmentSource, "#version %d\n%s", version, fragmentShaderFile.GetBuffer());
            }
            
            //Create a new shader with the vertex and fragment shaders
            Shader* shader = new Shader(vertexSource, fragmentSource);
            shader->SetKey(key);
            
            //Cycle through the attributes and add them to the shader
            for(unsigned int i = 0; i < aAttributes.size(); i++)
            {
                shader->AddAttribute(aAttributes.at(i).c_str());
            }
            
            //Link the shader
            shader->Link();
            
            //Set the shader map pair for the filename key
            m_ShaderMap[key] = shader;
            
            //Free the memory for the vertex and fragment sources
            free(vertexSource);
            free(fragmentSource);
        }
        
        //Return the shader object
        return shader;
    }

    Shader* ShaderManager::GetShader(const string& aShader)
    {
        //Get the shader for the key
        Shader* shader = m_ShaderMap[aShader];

        //Return the shader object
        return shader;
    }
    
    Shader* ShaderManager::GetShader(const string& aVertexShader, const string& aFragmentShader)
    {
        if(aVertexShader == aFragmentShader)
        {
            GetShader(aVertexShader);
        }
        return GetShader(string(aVertexShader + aFragmentShader));
    }
    
    Shader* ShaderManager::GetPassthroughShader()
    {
        return GetShader("passThrough");
    }
    
    Shader* ShaderManager::GetPassthroughTextureShader()
    {
        return GetShader("passThrough-tex");
    }

    bool ShaderManager::CanUpdate()
    {
        return false;
    }
    
    bool ShaderManager::CanDraw()
    {
        return false;
    }
}
