//
//  SceneManager.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__SceneManager__
#define __GameDev2D__SceneManager__

#include "../GameService.h"


namespace GameDev2D
{
    //This enum describes the mouse
    //buttons on a standard mouse
    enum TransitionState
    {
        TransitionNone = 0,
        TransitionPush,
        TransitionPop
    };

    //Forward declarations
    class Scene;
    class Timer;

    //The SceneManager is responsible for pushing and popping scene's onto the view, it draws
    //and updates the active Scene. The SceneManager registers for input when a Scene becomes
    //active and removes the input listener when the Scene is no longer active. In the future
    //it could support multiple Scene active at one time.
    class SceneManager : public GameService
    {
    public:
        SceneManager();
        ~SceneManager();
        
        //Used to Update and Draw the SceneManager
        void Update(double delta);
        void Draw();
        
        //Updates a specific Scene object
        void UpdateScene(Scene* scene, double delta);
        
        //Draws a specific Scene object - sets the Graphics current Camera
        void DrawScene(Scene* scene);
        
        //Returns the active Scenes vector
        vector<Scene*> GetActiveScenes();
        
        //Returns the Scene associated with the name, can return null if the Scene
        //associated with the name isn't found
        Scene* GetScene(const string& name);
        
        //Push and Pop scenes, pop will removing the active scene(s) (unless it's the root scene)
        void PushScene(Scene* scene, bool responsibleForDeletion);
        
        //Push scenes, pop will removing the active scene(s) (unless it's the root scene)
        void PushScenes(vector<Scene*>& scenes, vector<bool>& responsibleForDeletion);
        
        //Pops the active scene(s) unless it is the root scene that is being popped, that isn't allowed
        //the parameter allows you to inidicate how many scenes to pop, by default only 1 scene will be popped
        void Pop(unsigned int count = 1);
        
        //Adds a Scene to the Active Scene stack
        void AddScene(Scene* scene, bool responsibleForDeletion);
        
        //Methods to remove a Scene, by either name or pointer, if the SceneManager is responsible
        //for deleting the Scene it will be deleted at this time
        void RemoveScene(Scene* scene);
        void RemoveScene(const string& name);
        
        //Call this method to preload a Scene to avoid an unwanted loading scene, its recommended
        //to only call this in another Scene's LoadContent() method to avoid a frame rate hiccup
        void PreloadScene(Scene* scene);
        
        //Returns wether the scene pointer is part of the active scenes vector
        bool IsActiveScene(Scene* scene);
        
        //Tells the ServiceLocator wether to Update and Draw this Game Service
        bool CanUpdate();
        bool CanDraw();
        
    protected:
        //Called from the PlatformLayer to set the root Scene object
        void SetRootScene(Scene* scene);
        
        //Sets the active scene
        void SetActiveScene();
        
        //Sorts the active scenes based on the Scene's Depth() method
        static bool SortActiveScenes(Scene* sceneA, Scene* sceneB);
        
        //Methods relating to Scene loading
        void LoadActiveScene();
        bool IsActiveSceneLoaded();
        
        //Event Handling method
        void HandleEvent(Event* event);

        //Allows the PlatformLayer access to the SetRootScene() method
        friend class PlatformLayer;
        
    private:
        //Member variables
        vector<vector<Scene*>> m_Scenes;
        vector<Scene*> m_ActiveScenes;
        Scene* m_RootScene;
        Timer* m_TransitionTimer;
        TransitionState m_TransitionState;
        unsigned int m_PopCount;
    };
}

#endif /* defined(__GameDev2D__ScreenManager__) */
