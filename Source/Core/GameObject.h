//
//  GameObject.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-07.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__GameObject__
#define __GameDev2D__GameObject__

#include "FrameworkConfig.h"
#include "BaseObject.h"
#include "../Animation/Tween.h"
#include "../Animation/Easing.h"


using namespace glm;
using namespace std;

namespace GameDev2D
{
    //Conveniance define so that you don't have to do a static cast every time you want to delay a method
    #define DelayGameObjectMethod(method, delay) DelayCallingMethod(static_cast<GameObject::DelayedMethod>(method), delay)

    //Forward declarations
    struct Child;
    class Scene;

    //The GameObject class is intended to be the 'Base' class used to draw content to the Scene. You can add children objects.
    //Updating and Drawing can be enabled and disabled at any time for any reason. Most methods can be overridden.
    //Even though it isn't an abstract class, its not designed to be instantiated on its own, but rather to be inherited from.
    class GameObject : public BaseObject
    {
    public:
        //The GameObject constructor takes the type of object that is being created.
        GameObject(const string& type);
        virtual ~GameObject();
        
        //Used to Update() and Draw() the GameObject
        virtual void Update(double delta);
        virtual void Draw();
        
        //Used to determine if the GameObject can be Updated or Drawn. Currently returns IsEnabled(), but can be overridden.
        virtual bool CanUpdate();
        virtual bool CanDraw();
        
        //Can be overridden to reset the state of the GameObject, currently it calls Reset() for any children
        virtual void Reset();
        
        //Add a child GameObject, to the current object, the child object will move with its parent object.
        //You can also set if parent object owns the child object (memory ownership). Setting the depth is optional,
        //the higher the depth, the 'closer' to the front the object will be drawn at. Can be overriden
        virtual GameObject* AddChild(GameObject* gameObject, bool ownsGameObject, unsigned char depth = 0);
        
        //Removes a child object, if the parent owns the child being removed than the child will be delete from
        //memory. Can be overriden.
        virtual void RemoveChild(GameObject* gameObject);
        
        //Returns the Scene that this GameObject is part of. Can be overridden.
        virtual Scene* GetScene() const;
        
        //Returns the Parent that this GameObject is a child of, if any. Can be overridden.
        virtual GameObject* GetParent() const;
        
        //Set the depth of child GameObject, the GameObject must be a child of THIS object. Can be overridden.
        virtual void SetChildDepth(GameObject* gameObject, unsigned char depth);
        
