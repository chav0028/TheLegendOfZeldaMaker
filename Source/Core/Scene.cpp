//
//  Scene.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-27.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Scene.h"
#include "GameObject.h"
#include "../Graphics/Core/Camera.h"
#include "../Services/ServiceLocator.h"
#include "../Services/Graphics/Graphics.h"


namespace GameDev2D
{
    Scene::Scene(const string& aType) : BaseObject(aType), EventHandler(),
        m_Camera(nullptr),
        m_SceneManagerResponsibleForDeletion(false),
        m_IsLoaded(false)
    {
        //Create a new Camera object
        m_Camera = new Camera();
        m_Camera->ResetProjectionMatrix();
        m_Camera->ResetViewMatrix();
    }
    
    Scene::~Scene()
    {
        //Delete the Camera object
        SafeDelete(m_Camera);
        
        //Cycle through and remove all the GameObjects in the 'to add' vector
        for(unsigned int i = 0; i < m_GameObjectsToAdd.size(); i++)
        {
            SafeDelete(m_GameObjectsToAdd.at(i).gameObject);
        }
        
        //Cycle through and remove all the GameObjects in the 'to remove' vector
        for (unsigned int i = 0; i < m_GameObjectsToRemove.size(); i++)
        {
            int index = GetIndexForGameObject(m_GameObjectsToRemove.at(i));
            if(index != -1)
            {
                //Delete the object from memory
                SafeDelete(m_GameObjectsToRemove.at(i));
                m_SceneObjects.erase(m_SceneObjects.begin() + index);
            }
        }
        m_GameObjectsToRemove.clear();
        
        //Cycle through the remaining GameObjects and delete them
        for(unsigned int i = 0; i < m_SceneObjects.size(); i++)
        {
            SafeDelete(m_SceneObjects.at(i).gameObject);
        }
        m_SceneObjects.clear();
        
        //Clear the delayed methods vector
        m_DelayedMethods.clear();
    }
    
    void Scene::Update(double aDelta)
    {
        //Update the Camera
        if(m_Camera != nullptr)
        {
            m_Camera->Update(aDelta);
        }
        
        //Cycle through the delayed methods and update them and call them if needed
        for (unsigned int i = 0; i < m_DelayedMethods.size(); i++)
        {
            if(UpdateDelayedMethod(aDelta, m_DelayedMethods.at(i)) == true)
            {
                m_DelayedMethods.erase(m_DelayedMethods.begin() + i);
            }
        }
        
        //Cycle through and update the GameObjects
        for(unsigned int i = 0; i < m_SceneObjects.size(); i++)
        {
            //Can the GameObject be updated?
            if(m_SceneObjects.at(i).gameObject->CanUpdate() == true)
            {
                //Update the GameObject
                m_SceneObjects.at(i).gameObject->Update(aDelta);
                
                //Is the model matrix dirty
                if(m_SceneObjects.at(i).gameObject->IsModelMatrixDirty() == true)
                {
                    //Reset the GameObject's Model Matrix
                    m_SceneObjects.at(i).gameObject->ResetModelMatrix();
                }
            }
        }
        
        //Process any GameObjects needed to be added
        for (unsigned int i = 0; i < m_GameObjectsToAdd.size(); i++)
        {
            SceneObject sceneObject;
            sceneObject.gameObject = m_GameObjectsToAdd.at(i).gameObject;
            sceneObject.depth = m_GameObjectsToAdd.at(i).depth;
            m_SceneObjects.push_back(sceneObject);
        }
        
        //If we added any GameObjects we need to sort the SceneObjects
        if(m_GameObjectsToAdd.size() > 0)
        {
            SortSceneObjects();
        }
        
        //Clear the GameObjectsToAdd vector
        m_GameObjectsToAdd.clear();
        
        //Cycle through and remove all the Game Objects in the 'to remove' vector
        for (unsigned int i = 0; i < m_GameObjectsToRemove.size(); i++)
        {
            int index = GetIndexForGameObject(m_GameObjectsToRemove.at(i));
            if(index != -1)
            {
                //Delete the object from memory
                SafeDelete(m_GameObjectsToRemove.at(i));
                m_SceneObjects.erase(m_SceneObjects.begin() + index);
            }
        }
        
        //Clear the GameObjectsToRemove vector
        m_GameObjectsToRemove.clear();
    }
    
    void Scene::Draw()
    {
        //Cycle through and Draw the GameObjects
        for(unsigned int i = 0; i < m_SceneObjects.size(); i++)
        {
            //Can the GameObject be drawn?
            if(m_SceneObjects.at(i).gameObject->CanDraw() == true)
            {
                //This is only here to ENSURE that if the model matrix is dirty
                //that it get reset before the draw call
                if(m_SceneObjects.at(i).gameObject->IsModelMatrixDirty() == true)
                {
                    //Reset the GameObject's Model Matrix
                    m_SceneObjects.at(i).gameObject->ResetModelMatrix();
                }
            
                //Draw the GameObject
                m_SceneObjects.at(i).gameObject->Draw();
            }
        }
    }
    
