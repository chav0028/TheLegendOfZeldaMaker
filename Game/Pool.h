/*******************************************************************
Pool.h

Student:        Alvaro Chavez Mixco
Algonquin College
Date:           December 12, 2015
Course Number:  GAM1540
Professor:      David McCue
Purpose:
Template class that basically contains  2 list of objects, active and inactive
ones. The class involves managing the objects and in which list they are.
*********************************************************************/

#ifndef __GameDev2D__Pool__
#define __GameDev2D__Pool__

#include "../Source/Core/GameObject.h"

using namespace glm;

namespace GameDev2D
{
    class Projectile;
    class SubSection;
    class Sprite;

    /***************************************************************
    Class: Pool
    Template class that basically contains  2 list of objects, active and inactive
    ones. The class involves managing the objects and in which list they ar
    ***************************************************************/
    template<class T>
    class Pool
    {

    public:
        Pool(unsigned int numberObjects);
        ~Pool();

        void DeactivateObject(T* object);
        T* GetActivateObject();
        void AddNewObjectsToPool(unsigned int numberObjects);
        void Update(double delta);
        int GetActiveObjectIndex(T* object);
        vector<T*> GetCurrentlyActiveObjects();

    private:
        vector<T*>m_ActiveObjects;
        vector <T*>m_InactiveObjects;
    };

    template<class T>
    Pool<T>::Pool(unsigned int aNumberObjects)
    {
        AddNewObjectsToPool(aNumberObjects);//Create the number of objects
    }

    template<class T>
    void Pool<T>::AddNewObjectsToPool(unsigned int aNumberObjects)
    {
        for (unsigned int i = 0; i < aNumberObjects; i++)
        {
            m_InactiveObjects.push_back(new T(this));//Create an object and set that it belongs to this pool
        }
    }

    template<class T>
    Pool<T>::~Pool()
    {
        //Go through all objects
        while (m_ActiveObjects.empty() == false)
        {
            delete m_ActiveObjects.back();//Delete last object
            m_ActiveObjects.pop_back();//Remove it from vector
        }

        while (m_InactiveObjects.empty() == false)
        {
            delete m_InactiveObjects.back();//Delete last object
            m_InactiveObjects.pop_back();//Remove it from vector
        }
    }

    template<class T>
    T* Pool<T>::GetActivateObject()
    {
        //If we  have aN inactive object left
        if (m_InactiveObjects.empty() == false)
        {
            T* object = m_InactiveObjects.back();//Get the object from the back of the inactive list
            m_InactiveObjects.pop_back();//Remove the object from inactive list

            if (object != nullptr)
            {
                m_ActiveObjects.push_back(object);//Add the object to active list

                return object;//Return the object
            }
        }
        //We could create more objects here if the inactive list is empty

        return nullptr;//Return nullptr
    }

    template<class T>
    void Pool<T>::DeactivateObject(T* aObject)
    {
        m_InactiveObjects.push_back(aObject);//Add object back to inactive list


        int index = GetActiveObjectIndex(aObject);
        if (index != -1)//if the object was find in the active objects
        {
            m_ActiveObjects.erase(m_ActiveObjects.begin() + index);//Remove element from active objects
        }
    }

    template<class T>
    int Pool<T>::GetActiveObjectIndex(T* aObject)
    {
        //Check that there is a valid object
        if (aObject != nullptr)
        {
            //Look for the object in the active projectiles
            for (unsigned int i = 0; i < m_ActiveObjects.size(); i++)
            {
                //If the object is find
                if (m_ActiveObjects.at(i) == aObject)
                {
                    return i;//Return the position at it was find
                }
            }
        }

        return -1;//If it wasn't find , return -1 to indicate an error
    }

    template<class T>
    vector<T*> Pool<T>::GetCurrentlyActiveObjects()//returns all the currently active projectiles
    {
        return m_ActiveObjects;
    }

}



#endif /* defined(__GameDev2D__Pool__) */
