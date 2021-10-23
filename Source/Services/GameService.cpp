//
//  GameService.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-10.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//


#include "GameService.h"

namespace GameDev2D
{
    GameService::GameService(const string& aServiceName) : BaseObject(aServiceName), EventDispatcher(),
      m_ServiceLocatorResponsibleForDeletion(false)
    {
      
    }

    GameService::~GameService()
    {

    }

    void GameService::Update(double delta)
    {
        
    }
    
    void GameService::Draw()
    {
    
    }

    void GameService::SetServiceLocatorResponsibleForDeletion(bool aResponsibleForDeletion)
    {
      m_ServiceLocatorResponsibleForDeletion = aResponsibleForDeletion;
    }

    bool GameService::IsServiceLocatorResponsibleForDeletion()
    {
      return m_ServiceLocatorResponsibleForDeletion;
    }
}