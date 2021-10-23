//
//  ShaderManager.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-01-26.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__ShaderManager__
#define __GameDev2D__ShaderManager__

#include "../GameService.h"
#include "../../Graphics/Core/Shader.h"


using namespace std;

namespace GameDev2D
{
    //The ShaderManager uses reference counting to ensure that the same shader isn't loaded twice.
    //It has two default passthrough shader, one for basic geometry the other for textures.
    class ShaderManager : public GameService
    {
    public:
        ShaderManager();
        ~ShaderManager();
        
        //Loads a shader, where the vertex and fragment shaders have the same name, also takes in a vector of attributes
        Shader* LoadShader(const string& shader, vector<string> attributes);
        
        //Loads a shader, where the vertex and fragment shaders have different names, also takes in a vector of attributes
        Shader* LoadShader(const string& vertexShader, const string& fragmentShader, vector<string> attributes);
        
        //Returns an already loaded shader
        Shader* GetShader(const string& shader);
        Shader* GetShader(const string& vertexShader, const string& fragmentShader);
        
        //Default passthrough shader
        Shader* GetPassthroughShader();
        Shader* GetPassthroughTextureShader();
        
        //Tells the ServiceLocator wether to Update and Draw this Game Service
        bool CanUpdate();
        bool CanDraw();
        
    private:
        //Member variables
        map<string, Shader*> m_ShaderMap;
    };
}

#endif /* defined(__GameDev2D__ShaderManager__) */
