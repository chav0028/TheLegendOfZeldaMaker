//
//  ServiceLocator.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-10.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//


#include "ServiceLocator.h"
#include "../Platforms/PlatformLayer.h"


namespace GameDev2D
{
    
    //Initialize the static variables to NULL
    PlatformLayer* ServiceLocator::s_PlatformLayer = nullptr;
    Graphics* ServiceLocator::s_Graphics = nullptr;
    TextureManager* ServiceLocator::s_TextureManager = nullptr;
    ShaderManager* ServiceLocator::s_ShaderManager = nullptr;
    InputManager* ServiceLocator::s_InputManager = nullptr;
    AudioManager* ServiceLocator::s_AudioManager = nullptr;
    SceneManager* ServiceLocator::s_SceneManager = nullptr;
    FontManager* ServiceLocator::s_FontManager = nullptr;
    LoadingUI* ServiceLocator::s_LoadingUI = nullptr;
    DebugUI* ServiceLocator::s_DebugUI = nullptr;
    
    
    void ServiceLocator::SetPlatformLayer(PlatformLayer* aPlatformLayer)
    {
        s_PlatformLayer = aPlatformLayer;
    }
    
    void ServiceLocator::LoadDefaultServices()
    {
        AddService(new ShaderManager());
        AddService(new TextureManager());
        AddService(new Graphics());
        AddService(new InputManager());
        AddService(new AudioManager());
        AddService(new FontManager());
        AddService(new SceneManager());
        AddService(new LoadingUI());
        AddService(new DebugUI());
    }
    
    void ServiceLocator::AddService(Graphics* aGraphics, bool aResponsibleForDeletion)
    {
        AddService((GameService**)&s_Graphics, aGraphics, aResponsibleForDeletion);
    }
    
    void ServiceLocator::AddService(TextureManager* aTextureManager, bool aResponsibleForDeletion)
    {
        AddService((GameService**)&s_TextureManager, aTextureManager, aResponsibleForDeletion);
    }
    
    void ServiceLocator::AddService(ShaderManager* aShaderManager, bool aResponsibleForDeletion)
    {
        AddService((GameService**)&s_ShaderManager, aShaderManager, aResponsibleForDeletion);
    }
    
    void ServiceLocator::AddService(InputManager* aInputManager, bool aResponsibleForDeletion)
    {
        AddService((GameService**)&s_InputManager, aInputManager, aResponsibleForDeletion);
    }
    
    void ServiceLocator::AddService(AudioManager* aAudioManager, bool aResponsibleForDeletion)
    {
        AddService((GameService**)&s_AudioManager, aAudioManager, aResponsibleForDeletion);
    }
    
    void ServiceLocator::AddService(SceneManager* aSceneManager, bool aResponsibleForDeletion)
    {
        AddService((GameService**)&s_SceneManager, aSceneManager, aResponsibleForDeletion);
    }
    
    void ServiceLocator::AddService(FontManager* aFontManager, bool aResponsibleForDeletion)
    {
        AddService((GameService**)&s_FontManager, aFontManager, aResponsibleForDeletion);
    }
    
    void ServiceLocator::AddService(LoadingUI* aLoadingUI, bool aResponsibleForDeletion)
    {
        AddService((GameService**)&s_LoadingUI, aLoadingUI, aResponsibleForDeletion);
    }
    
    void ServiceLocator::AddService(DebugUI* aDebugUI, bool aResponsibleForDeletion)
    {
        AddService((GameService**)&s_DebugUI, aDebugUI, aResponsibleForDeletion);
    }
    
    PlatformLayer* ServiceLocator::GetPlatformLayer()
    {
        return s_PlatformLayer;
    }
    
    Graphics* ServiceLocator::GetGraphics()
    {
        assert(s_Graphics != nullptr);
        return s_Graphics;
    }
    
    TextureManager* ServiceLocator::GetTextureManager()
    {
        assert(s_TextureManager != nullptr);
        return s_TextureManager;
    }
    
    ShaderManager* ServiceLocator::GetShaderManager()
    {
        assert(s_SceneManager != nullptr);
        return s_ShaderManager;
    }
    
    InputManager* ServiceLocator::GetInputManager()
    {
        assert(s_InputManager != nullptr);
        return s_InputManager;
    }
    
    AudioManager* ServiceLocator::GetAudioManager()
    {
        assert(s_AudioManager != nullptr);
        return s_AudioManager;
    }
    
    SceneManager* ServiceLocator::GetSceneManager()
    {
        assert(s_SceneManager != nullptr);
        return s_SceneManager;
    }
    
    FontManager* ServiceLocator::GetFontManager()
    {
        assert(s_FontManager != nullptr);
        return s_FontManager;
    }
    
    LoadingUI* ServiceLocator::GetLoadingUI()
    {
        assert(s_LoadingUI != nullptr);
        return s_LoadingUI;
    }
    
    DebugUI* ServiceLocator::GetDebugUI()
    {
        assert(s_DebugUI != nullptr);
        return s_DebugUI;
    }
    
    void ServiceLocator::RemoveService(GameService* aService)
    {
        if(aService != nullptr)
        {
            if(aService->IsServiceLocatorResponsibleForDeletion() == true)
            {
                SafeDelete(aService);
            }
            else
            {
                aService = nullptr;
            }
        }
    }
    
    void ServiceLocator::RemoveAllServices()
    {
        RemoveService(s_DebugUI);
        RemoveService(s_LoadingUI);
        RemoveService(s_SceneManager);
        RemoveService(s_FontManager);
        RemoveService(s_AudioManager);
        RemoveService(s_InputManager);
        RemoveService(s_Graphics);
        RemoveService(s_TextureManager);
        RemoveService(s_ShaderManager);
    }
    
    void ServiceLocator::UpdateServices(double aDelta)
    {
        UpdateService(s_Graphics, aDelta);
        UpdateService(s_TextureManager, aDelta);
        UpdateService(s_ShaderManager, aDelta);
        UpdateService(s_InputManager, aDelta);
        UpdateService(s_AudioManager, aDelta);
        UpdateService(s_SceneManager, aDelta);
        UpdateService(s_FontManager, aDelta);
        UpdateService(s_LoadingUI, aDelta);
        UpdateService(s_DebugUI, aDelta);
    }
    
    void ServiceLocator::UpdateService(GameService* aService, double aDelta)
    {
        if(aService != nullptr && aService->CanUpdate() == true)
        {
            aService->Update(aDelta);
        }
    }
    
    void ServiceLocator::DrawServices()
    {
        DrawService(s_Graphics);
        DrawService(s_TextureManager);
        DrawService(s_ShaderManager);
        DrawService(s_InputManager);
        DrawService(s_AudioManager);
        DrawService(s_SceneManager);
        DrawService(s_FontManager);
        DrawService(s_LoadingUI);
        DrawService(s_DebugUI);
    }
    
    void ServiceLocator::DrawService(GameService* aService)
    {
        if(aService != nullptr && aService->CanDraw() == true)
        {
            aService->Draw();
        }
    }
    
    void ServiceLocator::AddService(GameService** aServiceToSet, GameService* aServiceRef, bool aResponsibleForDeletion)
    {
        //Remove the previous service, if there is one
        RemoveService(*aServiceToSet);
        
        //Set the service reference and the deletion flag
        *aServiceToSet = aServiceRef;
        (*aServiceToSet)->SetServiceLocatorResponsibleForDeletion(aResponsibleForDeletion);
    }
}
