/*******************************************************************
Projectile.cpp

Student:        Alvaro Chavez Mixco
Algonquin College
Date:           December 12, 2015
Course Number:  GAM1540
Professor:      David McCue
Purpose:
The projectile class is a class that handles all the basic properties  a
projectile has. Mostly involving the stats it will have once it is activated.
The class also handles deactivating the projectile (settind enabled as false
and if it belongs to a pool telling it to the pool ) when it goes off the Hero
subsection or when it collides with a nonwalkable tile.
*********************************************************************/

#include "Projectile.h"
#include "../../Source/UI/Sprite/Sprite.h"
#include "SubSection.h"
#include "Tiles/Tile.h"

namespace GameDev2D
{
    Projectile::Projectile(bool aUseDefaultSprite)
        : GameObject("Projectile"),
        m_PoolOwner(nullptr),
        m_SubSection(nullptr),
        m_Sprite(nullptr)
    {
        if (aUseDefaultSprite == true)
        {
            LoadSprite();
        }

        Reset();
    }

    Projectile::Projectile(Pool<Projectile>* aPool)
        : GameObject("PoolProjectile"),
        m_SubSection(nullptr),
        m_PoolOwner(aPool),
        m_Sprite(nullptr)
    {
        LoadSprite();
        Reset();
    }

    void Projectile::LoadSprite()
    {
        // create sprite
        m_Sprite = new Sprite("MainAtlas", "EnemyProjectile");
        m_Sprite->SetAnchorPoint(0.5, 0.5);
        AddChild(m_Sprite, false);
    }

    Projectile::~Projectile()
    {
        SafeDelete(m_Sprite);
    }

    void Projectile::Reset()
    {
        DeactivateProjectile();
    }

    void Projectile::Update(double aDelta)
    {
        if (IsEnabled() == true)//If the object is enabled
        {
            vec2 projectilePos = GetLocalPosition();

            //Calculate the movement
            projectilePos.x += m_Direction.x*(m_Speed*(float)aDelta);
            projectilePos.y += m_Direction.y*(m_Speed*(float)aDelta);

            //Move the position
            SetLocalPosition(projectilePos);

            if (m_SubSection != nullptr)
            {
                //Check that it doesn't go through non walkable tiles
                Tile* currentTile = GetProjectileTile();//Get the tile the projectile is on

                if (currentTile != nullptr)
                {
                    if (currentTile->IsWalkable() == false)//if the tile is not walkable
                    {
                        DeactivateProjectile();//Deactivate the projectile
                    }

                    unsigned int tileSize = m_SubSection->GetTileSize();
                    vec2 projectileLocalPos = GetWorldPosition() - m_SubSection->GetWorldPosition();
                    //Bound check the subsection, if the projectile goes offscreen disable the projectile                 
                    if (projectileLocalPos.x< 0.0f ||//Check left side
                        projectileLocalPos.y < 0.0f ||//Check bottom side
                        projectileLocalPos.y >((tileSize*m_SubSection->GetNumberOfVerticalTiles()) - tileSize) ||//Check top side, the -tile size is to avoid a bug in engine where it crashes since the projectile goes too fast
                        projectileLocalPos.x>(tileSize*m_SubSection->GetNumberOfHorizontalTiles()) //Check left side
                        )
                    {
                        DeactivateProjectile();//Deactivate the projectile
                    }
                }
            }

        }
    }

    void Projectile::Draw()
    {
        GameObject::Draw();

        //Draw the sprite
        if (m_Sprite != nullptr)
        {
            if (m_Sprite->IsEnabled() == true)
            {
                m_Sprite->Draw();
            }
        }
    }

    void Projectile::Fire(SubSection* aSubsection, vec2 aPosition, vec2 aDirection, float aSpeed, int aAttackDamage)
    {
        //Rotate the sprite
        if (m_Sprite != nullptr)//If the children is a sprite
        {
            m_Sprite->SetIsEnabled(true);//Enable the sprite

            //If the projectile is going up
            if (aDirection == vec2(0, 1))
            {
                m_Sprite->SetLocalAngle(0);
            }
            else if (aDirection == vec2(0, -1))//If the projectile is going down
            {
                m_Sprite->SetLocalAngle(-180);
            }
            else if (aDirection == vec2(1, 0))//If the projectile is going right
            {
                m_Sprite->SetLocalAngle(-90);
            }
            else if (aDirection == vec2(-1, 0)) //If the projectile is going left
            {
                m_Sprite->SetLocalAngle(90);
            }
        }

        //The projectile is activated in the pool before, we fire it. This is done in the code of whomever is
        //shooting the projectile
        //Check that it is a valid subsection
        if (aSubsection != nullptr)
        {
            m_SubSection = aSubsection;
        }

        //set stats
        SetLocalPosition(aPosition);
        m_Direction = aDirection;
        m_Speed = aSpeed;
        m_AttackDamage = aAttackDamage;

        //Enable projectile
        SetIsEnabled(true);
    }

    //Disables the projectile, and if it belongs to a pool, it tells the pool to remove it
    void Projectile::DeactivateProjectile()
    {
        //Disable the projectile
        SetIsEnabled(false);

        //Diable the sprite
        if (m_Sprite != nullptr)//If the children is a sprite
        {
            m_Sprite->SetIsEnabled(false);
        }

        if (m_PoolOwner != nullptr)//If the object is in a pool
        {
            //Tell the pool to move the projectile to the inactive list
            m_PoolOwner->DeactivateObject(this);
        }
    }

    Tile* Projectile::GetProjectileTile()
    {
        if (m_SubSection != nullptr)
        {
            //Get the position of the player relative to the subsection
            vec2 position = GetWorldPosition() - m_SubSection->GetWorldPosition();

            //Get the tile for the player's relative position
            Tile* currentTile = m_SubSection->GetTileForPosition(position);

            if (currentTile != nullptr)
            {
                return currentTile;
            }
        }

        return nullptr;
    }

    int Projectile::GetAttackDamage()
    {
        return m_AttackDamage;
    }
}