        //Sets the local position of the GameObject, can be applied over timer, using an easing function, can be reversed and repeated. Can be overridden.
        virtual void SetLocalX(float x, double duration = 0.0, EasingFunction easing = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        virtual void SetLocalY(float y, double duration = 0.0, EasingFunction easing = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        virtual void SetLocalPosition(float x, float y, double duration = 0.0, EasingFunction easing = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        virtual void SetLocalPosition(vec2 position, double duration = 0.0, EasingFunction easing = Linear::Interpolation, bool reverse = false, int repeatCount = 0);

        //Returns the local position of the GameObject. It does NOT take into account the parent's angle. Can be overridden
        virtual float GetLocalX();
        virtual float GetLocalY();
        virtual vec2 GetLocalPosition();
        
        //Returns the world position of the GameObject. This takes into account the parent's (if any) local position as well as the local position. Can be overridden.
        virtual float GetWorldX();
        virtual float GetWorldY();
        virtual vec2 GetWorldPosition();

        //Sets the local angle of the GameObject, can be applied over timer, using an easing function, can be reversed and repeated. Can be overridden.
        virtual void SetLocalAngle(float angle, double duration = 0.0, EasingFunction easing = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Returns the local angle, in degress. It does NOT take into account the parent's angle. Can be overridden
        virtual float GetLocalAngle();
        
        //Returns the world angle of the GameObject, in degrees. This takes into accound the parent's (if any) angle as well as the local angle. Can be overridden.
        virtual float GetWorldAngle();
        
        //Sets the scale of the GameObject, can be applied over timer, using an easing function, can be reversed and repeated. Can be overridden.
        virtual void SetLocalScaleX(float scaleX, double duration = 0.0, EasingFunction easing = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        virtual void SetLocalScaleY(float scaleY, double duration = 0.0, EasingFunction easing = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        virtual void SetLocalScale(float scaleX, float scaleY, double duration = 0.0, EasingFunction easing = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        virtual void SetLocalScale(vec2 scale, double duration = 0.0, EasingFunction easing = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Returns the local scale of the GameObject, it does NOT take into account the parent's scale Can be overridden.
        virtual vec2 GetLocalScale();
        
        //Returns the world scale of the GameObject, it takes into account the parent's (if any) scale Can be overridden.
        virtual vec2 GetWorldScale();

        //Getter and setter methods to enable this GameObject, by default if a GameObject is NOT enabled,
        //then it won't be updated OR drawn. However that behaviour can be modified by inheriting classes
        //by modifying the CanUpdate() and CanDraw() methods.
        virtual void SetIsEnabled(bool isEnabled);
        virtual bool IsEnabled();
        
        //Getter and setter methods to force this GameObject's CanUpdate() method to update irregardles if its enabled or not
        virtual void SetForceUpdate(bool forceUpdate);
        virtual bool IsForceUpdated();
        
        //Delayed method function pointer definition and method to initiate calling a delayed method
        typedef void (GameObject::*DelayedMethod)();
        void DelayCallingMethod(DelayedMethod delayedMethod, double delay);
        
        //Resets the model matrix, can be overridden.
        virtual void ResetModelMatrix();
        
        //This function indicates that the model matrix is dirty, there is an option to reset the
        //model matrix immediately. If the matrix is NOT reset immediately it will be done after
        //the GameObject has been updated.
        void ModelMatrixIsDirty(bool resetImmediately = false);
        
        //Returns wether the model matrix is dirty
        bool IsModelMatrixDirty();
        
        //Returns the number of children attached to this GameObject
        unsigned long GetNumberOfChildren();
        
        //Returns the child GameObject for a given index
        GameObject* GetChildAtIndex(unsigned int index);
        
        //Returns the depth of the child GameObject for a given index
        unsigned char GetDepthForChildAtIndex(unsigned int index);
        
        //Method to cancel any running tweens
        virtual void CancelTweens();
        
    protected:
        //Conveniance methods to set the Scene, there can only be one Scene set
        void SetScene(Scene* scene);
        
        //Conveniance methods to set the parent GameObject, there can only be one parent GameObject
        void SetParent(GameObject* parent);
    
        //Conveniance method used to sort the GameObject's children, based on their 'depth'
        void SortChildren();
        
        //Function pointer definition and conveniance tween method
        typedef void (GameObject::*TweenSetMethod)(float value, double duration, EasingFunction easingFunction, bool reverse, int repeat);
        void UpdateTween(double delta, Tween* tween, TweenSetMethod tweenSetMethod);
        
        //Conveniance method to update a delayed method call, if the delay reaches zero, this
        //method will call the delayed method and return true, otherwise it will return false
        bool UpdateDelayedMethod(double delta, pair<DelayedMethod, double>& delayedMethodPair);

public:
        //The model matrix member variable is available to inheriting classes
        mat4 m_ModelMatrix;
    
    private:
        //Private member variables
        Scene* m_Scene;
        GameObject* m_Parent;
        vector<Child> m_Children;
        vec2 m_Position;
        vec2 m_Scale;
        float m_Angle;
        bool m_IsEnabled;
        bool m_ForceUpdate;
        bool m_ResetModelMatrix;
        
        //Tween member variables
        Tween m_TweenLocalX;
        Tween m_TweenLocalY;
        Tween m_TweenAngle;
        Tween m_TweenScaleX;
        Tween m_TweenScaleY;
        
        //Delayed method member variables, the pair holds the method to be delayed and the delay duration
        vector<pair<DelayedMethod, double>> m_DelayedMethods;
    };
    
    //Struct to keep track of a GameObject's child, the child's depth and wether
    //the GameObject owns the child (ownership in terms of releasing it from memory)
    struct Child
    {
        GameObject* gameObject;
        unsigned char depth;
        bool ownsGameObject;
        
        bool operator()(Child const & a, Child const & b) const
        {
            return a.depth < b.depth;
        }
    };
}

#endif /* defined(__GameDev2D__GameObject__) */
