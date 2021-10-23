//
//  GameService.h
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-10.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__GameService__
#define __GameDev2D__GameService__

#include "FrameworkConfig.h"
#include "../Core/BaseObject.h"
#include "../Events/EventDispatcher.h"


namespace GameDev2D
{
    //The GameService class is an abstract class, used as an interface for services that would be
    //used in game, they are managed by the ServiceLocator
    class GameService : public BaseObject, public EventDispatcher
    {
    public:
        //Constructs a GameService, takes in the name of the service as it parameter
        GameService(const string& serviceName);
        virtual ~GameService();
        
        //If the CanUpdate() below returns true then the Update() method will be call to
        //update the service
        virtual void Update(double delta);
        
        //Abstract method used to determine if the inheriting GameService can be drawn
        virtual bool CanUpdate() = 0;
        
        //If the CanDraw() below returns true then the Draw() method will be call to
        //draw the service
        virtual void Draw();
        
        //Abstract method used to determine if the inheriting GameService can be drawn
        virtual bool CanDraw() = 0;
        
    protected:
        //Used to keep track if the ServiceLocator is responsible for deleting the
        //GameService from memory
        void SetServiceLocatorResponsibleForDeletion(bool responsibleForDeletion);
        bool IsServiceLocatorResponsibleForDeletion();
        
        //The ServiceLocator needs to access the protected members
        friend class ServiceLocator;
        
    private:
        //Member variable
        bool m_ServiceLocatorResponsibleForDeletion;
    };
}

#endif /* defined(__GameDev2D__GameService__) */
