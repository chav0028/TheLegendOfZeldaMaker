//
//  SceneManager.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "SceneManager.h"
#include "../ServiceLocator.h"
#include "../LoadingUI/LoadingUI.h"
#include "../Graphics/Graphics.h"
#include "../../Core/Scene.h"
#include "../../Platforms/PlatformLayer.h"
#include "../../Graphics/Core/Camera.h"
#include "../../Events/Platform/ResizeEvent.h"


namespace GameDev2D
{
    SceneManager::SceneManager() : GameService("SceneManager"),
        m_RootScene(nullptr),
        m_TransitionTimer(nullptr),
        m_TransitionState(TransitionNone),
        m_PopCount(0)
    {
        //Create the timer and register for the did finish event
        m_TransitionTimer = new Timer();
        m_TransitionTimer->AddEventListener(this, TIMER_DID_FINISH_EVENT);
        
        //Register for controller events
        ServiceLocator::GetInputManager()->AddEventListener(this, CONTROLLER_PLUGGED_IN);
        ServiceLocator::GetInputManager()->AddEventListener(this, CONTROLLER_UNPLUGGED);
        
        //Register for resize events
        ServiceLocator::GetPlatformLayer()->AddEventListener(this, RESIZE_EVENT);
    }
    
    SceneManager::~SceneManager()
    {
        //Remove all the listeners
        ServiceLocator::GetInputManager()->RemoveAllHandlersForListener(this);
        ServiceLocator::GetPlatformLayer()->RemoveAllHandlersForListener(this);
    
        //Set the active and root scene to null
        m_RootScene = nullptr;
        
        //Clear the active scenes vector
        m_ActiveScenes.clear();
        
        //Cycle through the scenes and delete the Scene if the SceneManager is responsible for its deletion
        while(m_Scenes.empty() == false)
        {
            //Get the last Scene object on the m_Scenes vector
            vector<Scene*> scenes = m_Scenes.back();
            
            //Cycle through the scenes and delete the Scene if the SceneManager is responsible for its deletion
            while(scenes.empty() == false)
            {
                //Get the last Scene object on the scenes vector
                Scene* scene = scenes.back();
            
                //Remove any input listeners for this Scene object
                ServiceLocator::GetInputManager()->RemoveAllHandlersForListener(scene);

                //Is the SceneManager responsible for deleting this Scene object
                if(scene->IsSceneManagerResponsibleForDeletion() == true)
                {
                    SafeDelete(scene);
                }
                
                //Remove the last Scene object from the scenes vector
                scenes.pop_back();
            }
            
            //Remove the last Scene object from the m_Scenes vector
            m_Scenes.pop_back();
        }
    }
    
    void SceneManager::Update(double aDelta)
    {
        //Update the transition timer
        m_TransitionTimer->Update(aDelta);
    
        //Is the scene loaded?
        if(IsActiveSceneLoaded() == true)
        {
            //If its loaded, update the active scene
            for(unsigned int i = 0; i < m_ActiveScenes.size(); i++)
            {
                UpdateScene(m_ActiveScenes.at(i), aDelta);
            }
        }
        else
        {
            //Otherwise load the active scene
            LoadActiveScene();
        }
    }
    
    void SceneManager::UpdateScene(Scene* aScene, double aDelta)
    {
        //Safety check that the scene isn't null, and update the scene if it isn't
        if(aScene != nullptr)
        {
            aScene->Update(aDelta);
        }
    }
    
    void SceneManager::Draw()
    {
        for(unsigned int i = 0; i < m_ActiveScenes.size(); i++)
        {
            DrawScene(m_ActiveScenes.at(i));
        }
    }
    
    void SceneManager::DrawScene(Scene* aScene)
    {
        //Safety check the active scene pointer
        if(aScene != nullptr)
        {
            //Cache the active camera and set the scene's camera as the active camera
            Camera* activeCamera = ServiceLocator::GetGraphics()->GetActiveCamera();
            ServiceLocator::GetGraphics()->SetActiveCamera(aScene->GetCamera());
            
            //Draw the scene
            aScene->Draw();
            
            //Set the active camera back
            ServiceLocator::GetGraphics()->SetActiveCamera(activeCamera);
        }
    }
    