    void Scene::Reset()
    {
        //Cycle through and Reset the GameObjects
        for(unsigned int i = 0; i < m_SceneObjects.size(); i++)
        {
            //Reset the GameObject
            m_SceneObjects.at(i).gameObject->Reset();
        }
        
        //Also cycle through the Scene objects that will be added
        for (unsigned int i = 0; i < m_GameObjectsToAdd.size(); i++)
        {
            //Reset the GameObject
            m_GameObjectsToAdd.at(i).gameObject->Reset();
        }
    }
    
    void Scene::HandleEvent(Event* aEvent)
    {
        //Needs to be overridden in inheriting classes
    }
    
    void Scene::Resize(unsigned int aWidth, unsigned int aHeight)
    {
        //Needs to be overridden in inheriting classes
    }
    
    GameObject* Scene::AddGameObject(GameObject* aGameObject, unsigned char aDepth)
    {
        //Safety check that the GameObject isn't already in the Scene. If the assert below is hit,
        //that means the GameObject you are trying to add is ALREADY in the Scene.
        #if DEBUG
        bool containsObject = false;
        for(unsigned int i = 0; i < GetNumberOfSceneObjects(); i++)
        {
            if(GetSceneObjectAtIndex(i) == aGameObject)
            {
                containsObject = true;
                break;
            }
        }
        assert(containsObject == false);
        #endif
    
        //Safety check the GameObject
        if(aGameObject != nullptr)
        {
            //Setup the SceneObject struct
            SceneObject sceneObject;
            sceneObject.gameObject = aGameObject;
            sceneObject.depth = aDepth;
        
            //Is the scene loaded or not
            if(IsLoaded() == false)
            {
                m_SceneObjects.push_back(sceneObject);
            }
            else
            {
                //Push the SceneObject onto the to add vector
                m_GameObjectsToAdd.push_back(sceneObject);
            }
        }
        
        //Return the Scene object
        return aGameObject;
    }
    
    void Scene::RemoveGameObject(GameObject* aGameObject)
    {
        if(aGameObject != nullptr)
        {
            m_GameObjectsToRemove.push_back(aGameObject);
        }
    }
    
    void Scene::WillTransitionOn()
    {

    }
        
    double Scene::WillTransitionOff()
    {
        return 0.0;
    }
    
    Camera* Scene::GetCamera() const
    {
        return m_Camera;
    }
    
    unsigned int Scene::GetNumberOfSceneObjects()
    {
        return (unsigned int)m_SceneObjects.size();
    }
    
    GameObject* Scene::GetSceneObjectAtIndex(unsigned int aIndex)
    {
        if(aIndex < GetNumberOfSceneObjects())
        {
            return m_SceneObjects.at(aIndex).gameObject;
        }
        return nullptr;
    }
    
    unsigned char Scene::GetDepthForSceneObjectAtIndex(unsigned int aIndex)
    {
        if(aIndex < GetNumberOfSceneObjects())
        {
            return m_SceneObjects.at(aIndex).depth;
        }
        return 0;
    }
    
    void Scene::SetSceneObjectDepth(GameObject* aGameObject, unsigned char aDepth)
    {
        bool sortSceneObjects = false;
    
        //Cycle through SceneObjects vector and find the GameObject to change its depth
        for(unsigned int i = 0; i < m_SceneObjects.size(); i++)
        {
            if(aGameObject == m_SceneObjects.at(i).gameObject)
            {
                //Set the new depth for the Scene object
                m_SceneObjects.at(i).depth = aDepth;
                sortSceneObjects = true;
                break;
            }
        }
        
        //If we need to sort the Scene objects, then sort them
        if(sortSceneObjects == true)
        {
            SortSceneObjects();
        }
    }
    
    void Scene::DelayCallingMethod(DelayedMethod aDelayedMethod, double aDelay)
    {
        m_DelayedMethods.push_back(make_pair(aDelayedMethod, aDelay));
    }
    
    bool Scene::UpdateDelayedMethod(double aDelta, pair<DelayedMethod, double>& aDelayedMethodPair)
    {
        //Countdown the delay
        aDelayedMethodPair.second -= aDelta;
        
        //If the delay has reached zero, call the method and return true
        if(aDelayedMethodPair.second <= 0.0)
        {
            (this->*aDelayedMethodPair.first)();
            return true;
        }
    
        return false;
    }

    void Scene::SortSceneObjects()
    {
        sort(m_SceneObjects.begin(), m_SceneObjects.end(), SceneObject());
    }
    
    int Scene::GetIndexForGameObject(GameObject* aGameObject)
    {
        if(aGameObject != nullptr)
        {
            for (unsigned int i = 0; i < m_SceneObjects.size(); i++)
            {
                if(m_SceneObjects.at(i).gameObject == aGameObject)
                {
                    return i;
                }
            }
        }
        return -1;
    }
    
    void Scene::SetSceneManagerResponsibleForDeletion(bool aResponsibleForDeletion)
    {
        m_SceneManagerResponsibleForDeletion = aResponsibleForDeletion;
    }
    
    bool Scene::IsSceneManagerResponsibleForDeletion()
    {
        return m_SceneManagerResponsibleForDeletion;
    }

    void Scene::SetIsLoaded(bool aIsLoaded)
    {
        m_IsLoaded = aIsLoaded;
    }
    
    bool Scene::IsLoaded()
    {
        return m_IsLoaded;
    }
    
    unsigned int Scene::Depth()
    {
        return 0;
    }
}