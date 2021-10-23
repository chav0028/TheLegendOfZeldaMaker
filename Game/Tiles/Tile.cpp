//
//  Tile.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-14.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Tile.h"
#include "../SubSection.h"
#include "../Pickups/Pickup.h"
#include "../../Source/UI/UI.h"


namespace GameDev2D
{
    Tile::Tile(const string& aTileName, SubSection* aSubSection, uvec2 aCoordinates) : GameObject(aTileName),
        m_SubSection(aSubSection),
        m_Pickup(nullptr),
        m_IsWalkable(false),
        m_IsHeroSpawnPoint(false),
        m_IsEnemySpawnPoint(false),
        m_Variant(0)
    {
        //Set the local position based on the coordinates that were passed in
        SetLocalPosition((float)(aCoordinates.x * GetSize()), (float)(aCoordinates.y * GetSize()));
    }
    
    Tile::~Tile()
    {
    
    }

    void Tile::Update(double aDelta)
    {
        //Update the tile
        GameObject::Update(aDelta);
        
        //Update the pickup
        if(m_Pickup != nullptr && m_Pickup->IsEnabled() == true)
        {
            m_Pickup->Update(aDelta);
        }
        else if(m_Pickup != nullptr && m_Pickup->IsEnabled() == false)
        {
            RemovePickup();
        }
    }

    void Tile::Draw()
    {
        //Draw the tile
        GameObject::Draw();
        
        //Draw the pickup
        if(m_Pickup != nullptr && m_Pickup->IsEnabled() == true)
        {
            m_Pickup->Draw();
        }
    }
    
    unsigned int Tile::GetVariantCount()
    {
        return 2;
    }
    
    unsigned int Tile::GetVariant()
    {
        return m_Variant;
    }
    
    void Tile::SetVariant(unsigned int aVariant)
    {
        //Set the variant
        m_Variant = aVariant;
        
        //Update the sprite based on the variant
        UpdateSprite(m_Variant);
    }
    
    unsigned int Tile::GetSize()
    {
        if(m_SubSection != nullptr)
        {
            return m_SubSection->GetTileSize();
        }
        return 0;
    }
    
    vec2 Tile::GetCenter(bool aWorldPosition)
    {
        if(aWorldPosition == true)
        {
            return vec2(GetWorldX() + (GetSize() / 2), GetWorldY() + (GetSize() / 2));
        }
        
        return vec2(GetLocalX() + (GetSize() / 2), GetLocalY() + (GetSize() / 2));
    }
    
    uvec2 Tile::GetCoordinates()
    {
        if(m_SubSection != nullptr)
        {
            return m_SubSection->GetTileCoordinatesForTile(this);
        }
        return uvec2(0,0);
    }
    
    SubSection* Tile::GetSubSection()
    {
        return m_SubSection;
    }
    
    void Tile::AddPickup(Pickup* aPickup)
    {
        if(m_Pickup == nullptr)
        {
            m_Pickup = aPickup;
            m_Pickup->SetLocalPosition(GetCenter(true));
            m_Pickup->SetTile(this);
        }
        else
        {
            SafeDelete(aPickup);
        }
    }
    
    void Tile::RemovePickup()
    {
        if(m_Pickup != nullptr)
        {
            SafeDelete(m_Pickup);
        }
    }
    
    Pickup* Tile::GetPickup()
    {
        return m_Pickup;
    }
    
    bool Tile::IsWalkable()
    {
        return m_IsWalkable;
    }

    void Tile::SetIsWalkable(bool aIsWalkable)
    {
        m_IsWalkable = aIsWalkable;
    }

    bool Tile::IsHeroSpawnPoint()
    {
        return m_IsHeroSpawnPoint;
    }
    
    void Tile::SetIsHeroSpawnPoint(bool aIsHeroSpawnPoint)
    {
        m_IsHeroSpawnPoint = aIsHeroSpawnPoint;
    }

    bool Tile::IsEnemySpawnPoint()
    {
        return m_IsEnemySpawnPoint;
    }
    
    void Tile::SetIsEnemySpawnPoint(bool aIsEnemySpawnPoint)
    {
        m_IsEnemySpawnPoint = aIsEnemySpawnPoint;
    }
    
    void Tile::UpdateSprite(unsigned int aVariant)
    {
        //Safety check the variant index
        if(aVariant > GetVariantCount())
        {
            return;
        }
    
        //Cycle through and remove all the children from the tile
        for (unsigned int i = 0; i < GetNumberOfChildren(); i++)
        {
            RemoveChild(GetChildAtIndex(i));
        }
        
        //Get the atlas key for the sprite
        string atlasKey;
        GetAtlasKeyForVariant(aVariant, atlasKey);

        //Add the Sprite object as a child
        AddChild(new Sprite("MainAtlas", atlasKey), true);
    }
}
