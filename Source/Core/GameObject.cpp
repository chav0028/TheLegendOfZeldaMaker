//
//  GameObject.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-07.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "GameObject.h"



namespace GameDev2D
{
    GameObject::GameObject(const string& type) : BaseObject(type),
        m_ModelMatrix(mat4(1.0f)),
        m_Position(0.0f, 0.0f),
        m_Scale(1.0f, 1.0f),
        m_Scene(nullptr),
        m_Parent(nullptr),
        m_Angle(0.0f),
        m_IsEnabled(true),
        m_ResetModelMatrix(false),
        m_ForceUpdate(false),
        m_TweenLocalX(),
        m_TweenLocalY(),
        m_TweenAngle(),
        m_TweenScaleX(),
        m_TweenScaleY()
    {

    }

    GameObject::~GameObject()
    {
        //Cycle through and delete any children objects the parent owns
        for(unsigned int i = 0; i < m_Children.size(); i++)
        {
            if(m_Children.at(i).ownsGameObject == true)
            {
                SafeDelete(m_Children.at(i).gameObject);
            }
        }
        
        //Clear the children vector
        m_Children.clear();
        
        //Clear the delayed methods vector
        m_DelayedMethods.clear();
    }
            
    void GameObject::Update(double aDelta)
    {
        //Update the tweens, for position, angle and scale
        UpdateTween(aDelta, &m_TweenLocalX, &GameObject::SetLocalX);
        UpdateTween(aDelta, &m_TweenLocalY, &GameObject::SetLocalY);
        UpdateTween(aDelta, &m_TweenAngle, &GameObject::SetLocalAngle);
        UpdateTween(aDelta, &m_TweenScaleX, &GameObject::SetLocalScaleX);
        UpdateTween(aDelta, &m_TweenScaleY, &GameObject::SetLocalScaleY);
    
        //Cycle through the delayed methods and update them and call them if needed
        for (unsigned int i = 0; i < m_DelayedMethods.size(); i++)
        {
            if(UpdateDelayedMethod(aDelta, m_DelayedMethods.at(i)) == true)
            {
                m_DelayedMethods.erase(m_DelayedMethods.begin() + i);
            }
        }
    
        //Cycle through the children and update them, if they can be updated
        for(unsigned int i = 0; i < GetNumberOfChildren(); i++)
        {
            if(GetChildAtIndex(i)->CanUpdate() == true)
            {
                GetChildAtIndex(i)->Update(aDelta);
            }
        }
    }

    void GameObject::Draw()
    {
        //Check to see if the model matrix is dirty
        if(IsModelMatrixDirty() == true)
        {
            ResetModelMatrix();
        }
    
        //Cycle through the children and draw them, if they can be drawn
        for(unsigned int i = 0; i < GetNumberOfChildren(); i++)
        {
            if(GetChildAtIndex(i)->CanDraw() == true)
            {
                //Get the child
                GameObject* child = (GameObject*)GetChildAtIndex(i);
                
                //Is the model matrix dirty
                if(child->IsModelMatrixDirty() == true)
                {
                    child->ResetModelMatrix();
                }
                
                //Draw the child
                child->Draw();
            }
        }
    }
    
    bool GameObject::CanUpdate()
    {
        return IsEnabled() || IsForceUpdated();
    }
    
    bool GameObject::CanDraw()
    {
        return IsEnabled();
    }
    
    void GameObject::Reset()
    {
        //Cycle through all the children and reset them
        for(unsigned int i = 0; i < GetNumberOfChildren(); i++)
        {
            GetChildAtIndex(i)->Reset();
        }
    }
    
    GameObject* GameObject::AddChild(GameObject* aGameObject, bool aOwnsObject, unsigned char aDepth)
    {
        //If you hit this assert, that means the child you are trying to add already has a parent
        assert(aGameObject->GetParent() == nullptr);
    
        //Safety check that the child isn't already in the children's list. If the assert below is hit,
        //that means the child you are trying to add is ALREADY a child of this parent.
        #if DEBUG
        bool containsChild = false;
        for(unsigned int i = 0; i < GetNumberOfChildren(); i++)
        {
            if(GetChildAtIndex(i) == aGameObject)
            {
                containsChild = true;
                break;
            }
        }
        assert(containsChild == false);
        #endif
    
        //Safety check the child object
        if(aGameObject != nullptr)
        {
            //Setup the child struct
            Child child;
            child.gameObject = aGameObject;
            child.ownsGameObject = aOwnsObject;
            child.depth = aDepth;
            
            //Set the child object's parent
            aGameObject->SetParent(this);
            
            //Flag the model matrix as dirty
            aGameObject->ModelMatrixIsDirty();
            
            //Push the child onto the vector of children
            m_Children.push_back(child);
            
            //Sort the children based on their depth
            SortChildren();
        }
        
        //Return the child object
        return aGameObject;
    }
    