    vector<Scene*> SceneManager::GetActiveScenes()
    {
        return m_ActiveScenes;
    }
    
    Scene* SceneManager::GetScene(const string& aName)
    {
        //Cycle through the Scene's and compare against the name
        for(unsigned int i = 0; i < m_Scenes.size(); i++)
        {
            for(unsigned int j = 0; j < m_Scenes.at(i).size(); j++)
            {
                if(m_Scenes.at(i).at(j)->GetType() == aName)
                {
                    return m_Scenes.at(i).at(j);
                }
            }
        }
    
        return nullptr;
    }
    
    void SceneManager::LoadActiveScene()
    {
        bool isLoaded = true;
        float totalPercentage = 0.0f;
    
        //Cycle through the active scene's and load their content
        for(unsigned int i = 0; i < m_ActiveScenes.size(); i++)
        {
            //Begin profiling how long the load step takes
            BeginProfile();
        
            //Get the percentage of loaded content for the active scene
            float scenePercentage = m_ActiveScenes.at(i)->IsLoaded() == true ? 1.0f : m_ActiveScenes.at(i)->LoadContent();
            totalPercentage += scenePercentage;

            //End profiling how long the load step look
            double duration = EndProfile();
            Log("Loading %s - %f", m_ActiveScenes.at(i)->GetType().c_str(), duration);

            //If the scene percentage is 100% or greater(?),
            if(scenePercentage >= 1.0f)
            {
                //Set that the active scene is loaded
                m_ActiveScenes.at(i)->SetIsLoaded(true);
            }
            else
            {
                isLoaded = false;
            }
        }
        
        //Set the loading bar percentage
        ServiceLocator::GetLoadingUI()->SetLoadingBarPercentage(totalPercentage / (float)m_ActiveScenes.size());
        
        //Are we done loading the active scene's
        if(isLoaded == true)
        {
            //Cycle through the active scene's and tell the scene that it will now transition on screen
            for(unsigned int i = 0; i < m_ActiveScenes.size(); i++)
            {
                m_ActiveScenes.at(i)->WillTransitionOn();
            }
            
            //And Hide the Loading UI
            ServiceLocator::GetLoadingUI()->Hide();
        }
    }
    
    bool SceneManager::IsActiveSceneLoaded()
    {
        //Cycle through the active scenes and make sure all the active scene's are loaded
        bool isLoaded = true;
        for(unsigned int i = 0; i < m_ActiveScenes.size(); i++)
        {
            if(m_ActiveScenes.at(i)->IsLoaded() == false)
            {
                isLoaded = false;
                break;
            }
        }

        return isLoaded;
    }
    
    void SceneManager::PushScene(Scene* aScene, bool aResponsibleForDeletion)
    {
        vector<Scene*> scenes;
        scenes.push_back(aScene);
        
        vector<bool> responsibleForDeletion;
        responsibleForDeletion.push_back(aResponsibleForDeletion);
        
        PushScenes(scenes, responsibleForDeletion);
    }
    
