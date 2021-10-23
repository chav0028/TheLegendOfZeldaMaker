//
//  Scene.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-27.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Scene__
#define __GameDev2D__Scene__

#include "FrameworkConfig.h"
#include "BaseObject.h"
#include "../Events/EventHandler.h"


using namespace std;

namespace GameDev2D
{
    //Conveniance define so that you don't have to do a static cast every time you want to delay a method
    #define DelaySceneMethod(method, delay) DelayCallingMethod(static_cast<BaseObject::DelayedMethod>(method), delay)

    //Forward declarations
    class GameObject;
    class Camera;
    struct SceneObject;

    //The Scene class is an abstract class (can't be instantiated), and MUST be inherited from. It's intended to
    //be used for GameObject management, the intend is that each menu could its own Scene, as well as the Game
    //and any other Game scenes. The Scene is also an EventHandler, so it can register for any events and have
    //them sent to the inheriting class, make sure to override the HandleEvent() method. The Scene owns the memory
    //for any added GameObjects.
    class Scene : public BaseObject, public EventHandler
    {
    public:
        //The constructor takes in the type of scene.
        Scene(const string& type);
        virtual ~Scene();

        //Pure virtual method, it must be implemented by inheriting classes. Its used to load game content,
        //return 1.0f when loading is complete, otherwise load the percentage of game content that has been loaded
        virtual float LoadContent() = 0;
        
        //The Update() method updates all the GameObjects in the Scene. Can be overridden.
        virtual void Update(double delta);
        
        //The Draw() method draws all the GameObjects in the Scene. Can be overridden.
        virtual void Draw();
        
        //Calls the Reset() method for all GameObjects in the Scene, Can be overridden.
        virtual void Reset();
        
        //The event handler method, used to handle all Events used by the Scene, can be overridden.
        virtual void HandleEvent(Event* event);
        
        //The Resize() method is automatically called when the application's view is resized, the SceneManager handles
        //the resize event and notifies every scene it manages. Can be overridden (and should be).
        virtual void Resize(unsigned int width, unsigned int height);
        
        //Adds a GameObject to the scene, it is responsible for updating and
        //drawing the GameObject. Can be overridden.
        virtual GameObject* AddGameObject(GameObject* gameObject, unsigned char depth = 0);
        
        //Removes a GameObject from the Scene. Can be overridden.
        virtual void RemoveGameObject(GameObject* gameObject);
        
        //Called when the Scene is about to be transitioned On screen. Can be overridden.
        virtual void WillTransitionOn();
        
        //Called when the Scene is about to be transitioned Off screen. Returns how long
        //it will take to transition Off screen (in seconds). This will allow additional
        //time to perform any additional exit animations. Can be overridden.
        virtual double WillTransitionOff();
        
        //Returns a pointer to the scene's camera. Can be overridden.
        virtual Camera* GetCamera() const;
        
        //Returns the number of GameObjects in the Scene. Can be overridden.
        virtual unsigned int GetNumberOfSceneObjects();
        
        //Returns the GameObject for a given index. Can be overridden.
        virtual GameObject* GetSceneObjectAtIndex(unsigned int index);
        
        //Returns the depth of the child GameObject for a given index. Can be overridden.
        virtual unsigned char GetDepthForSceneObjectAtIndex(unsigned int index);
        
        //Set the depth of GameObject, the GameObject must be part of the Scene. Can be overridden.
        virtual void SetSceneObjectDepth(GameObject* gameObject, unsigned char depth);
        
        //Delayed method function pointer definition and method to initiate calling a delayed method
        void DelayCallingMethod(DelayedMethod delayedMethod, double delay);
        
    protected:
        //Conveniance method used to sort the Scene's GameObjects, based on their 'depth'. Can be overridden.
        virtual void SortSceneObjects();
    
        //Returns the index for a GameObject. Can be overridden.
        virtual int GetIndexForGameObject(GameObject* gameObject);
    
        //These methods are called by the scene manager, they manage scene ownership
        void SetSceneManagerResponsibleForDeletion(bool responsibleForDeletion);
        bool IsSceneManagerResponsibleForDeletion();
        
        //These methods are called by the scene manager, they handle the loading state
        void SetIsLoaded(bool isLoaded);
        bool IsLoaded();
        
        //Sets the depth of the scene, by default it is zero. Can be overridden
        virtual unsigned int Depth();
        
        //Conveniance method to update a delayed method call, if the delay reaches zero, this
        //method will call the delayed method and return true, otherwise it will return false
        bool UpdateDelayedMethod(double delta, pair<DelayedMethod, double>& delayedMethodPair);
        
        //The scene manager is a friend class, it helps with loading content, and will
        //display the LoadingUI while scene content is being loaded
        friend class SceneManager;
        
    private:
        //Member variables
        Camera* m_Camera;
        vector<SceneObject> m_SceneObjects;
        vector<SceneObject> m_GameObjectsToAdd;
        vector<GameObject*> m_GameObjectsToRemove;
        vector<pair<DelayedMethod, double>> m_DelayedMethods;
        bool m_SceneManagerResponsibleForDeletion;
        bool m_IsLoaded;
    };
    
    //Struct to keep track the Scene's GameObjects, it manages the GameObject's depth
    struct SceneObject
    {
        GameObject* gameObject;
        unsigned char depth;
        
        bool operator()(SceneObject const & a, SceneObject const & b) const
        {
            return a.depth < b.depth;
        }
    };
}

#endif /* defined(__GameDev2D__Scene__) */
