//
//  ServiceLocator.h
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-10.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__ServiceLocator__
#define __GameDev2D__ServiceLocator__

#include "FrameworkConfig.h"
#include "GameService.h"
#include "Graphics/Graphics.h"
#include "TextureManager/TextureManager.h"
#include "ShaderManager/ShaderManager.h"
#include "InputManager/InputManager.h"
#include "SceneManager/SceneManager.h"
#include "FontManager/FontManager.h"
#include "AudioManager/AudioManager.h"
#include "DebugUI/DebugUI.h"
#include "LoadingUI/LoadingUI.h"


namespace GameDev2D
{
    //Forward declarations
    class PlatformLayer;

    //
    class ServiceLocator
    {
    public:
        //Used to set the PlatformLayer
        static void SetPlatformLayer(PlatformLayer* platformLayer);
        
        //Loads the default services used by the framework, but anyone can substitute one of the services
        //for a modified service, perhaps one that has advanced logging or debug checks.
        static void LoadDefaultServices();
        
        //Methods that are called from the PlatformLayer to Update and Draw all the services
        static void UpdateServices(double delta);
        static void DrawServices();

        //Setter methods to add each GameService to the ServiceLocator
        static void AddService(Graphics* graphics, bool responsibleForDeletion = true);
        static void AddService(TextureManager* textureManager, bool responsibleForDeletion = true);
        static void AddService(ShaderManager* shaderManager, bool responsibleForDeletion = true);
        static void AddService(InputManager* inputManager, bool responsibleForDeletion = true);
        static void AddService(AudioManager* audioManager, bool responsibleForDeletion = true);
        static void AddService(SceneManager* sceneManager, bool responsibleForDeletion = true);
        static void AddService(FontManager* fontManager, bool responsibleForDeletion = true);
        static void AddService(LoadingUI* loadingUI, bool responsibleForDeletion = true);
        static void AddService(DebugUI* debugUI, bool responsibleForDeletion = true);
        
        //Getter methods to access the ServiceLocator's GameServices
        static PlatformLayer* GetPlatformLayer();
        static Graphics* GetGraphics();
        static TextureManager* GetTextureManager();
        static ShaderManager* GetShaderManager();
        static InputManager* GetInputManager();
        static AudioManager* GetAudioManager();
        static SceneManager* GetSceneManager();
        static FontManager* GetFontManager();
        static LoadingUI* GetLoadingUI();
        static DebugUI* GetDebugUI();
        
        //Removes a specific service from the ServiceLocator
        static void RemoveService(GameService* service);
        
        //Removes all services from the ServiceLocator
        static void RemoveAllServices();
        
    private:
        //Conveniance methods to Update and Draw an individual GameService
        static void UpdateService(GameService* service, double aDelta);
        static void DrawService(GameService* aService);

        //Conveniance method to add an individual GameService
        static void AddService(GameService** serviceToSet, GameService* serviceRef, bool responsibleForDeletion);
        
        //Static variables for each GameService
        static PlatformLayer* s_PlatformLayer;
        static Graphics* s_Graphics;
        static TextureManager* s_TextureManager;
        static ShaderManager* s_ShaderManager;
        static InputManager* s_InputManager;
        static AudioManager* s_AudioManager;
        static SceneManager* s_SceneManager;
        static FontManager* s_FontManager;
        static LoadingUI* s_LoadingUI;
        static DebugUI* s_DebugUI;
    };
}
#endif /* defined(__GameDev2D__ServiceLocator__) */