    void SceneManager::PushScenes(vector<Scene*>& aScenes, vector<bool>& aResponsibleForDeletion)
    {
        //If this assert is hit, it means you are trying to push a scene during another scene's transition
        assert(m_TransitionTimer->IsRunning() == false);
    
        //If this assert is hit, it means that the vector parameters above do NOT have the same size
        assert(aScenes.size() == aResponsibleForDeletion.size());
    
        //Used to add the new scenes
        vector<Scene*> scenes;
    
        //Cycle through the scenes that we want to push and add them
        for(unsigned int i = 0; i < aScenes.size(); i++)
        {
            //Keep track if the SceneManager is responsible for deleting this scene
            aScenes.at(i)->SetSceneManagerResponsibleForDeletion(aResponsibleForDeletion.at(i));
    
            //Add the Scene object to the scenes vector
            scenes.push_back(aScenes.at(i));
        }
        
        //Push the scenes vector onto the m_Scenes vector, confusing right?
        m_Scenes.push_back(scenes);
    
        //If there isn't an active scene in the scene stack set the active scene
        //immediately otherwise we have to handle a scene transition
        if(m_ActiveScenes.size() == 0)
        {
            SetActiveScene();
        }
        else
        {
            double duration = 0.0;
            
            //Cycle through the active scenes and determine the biggest transition duration
            for(unsigned int i = 0; i < m_ActiveScenes.size(); i++)
            {
                duration = fmax(duration, m_ActiveScenes.at(i)->WillTransitionOff());
            }
            
            //Set the transition state
            m_TransitionState = TransitionPush;
            m_TransitionTimer->SetDuration(duration);
            m_TransitionTimer->Reset(true);
        }
    }
    
    void SceneManager::Pop(unsigned int aCount)
    {
        //If this assert is hit, it means you are trying to push a scene during another scene's transition
        assert(m_TransitionTimer->IsRunning() == false);
    
        //Cycle through the active scenes and determine the biggest transition duration
        double duration = 0.0;
        for(unsigned int i = 0; i < m_ActiveScenes.size(); i++)
        {
            duration = fmax(duration, m_ActiveScenes.at(i)->WillTransitionOff());
        }
        
        //Set the pop count
        m_PopCount = aCount;
        
        //Set the transition state
        m_TransitionState = TransitionPop;
        m_TransitionTimer->SetDuration(duration);
        m_TransitionTimer->Reset(true);
    }
    
    void SceneManager::AddScene(Scene* aScene, bool aResponsibleForDeletion)
    {
        //If this assert is hit, it means you are trying to push a scene during another scene's transition
        assert(m_TransitionTimer->IsRunning() == false);
    
        //Keep track if the SceneManager is responsible for deleting this scene
        aScene->SetSceneManagerResponsibleForDeletion(aResponsibleForDeletion);
        
        //If this assert is hit, it means you are trying to add a Scene when no other scenes exist, try pushing a scene instead
        assert(m_Scenes.size() > 0 && m_Scenes.back().size() > 0);
        
        //Add the scene to the back of the current scenes vector (usually the active scenes)
        m_Scenes.back().push_back(aScene);
        
        //Update the active scenes
        SetActiveScene();
    }
    
    void SceneManager::RemoveScene(Scene* aScene)
    {
        //Safety check the scene parameter
        if(aScene != nullptr)
        {
            //Flag to break out of the first for loop
            bool found = false;
        
            //Cycle through and find the Scene object to remove
            for(unsigned int i = 0; i < m_Scenes.size(); i++)
            {
                for(unsigned int j = 0; j < m_Scenes.at(i).size(); j++)
                {
                    //Did we find the scene?
                    if(m_Scenes.at(i).at(j) == aScene)
                    {
                        //Is the SceneManager responsible for deleting this Scene object
                        if(m_Scenes.at(i).at(j)->IsSceneManagerResponsibleForDeletion() == true)
                        {
                            SafeDelete(m_Scenes.at(i).at(j));
                        }
                        
                        //Erase the Scene pointer from the m_Scenes vector
                        m_Scenes.at(i).erase(m_Scenes.at(i).begin() + j);
                        
                        found = true;
                        break;
                    }
                }
                
                //Break out of the outer for loop if the flag is true
                if(found == true)
                {
                    break;
                }
            }
            
            //Update the active scenes
            if(found == true)
            {
                SetActiveScene();
            }
        }
    }
    
    void SceneManager::RemoveScene(const string& aName)
    {
        RemoveScene(GetScene(aName));
    }
    
    void SceneManager::PreloadScene(Scene* aScene)
    {
        //Safety check the Scene pointer
        if(aScene != nullptr)
        {
            bool loaded = false;
            
            //Load the scene until it is loaded
            while (loaded == false)
            {
                loaded = aScene->LoadContent() == 1.0f;
            }
            
            //Set that the scene has been loaded
            aScene->SetIsLoaded(true);
        }
    }
    