    void GameObject::RemoveChild(GameObject* aGameObject)
    {
        //Cycle through the children vector and find the child to be removed
        for(unsigned int i = 0; i < m_Children.size(); i++)
        {
            if(aGameObject == m_Children.at(i).gameObject)
            {
                //If the parent own's the child object, we need to delete it
                if(m_Children.at(i).ownsGameObject == true)
                {
                    SafeDelete(m_Children.at(i).gameObject);
                }
                
                //Erase the child object from the children vector
                m_Children.erase(m_Children.begin() + i);
                break;
            }
        }
    }
    
    Scene* GameObject::GetScene() const
    {
        return m_Scene;
    }
    
    GameObject* GameObject::GetParent() const
    {
        return m_Parent;
    }
    
    void GameObject::SetChildDepth(GameObject* aGameObject, unsigned char aDepth)
    {
        bool sortChildren = false;
    
        //Cycle through children vector and find the child object to change its depth
        for(unsigned int i = 0; i < m_Children.size(); i++)
        {
            if(aGameObject == m_Children.at(i).gameObject)
            {
                //Set the new depth for the child object
                m_Children.at(i).depth = aDepth;
                sortChildren = true;
                break;
            }
        }
        
        //If we need to sort the children, then sort them
        if(sortChildren == true)
        {
            SortChildren();
        }
    }
    