    bool SceneManager::IsActiveScene(Scene* aScene)
    {
        if(aScene != nullptr)
        {
            for(unsigned int i = 0; i < m_ActiveScenes.size(); i++)
            {
                if(m_ActiveScenes.at(i) == aScene)
                {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    void SceneManager::SetRootScene(Scene* aScene)
    {
        //Set the root scene
        m_RootScene = aScene;
        
        //Push the root scene onto the scene stack
        PushScene(m_RootScene, true);
    }
    
    void SceneManager::SetActiveScene()
    {
        //Remove all the input handlers for the active scene
        for(unsigned int i = 0; i < m_ActiveScenes.size(); i++)
        {
            ServiceLocator::GetInputManager()->RemoveAllHandlersForListener(m_ActiveScenes.at(i));
        }

        //Handle the transition type
        if(m_TransitionState == TransitionPop)
        {
            //If this assert is hit it means you are trying to remove the LAST of the active scenes
            assert(m_ActiveScenes == m_Scenes.back());
            
            //While there are scenes to pop, keep looping
            while (m_PopCount > 0)
            {
                if(find(m_Scenes.back().begin(), m_Scenes.back().end(), m_RootScene) != m_Scenes.back().end())
                {
                    Error(false, "Trying to Pop the Root scene");
                    break;
                }
                else
                {
                    //Get the last scenes vector
                    vector<Scene*> scenes = m_Scenes.back();

                    //Cycle through the scenes and delete the Scene if the SceneManager is responsible for its deletion
                    while(scenes.empty() == false)
                    {
                        //Get the last Scene object on the scenes vector
                        Scene* scene = scenes.back();

                        //Is the SceneManager responsible for deleting this Scene object
                        if(scene->IsSceneManagerResponsibleForDeletion() == true)
                        {
                            SafeDelete(scene);
                        }
                        
                        //Remove the last Scene object from the scenes vector
                        scenes.pop_back();
                    }
                    
                    //Pop the Scene object from the scenes vector
                    m_Scenes.pop_back();
                    
                    //Decrement the pop count
                    m_PopCount--;
                }
            }
        }
        
        //Set the active scenes vector
        m_ActiveScenes = m_Scenes.back();
        
        //
        sort(m_ActiveScenes.begin(), m_ActiveScenes.end(), SceneManager::SortActiveScenes);
        
        //Set that the transition is done
        m_TransitionState = TransitionNone;

        //Cycle through the active scenes and register for input
        for(unsigned int i = 0; i < m_ActiveScenes.size(); i++)
        {
            //If the platform has Mouse input, register the scene for mouse input
            if(ServiceLocator::GetPlatformLayer()->HasMouseInput() == true)
            {
                ServiceLocator::GetInputManager()->AddEventListener(m_ActiveScenes.at(i), MOUSE_MOVEMENT_EVENT);
                ServiceLocator::GetInputManager()->AddEventListener(m_ActiveScenes.at(i), MOUSE_CLICK_EVENT);
                ServiceLocator::GetInputManager()->AddEventListener(m_ActiveScenes.at(i), MOUSE_WHEEL_EVENT);
            }

            //If the platform has Keyboard input, register the scene for keyboard input
            if(ServiceLocator::GetPlatformLayer()->HasKeyboardInput() == true)
            {
                ServiceLocator::GetInputManager()->AddEventListener(m_ActiveScenes.at(i), KEYBOARD_EVENT);
            }

            //If the platform has Touch input, register the scene for touch input
            if(ServiceLocator::GetPlatformLayer()->HasTouchInput() == true)
            {
                ServiceLocator::GetInputManager()->AddEventListener(m_ActiveScenes.at(i), TOUCH_EVENT);
            }

            //If the platform has Gyroscope input, register the scene for Gyroscope input
            if(ServiceLocator::GetPlatformLayer()->HasGyroscopeInput() == true)
            {
                ServiceLocator::GetInputManager()->AddEventListener(m_ActiveScenes.at(i), GYROSCOPE_EVENT);
            }

            //If the platform has Accelerometer input, register the scene for Accelerometer input
            if(ServiceLocator::GetPlatformLayer()->HasAccelerometerInput() == true)
            {
                ServiceLocator::GetInputManager()->AddEventListener(m_ActiveScenes.at(i), ACCELEROMETER_EVENT);
            }

            //If the platform has controller input, register the scene for controller input
            if(ServiceLocator::GetPlatformLayer()->HasControllerInput() == true)
            {
                ServiceLocator::GetInputManager()->AddEventListener(m_ActiveScenes.at(i), CONTROLLER_BUTTON_EVENT);
                ServiceLocator::GetInputManager()->AddEventListener(m_ActiveScenes.at(i), CONTROLLER_ANALOG_EVENT);
                ServiceLocator::GetInputManager()->AddEventListener(m_ActiveScenes.at(i), CONTROLLER_ANALOG_STICK_EVENT);
            }
        }

        //Show the Loading UI
        if(IsActiveSceneLoaded() == false)
        {
            ServiceLocator::GetLoadingUI()->Show();
        }
        else
        {
            //Cycle through the active scene's and tell the scene that it will now transition on screen
            for(unsigned int i = 0; i < m_ActiveScenes.size(); i++)
            {
                m_ActiveScenes.at(i)->WillTransitionOn();
            }
        }
    }
    
    bool SceneManager::SortActiveScenes(Scene* aSceneA, Scene* aSceneB)
    {
        return aSceneA->Depth() < aSceneB->Depth();
    }
    
    bool SceneManager::CanUpdate()
    {
        return true;
    }
    
    bool SceneManager::CanDraw()
    {
        return IsActiveSceneLoaded();
    }

    void SceneManager::HandleEvent(Event* aEvent)
    {
        switch(aEvent->GetEventCode())
        {
            case TIMER_DID_FINISH_EVENT:
            {
                SetActiveScene();
            }
            break;
            
            case RESIZE_EVENT:
            {
                ResizeEvent* resizeEvent = (ResizeEvent*)aEvent;
            
                //Cycle through and notify ALL the scenes about the resize event
                for(unsigned int i = 0; i < m_Scenes.size(); i++)
                {
                    for(unsigned int j = 0; j < m_Scenes.at(i).size(); j++)
                    {
                        m_Scenes.at(i).at(j)->Resize((unsigned int)resizeEvent->GetSize().x, (unsigned int)resizeEvent->GetSize().y);
                    }
                }
            }
            break;
        
            case CONTROLLER_PLUGGED_IN:
            {
                //If a controller was plugged in, register the active scene for controller input
                for(unsigned int i = 0; i < m_ActiveScenes.size(); i++)
                {
                    ServiceLocator::GetInputManager()->AddEventListener(m_ActiveScenes.at(i), CONTROLLER_BUTTON_EVENT);
                    ServiceLocator::GetInputManager()->AddEventListener(m_ActiveScenes.at(i), CONTROLLER_ANALOG_EVENT);
                    ServiceLocator::GetInputManager()->AddEventListener(m_ActiveScenes.at(i), CONTROLLER_ANALOG_STICK_EVENT);
                }
            }
            break;

            case CONTROLLER_UNPLUGGED:
            {
                //If a controller was unplugged, remove the listeners from the active scene
                for(unsigned int i = 0; i < m_ActiveScenes.size(); i++)
                {
                    ServiceLocator::GetInputManager()->RemoveEventListener(m_ActiveScenes.at(i), CONTROLLER_BUTTON_EVENT);
                    ServiceLocator::GetInputManager()->RemoveEventListener(m_ActiveScenes.at(i), CONTROLLER_ANALOG_EVENT);
                    ServiceLocator::GetInputManager()->RemoveEventListener(m_ActiveScenes.at(i), CONTROLLER_ANALOG_STICK_EVENT);
                }
            }
            break;

            default:
                break;
        }
    }
}