    void GameObject::SetLocalX(float aX, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aX != m_Position.x)
        {
            if(aDuration == 0.0)
            {
                m_Position.x = aX;
                ModelMatrixIsDirty();
            }
            else
            {
                if(m_Position.x != aX)
                {
                    m_TweenLocalX.Set(m_Position.x, aX, aDuration, aEasingFunction, aReverse, aRepeatCount);
                }
            }
        }
    }

    void GameObject::SetLocalY(float aY, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aY != m_Position.y)
        {
            if(aDuration == 0.0)
            {
                m_Position.y = aY;
                ModelMatrixIsDirty();
            }
            else
            {
                if(m_Position.y != aY)
                {
                    m_TweenLocalY.Set(m_Position.y, aY, aDuration, aEasingFunction, aReverse, aRepeatCount);
                }
            }
        }
    }

    void GameObject::SetLocalPosition(float aX, float aY, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        SetLocalX(aX, aDuration, aEasingFunction, aReverse, aRepeatCount);
        SetLocalY(aY, aDuration, aEasingFunction, aReverse, aRepeatCount);
    }

    void GameObject::SetLocalPosition(vec2 aPosition, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        SetLocalPosition(aPosition.x, aPosition.y, aDuration, aEasingFunction, aReverse, aRepeatCount);
    }
    
    float GameObject::GetLocalX()
    {
        return GetLocalPosition().x;
    }
    
    float GameObject::GetLocalY()
    {
        return GetLocalPosition().y;
    }
    
    vec2 GameObject::GetLocalPosition()
    {
        return m_Position;
    }
    
    float GameObject::GetWorldX()
    {
        return GetWorldPosition().x;
    }

    float GameObject::GetWorldY()
    {
        return GetWorldPosition().y;
    }

    vec2 GameObject::GetWorldPosition()
    {
        if(m_Parent != nullptr)
        {
            return m_Parent->GetWorldPosition() + GetLocalPosition();
        }
        return GetLocalPosition();
    }
    
    void GameObject::SetLocalAngle(float aAngle, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aAngle != m_Angle)
        {
            if(aDuration == 0.0)
            {
                m_Angle = aAngle;
                ModelMatrixIsDirty();
            }
            else
            {
                if(m_Angle != aAngle)
                {
                    m_TweenAngle.Set(m_Angle, aAngle, aDuration, aEasingFunction, aReverse, aRepeatCount);
                }
            }
        }
    }
    
    float GameObject::GetLocalAngle()
    {
        return m_Angle;
    }
    
    float GameObject::GetWorldAngle()
    {
        if(m_Parent != nullptr)
        {
            return m_Parent->GetWorldAngle() + GetLocalAngle();
        }
        return m_Angle;
    }
    
    void GameObject::SetLocalScaleX(float aScaleX, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aScaleX != m_Scale.x)
        {
            if(aDuration == 0.0)
            {
                m_Scale.x = aScaleX;
                ModelMatrixIsDirty();
            }
            else
            {
                if(m_Scale.x != aScaleX)
                {
                    m_TweenScaleX.Set(m_Scale.x, aScaleX, aDuration, aEasingFunction, aReverse, aRepeatCount);
                }
            }
        }
    }
    
    void GameObject::SetLocalScaleY(float aScaleY, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aScaleY != m_Scale.y)
        {
            if(aDuration == 0.0)
            {
                m_Scale.y = aScaleY;
                ModelMatrixIsDirty();
            }
            else
            {
                if(m_Scale.y != aScaleY)
                {
                    m_TweenScaleY.Set(m_Scale.y, aScaleY, aDuration, aEasingFunction, aReverse, aRepeatCount);
                }
            }
        }
    }
    
    void GameObject::SetLocalScale(float aScaleX, float aScaleY, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        SetLocalScaleX(aScaleX, aDuration, aEasingFunction, aReverse, aRepeatCount);
        SetLocalScaleY(aScaleY, aDuration, aEasingFunction, aReverse, aRepeatCount);
    }
    
    void GameObject::SetLocalScale(vec2 aScale, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        SetLocalScale(aScale.x, aScale.y, aDuration, aEasingFunction, aReverse, aRepeatCount);
    }
    
    vec2 GameObject::GetLocalScale()
    {
        return m_Scale;
    }
    
    vec2 GameObject::GetWorldScale()
    {
        if(m_Parent != nullptr)
        {
            return m_Parent->GetWorldScale() * GetLocalScale();
        }
        return m_Scale;
    }

    void GameObject::SetIsEnabled(bool aIsEnabled)
    {
        m_IsEnabled = aIsEnabled;
    }
    
    void GameObject::SetForceUpdate(bool aForceUpdate)
    {
        m_ForceUpdate = aForceUpdate;
    }
    
    bool GameObject::IsForceUpdated()
    {
        return m_ForceUpdate;
    }

    bool GameObject::IsEnabled()
    {
      return m_IsEnabled;
    }
    
    void GameObject::SetScene(Scene* aScene)
    {
        m_Scene = aScene;
    }
    
    void GameObject::SetParent(GameObject* aParent)
    {
        m_Parent = aParent;
    }
    
    void GameObject::SortChildren()
    {
        sort(m_Children.begin(), m_Children.end(), Child());
    }
    
    void GameObject::UpdateTween(double aDelta, Tween* aTween, TweenSetMethod aTweenSetMethod)
    {
        //Safety check the Tween and make sure it is running
        if(aTween != nullptr && aTween->IsRunning() == true)
        {
            //Update the Tween
            aTween->Update(aDelta);
            
            //Using the function pointer, call the set method
            //and pass the Tween's current value as the parameter
            (this->*aTweenSetMethod)(aTween->GetCurrent(), 0.0, nullptr, false, 0);
        }
    }
    
    void GameObject::DelayCallingMethod(DelayedMethod aDelayedMethod, double aDelay)
    {
        m_DelayedMethods.push_back(make_pair(aDelayedMethod, aDelay));
    }
    
    bool GameObject::UpdateDelayedMethod(double aDelta, pair<DelayedMethod, double>& aDelayedMethodPair)
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
    
    void GameObject::ResetModelMatrix()
    {
        //Cycle through the children's model matrices and reset them
        for(unsigned int i = 0; i < GetNumberOfChildren(); i++)
        {
            //Reset the child object's model matrix
            GetChildAtIndex(i)->ResetModelMatrix();
        }
    
        m_ResetModelMatrix = false;
    }
    
    void GameObject::ModelMatrixIsDirty(bool aResetImmediately)
    {
        if(aResetImmediately == true)
        {
            ResetModelMatrix();
        }
        else
        {
            m_ResetModelMatrix = true;
        }
    }
    
    bool GameObject::IsModelMatrixDirty()
    {
        bool isDirty = false;
        if(m_Parent != nullptr)
        {
            isDirty = m_Parent->IsModelMatrixDirty();
        }
        
        if(isDirty == false)
        {
            isDirty = m_ResetModelMatrix;
        }
        
        return isDirty;
    }
    
    unsigned long GameObject::GetNumberOfChildren()
    {
        return m_Children.size();
    }
    
    GameObject* GameObject::GetChildAtIndex(unsigned int aIndex)
    {
        if(aIndex < GetNumberOfChildren())
        {
            return m_Children.at(aIndex).gameObject;
        }
    
        return nullptr;
    }
    
    unsigned char GameObject::GetDepthForChildAtIndex(unsigned int aIndex)
    {
        if(aIndex < GetNumberOfChildren())
        {
            return m_Children.at(aIndex).depth;
        }
    
        return 0;
    }
    
    void GameObject::CancelTweens()
    {
        m_TweenLocalX.Cancel();
        m_TweenLocalY.Cancel();
        m_TweenAngle.Cancel();
        m_TweenScaleX.Cancel();
        m_TweenScaleY.Cancel();
    }
}