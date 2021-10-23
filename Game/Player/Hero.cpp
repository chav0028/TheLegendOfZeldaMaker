//
//  Hero.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-14.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

/*
ChangeLog
_______________________________________________
-Overwrite the function HasChangedTiles in Hero.cpp.
-Modified Hero.cpp Update method by organizing most of its code
into 2 new fucntions , HeroAttackCollision and HeroMovementCollision.
-Modified hero.cpp  ApplyDamage and Update methods so that the low health sound is correctly played and stopped.
-Modified World.cpp so that the background music has a lower volume.
-Added the ability for the hero to throw a Projectile when he has full health.
-Changed HasChangedTiles function in Hero.cpp to account for the BlueRupeePickup and GreenRupeePickup.
-Changed the AddRupees function in Hero.cpp to check that the user doesn't go over the maximum number of rupees.
-Changed Hero.cpp attacking state to fire a Projectile.
-Made in Hero.cpp the function FireProjectile.
-Made in Hero.cpp the function HeroProjectileCollision.
-Made Projectile.cpp spin the sprite according the direction in which it was fired.
-Made class HeroProjectile to have a different sprite.
-Modified Hero.cpp , HasChangedTiles so that the pickup sounds are played.
-Removed Hero.cpp as event listener. In Game.cpp , the HandleEvent now calls the hero handle event function, if the game is not paused.
-In the update method the alpha of the hero now flickers when he is invicible, and stays solid when he is not invincible.
-Implemented stop music, and ensure that on dead all music is stopped.
-A sound is played when the hero is hurt
*/

#include "Hero.h"
#include "Enemy.h"
#include "../World.h"
#include "../SubSection.h"
#include "../Tiles/Tile.h"
#include "../Pickups/Pickup.h"
#include "../Pickups/HeartPickup.h"
#include "../HeroProjectile.h"
#include "../../Source/UI/UI.h"
#include "../../Source/Events/Input/InputEvents.h"
#include "../../Source/Services/ServiceLocator.h"
#include "../../Source/Audio/Audio.h"
#include "Enemy.h"
#include "../Menus/MainMenu.h"


namespace GameDev2D
{
    Hero::Hero(World* aWorld, Tile* aSpawnTile) : Player("Hero", aWorld),
        m_HeroState(HeroUnknown),
        m_Rupees(0),
        m_HasSword(HERO_HAS_SWORD_BY_DEFAULT),
        m_InvincibilityTimer(nullptr),
        m_SwordAttackSound(nullptr),
        m_LowHealthSound(nullptr),
        m_HeartPickupSound(nullptr),
        m_RupeePickupSound(nullptr),
		m_HeroHurtSound(nullptr),
        m_HeroProjectile(nullptr)

    {
        //Set the local position of the hero, based on the center of the spawn tile
        SetLocalPosition(aSpawnTile->GetCenter(true));

        //Cycle through and create the walking and attacking sprites for all 4 directions
        for (unsigned int i = 0; i < PLAYER_DIRECTION_COUNT; i++)
        {
            //Create the walking sprite for the current direction
            m_WalkingSprite[i] = new Sprite("MainAtlas", HERO_WALKING_ATLAS_KEY_1[i]);
            m_WalkingSprite[i]->AddFrame("MainAtlas", HERO_WALKING_ATLAS_KEY_2[i]);
            m_WalkingSprite[i]->SetAnchorPoint(0.5f, 0.5f);
            m_WalkingSprite[i]->SetDoesLoop(true);
            m_WalkingSprite[i]->SetIsEnabled(false);
            AddChild(m_WalkingSprite[i], false);

            //Create the attacking sprite for the current direction
            m_AttackingSprite[i] = new Sprite("MainAtlas", HERO_ATTACKING_ATLAS_KEY[i]);
            m_AttackingSprite[i]->SetAnchorPoint(HERO_ATTACKING_ANCHOR_POINT[i]);
            m_AttackingSprite[i]->SetIsEnabled(false);
            AddChild(m_AttackingSprite[i], false);
        }

        //Add an event listener for keyboard input
        //ServiceLocator::GetInputManager()->AddEventListener(this, KEYBOARD_EVENT);

        //Create the invincibility timer
        m_InvincibilityTimer = new Timer(HERO_INVINCIBILITY_DURATION);

        //Create the audio sound effects
        m_SwordAttackSound = new Audio("Sword", "wav", false, false);

        //Create the low health sound effect
        m_LowHealthSound = new Audio("LowHealth", "wav", false, true);

		//Create hero hurt sound
		m_HeroHurtSound = new Audio("HeroHurt", "wav", false, false);
		m_HeroHurtSound->SetVolume(1.2);

        //Create pick up sound effects
        m_HeartPickupSound = new Audio("HeartPickup", "wav", false, false);
        m_RupeePickupSound = new Audio("RupeePickup", "wav", false, false);

        //Create projectile
        m_HeroProjectile = new HeroProjectile();

        //Set the hero's initial health and health capacity
        SetHealthCapacity(HERO_INITIAL_HEALTH_CAPACITY);
        SetHealth(HERO_INITIAL_HEALTH);

        //Set the hero's state to idle
        SetState(HeroIdle);
    }

    Hero::~Hero()
    {
        //Remove the event listener
        ServiceLocator::GetInputManager()->RemoveEventListener(this, KEYBOARD_EVENT);

        //Cycle through and delete the hero Sprites
        for (unsigned int i = 0; i < PLAYER_DIRECTION_COUNT; i++)
        {
            SafeDelete(m_WalkingSprite[i]);
            SafeDelete(m_AttackingSprite[i]);
        }

        //Delete the timer object
        SafeDelete(m_InvincibilityTimer);

        //Delete sound effect audio objects
        SafeDelete(m_SwordAttackSound);
        SafeDelete(m_LowHealthSound);
        SafeDelete(m_HeartPickupSound);
        SafeDelete(m_RupeePickupSound);
		SafeDelete(m_HeroHurtSound);

        //Delete projectile
        SafeDelete(m_HeroProjectile);
    }

    void Hero::Update(double aDelta)
    {
        //Update the invicibility timer
        m_InvincibilityTimer->Update(aDelta);

        //Check collision against enviroment
        HeroMovementCollision(aDelta);

        //Check the collision for when the player is attacking
        HeroAttackCollision(aDelta);

        //Check if the low health sound is going to stop
        //If the user no longer has low health or if he is dead
        if (GetHealth() > HERO_LOW_HEALTH_SOUND_TRIGGER || m_HeroState==HeroDead)
        {
            //If we have sound
            if (m_LowHealthSound != nullptr)
            {
                //If the sound is playing
                if (m_LowHealthSound->IsPlaying() == true)
                {
                    m_LowHealthSound->Stop();//Stop the sound
                }
            }
        }

        //Player flickering
        if (IsInvincible() == true)
        {
            //flicker between the 2 active alpha values
            if (m_ActiveSprite->GetAlpha() <= HERO_FLICKERING_MIN_ALPHA)
            {
                m_ActiveSprite->SetAlpha(HERO_FLICKER_MAX_ALPHA);
            }
            else
            {
                m_ActiveSprite->SetAlpha(HERO_FLICKERING_MIN_ALPHA);
            }
        }
        else
        {
            m_ActiveSprite->SetAlpha(1);
        }


        //Update the base class
        Player::Update(aDelta);
    }

    void Hero::Draw()
    {
        Player::Draw();

        //Draw the projectile
        if (m_HeroProjectile != nullptr)//if the projectile is valid
        {
            if (m_HeroProjectile->IsEnabled() == true)//if the projectile is enabled
            {
                m_HeroProjectile->Draw();//Draw the projectile
            }
        }
    }

    void Hero::Reset()
    {
        //Set the hero's state to idle
        SetState(HeroIdle);
		m_InvincibilityTimer->Reset(false);//Reset the invicibility timer
		StopAllSound();//Stop all the sounds

        //Reset the base class
        Player::Reset();
    }

    void Hero::SetState(unsigned int aState)
    {
        //Set the hero state
        m_HeroState = (HeroState)aState;

        //Disable the active Sprite
        if (m_ActiveSprite != nullptr)
        {
            m_ActiveSprite->SetIsEnabled(false);
        }

        //Handle the hero state switch
        switch (aState)
        {
        case HeroIdle:
        case HeroWalking:
        {
            //Set the active Sprite based on the direction
            m_ActiveSprite = m_WalkingSprite[m_DirectionIndex];

            //Set the player speed
            float playerSpeed = aState == HeroIdle ? 0.0f : HERO_WALKING_SPEED;
            SetSpeed(playerSpeed);

            //Set the frame speed
            float frameSpeed = aState == HeroIdle ? 0.0f : HERO_WALKING_ANIMATION_FRAME_SPEED;
            m_ActiveSprite->SetFrameSpeed(frameSpeed);
        }
        break;

        case HeroAttacking:
        {
            //Set the active Sprite based on the direction
            m_ActiveSprite = m_AttackingSprite[m_DirectionIndex];

            //Set the player speed
            SetSpeed(0.0f);

            FireProjectile();//Fire the hero projectile

            //Plat the sword attacking sound effect
            m_SwordAttackSound->Play();

            //Stop attacking after a delay
            DelayGameObjectMethod(&Hero::StopAttacking, HERO_ATTACK_TIME);
        }
        break;

        case HeroDead:
        {
			StopAllSound();//Ensure all the sounds and stats are properly stopped before quitting
            GoToMainMenu();//Go back to main menu	
			if (m_World != nullptr)//Stop the game music
			{
				m_World->StopMusic();
			}
        }
        break;

        default:
            break;
        }

        //Reset the frame index to zero and enable it
        m_ActiveSprite->SetFrameIndex(0);
        m_ActiveSprite->SetIsEnabled(true);
    }

    bool Hero::HasSword()
    {
        return m_HasSword;
    }

    void Hero::PickedUpSword()
    {
        if (m_HasSword == false)
        {
            //Set the hasSword flag
            m_HasSword = true;
        }
    }

    void Hero::AddRupees(unsigned short aRupees)
    {
        //Safety check the rupees bounds
        if (m_Rupees + aRupees >= USHRT_MAX)
        {
            m_Rupees = USHRT_MAX;
        }
        else
        {
            m_Rupees += aRupees;

            //If the hero has more rupees than the max
            if (m_Rupees > HERO_MAX_RUPEES)
            {
                //Set the number of rupees the hero has to the max, 
                //this is done after adding the rupees to the count to consider for blue rupees, since the user may be missing less than 5 rupees when he picks a blue rupee
                m_Rupees = HERO_MAX_RUPEES;
            }
        }
    }

    unsigned short Hero::GetRupees()
    {
        return m_Rupees;
    }

    bool Hero::IsInvincible()
    {
        return m_InvincibilityTimer->IsRunning();
    }

    void Hero::ApplyDamage(unsigned int aAttackDamage)
    {
        Player::ApplyDamage(aAttackDamage);

		if (m_HeroHurtSound != nullptr)
		{
			if (IsInvincible() == false) //if the hero is not invincible
			{
				m_HeroHurtSound->Play();//Play the hero hurt sound
			}
		}

        //If there is still health left, reset the invincibility timer
        if (GetHealth() > 0)
        {
            m_InvincibilityTimer->Reset(true);

        }

        //If the hero has low health and is still alive
        if (GetHealth() <= HERO_LOW_HEALTH_SOUND_TRIGGER && m_HeroState!=HeroDead)
        {
            if (m_LowHealthSound != nullptr)//If there is a sound
            {
                m_LowHealthSound->Play();//play the sound
            }
        }
    }

    void Hero::HasDied()
    {
        SetState(HeroDead);//Set the that hero state as dead
        Player::HasDied();
    }

    void Hero::HandleEvent(Event* aEvent)
    {
        if (IsEnabled() == true)
        {
            switch (aEvent->GetEventCode())
            {
            case KEYBOARD_EVENT:
            {
                KeyEvent* keyEvent = (KeyEvent*)aEvent;
                KeyEventType keyEventType = keyEvent->GetKeyEventType();

                //Handle key down and up events
                if (keyEventType == KeyDown)
                {
                    HandleKeyDown(keyEvent->GetKeyCode());
                }
                else if (keyEventType == KeyUp)
                {
                    HandleKeyUp(keyEvent->GetKeyCode());
                }
            }
            break;

            default:
                break;
            }
        }
    }

    void Hero::HandleKeyDown(KeyCode aKeyCode)
    {
        switch (aKeyCode)
        {
        case KEY_CODE_UP:
            SetDirection(PLAYER_DIRECTION_UP);
            SetState(HeroWalking);
            break;

        case KEY_CODE_DOWN:
            SetDirection(PLAYER_DIRECTION_DOWN);
            SetState(HeroWalking);
            break;

        case KEY_CODE_LEFT:
            SetDirection(PLAYER_DIRECTION_LEFT);
            SetState(HeroWalking);
            break;

        case KEY_CODE_RIGHT:
            SetDirection(PLAYER_DIRECTION_RIGHT);
            SetState(HeroWalking);
            break;

        case KEY_CODE_SPACE:
            if (HasSword() == true)
            {
                SetState(HeroAttacking);
            }
            break;

        default:
            break;
        }
    }

    void Hero::HandleKeyUp(KeyCode aKeyCode)
    {
        //Handle scenarios where one (or more) of the arrow keys are still down
        bool isUpDown = ServiceLocator::GetInputManager()->IsKeyPressed(KEY_CODE_UP);
        bool isDownDown = ServiceLocator::GetInputManager()->IsKeyPressed(KEY_CODE_DOWN);
        bool isLeftDown = ServiceLocator::GetInputManager()->IsKeyPressed(KEY_CODE_LEFT);
        bool isRightDown = ServiceLocator::GetInputManager()->IsKeyPressed(KEY_CODE_RIGHT);

        //Handle the key up event
        switch (aKeyCode)
        {
        case KEY_CODE_UP:
            if (isDownDown == true)
            {
                SetDirection(PLAYER_DIRECTION_DOWN);
                SetState(HeroWalking);
            }
            else if (isLeftDown == true)
            {
                SetDirection(PLAYER_DIRECTION_LEFT);
                SetState(HeroWalking);
            }
            else if (isRightDown == true)
            {
                SetDirection(PLAYER_DIRECTION_RIGHT);
                SetState(HeroWalking);
            }
            else
            {
                SetState(HeroIdle);
            }
            break;

        case KEY_CODE_DOWN:
            if (isUpDown == true)
            {
                SetDirection(PLAYER_DIRECTION_UP);
                SetState(HeroWalking);
            }
            else if (isLeftDown == true)
            {
                SetDirection(PLAYER_DIRECTION_LEFT);
                SetState(HeroWalking);
            }
            else if (isRightDown == true)
            {
                SetDirection(PLAYER_DIRECTION_RIGHT);
                SetState(HeroWalking);
            }
            else
            {
                SetState(HeroIdle);
            }
            break;

        case KEY_CODE_LEFT:
            if (isRightDown == true)
            {
                SetDirection(PLAYER_DIRECTION_RIGHT);
                SetState(HeroWalking);
            }
            else if (isUpDown == true)
            {
                SetDirection(PLAYER_DIRECTION_UP);
                SetState(HeroWalking);
            }
            else if (isDownDown == true)
            {
                SetDirection(PLAYER_DIRECTION_DOWN);
                SetState(HeroWalking);
            }
            else
            {
                SetState(HeroIdle);
            }
            break;

        case KEY_CODE_RIGHT:
            if (isLeftDown == true)
            {
                SetDirection(PLAYER_DIRECTION_LEFT);
                SetState(HeroWalking);
            }
            else if (isUpDown == true)
            {
                SetDirection(PLAYER_DIRECTION_UP);
                SetState(HeroWalking);
            }
            else if (isDownDown == true)
            {
                SetDirection(PLAYER_DIRECTION_DOWN);
                SetState(HeroWalking);
            }
            else
            {
                SetState(HeroIdle);
            }
            break;

        default:
            break;
        }
    }

    void Hero::HasChangedTiles(Tile* aNewTile, Tile* aOldTile)
    {
        //Check if there is a pick up in the new tile
        if (aNewTile->GetPickup() != nullptr)
        {
            //Check which type of pick
            PickupType typeOfPickUp = aNewTile->GetPickup()->GetPickupType();

            //Implement effects according to the pick up
            switch (typeOfPickUp)
            {

                //Heart pick up
            case PickupTypeHeart:
                m_HeartPickupSound->Play();//Play sound effect				
                SetHealth(GetHealth() + HERO_FULL_HEART);//Increase health by 1 heart
                break;

                //Green Rupee pick up
            case PickupTypeGreenRupee:
                m_RupeePickupSound->Play();//Play sound effect	
                AddRupees(HERO_GREEN_RUPEE_VALUE);//Add the value of the green rupee to the hero rupee count
                break;

                //Blue Rupee pick up
            case PickupTypeBlueRupee:
                m_RupeePickupSound->Play();//Play sound effect	
                AddRupees(HERO_BLUE_RUPEE_VALUE);//Add the value of the blue rupee to the hero rupee count
                break;

            default:
                break;
            }

            //Remove pick up from tile
            aNewTile->RemovePickup();
        }
    }

    void Hero::StopAttacking()
    {
        //Handle scenarios where one (or more) of the arrow keys are still down
        bool isUpDown = ServiceLocator::GetInputManager()->IsKeyPressed(KEY_CODE_UP);
        bool isDownDown = ServiceLocator::GetInputManager()->IsKeyPressed(KEY_CODE_DOWN);
        bool isLeftDown = ServiceLocator::GetInputManager()->IsKeyPressed(KEY_CODE_LEFT);
        bool isRightDown = ServiceLocator::GetInputManager()->IsKeyPressed(KEY_CODE_RIGHT);

        //Handle the case where a key might be still held down
        if (isUpDown == true)
        {
            SetDirection(PLAYER_DIRECTION_UP);
            SetState(HeroWalking);
        }
        else if (isDownDown == true)
        {
            SetDirection(PLAYER_DIRECTION_DOWN);
            SetState(HeroWalking);
        }
        else if (isLeftDown == true)
        {
            SetDirection(PLAYER_DIRECTION_LEFT);
            SetState(HeroWalking);
        }
        else if (isRightDown == true)
        {
            SetDirection(PLAYER_DIRECTION_RIGHT);
            SetState(HeroWalking);
        }
        else
        {
            SetState(HeroIdle);
        }
    }

    void Hero::FireProjectile()
    {
        if (GetHealth() >= GetHealthCapacity())//If the player has full health
        {
            if (m_HeroProjectile != nullptr)//if the projectile is valid
            {
                if (m_HeroProjectile->IsEnabled() == false)//Only fire the projectile if it currently inactive
                {
                    m_HeroProjectile->Fire(GetSubSection(), GetLocalPosition(), GetDirection(), HERO_PROJECTILE_DEFAULT_SPEED, HERO_PROJECTILE_DEFAULT_ATTACK_DAMAGE);//Fire the projectile
                }
            }
        }
    }

    //Handles the player collision against other tiles
    void Hero::HeroMovementCollision(double aDelta)
    {
        //Cache the previous tile and location of the player
        Tile* previousTile = GetTile();
        vec2 previousPosition = GetLocalPosition();

        //Calculate and set the new player's position
        SetLocalPosition(vec2(GetLocalX() + GetDirection().x * GetSpeed() * aDelta, GetLocalY() + GetDirection().y * GetSpeed() * aDelta));

        //Factor in the collision zone for the player's position, based on the direction the player is walking
        vec2 position = GetWorldPosition() + vec2(GetDirection() * vec2(HERO_COLLISION_ZONE, HERO_COLLISION_ZONE));

        //Determine the subsection the position is on
        SubSection* subSection = m_World->GetSubSectionForPosition(position);

        //Calculate the perpendicular angles for the player collision zone
        vec2 v = GetWorldPosition() - position;
        vec2 perpendicularA = vec2(-v.y, v.x) / sqrtf((v.x * v.x) + (v.y * v.y)) * HERO_COLLISION_ZONE;
        vec2 perpendicularB = vec2(-v.y, v.x) / sqrtf((v.x * v.x) + (v.y * v.y)) * -HERO_COLLISION_ZONE;

        //Factor in the sub-section the player is on
        position -= subSection->GetWorldPosition();

        //Create a vector for the 3 collision points to check against
        vector<vec2> collisionPoints = { position, vec2(position + perpendicularA), vec2(position + perpendicularB) };

        //Cycle through the collision points
        for (unsigned int i = 0; i < collisionPoints.size(); i++)
        {
            //Get the current tile for the collision point
            Tile* currentTile = subSection->GetTileForPosition(collisionPoints[i]);

            //If the current tile doesn't equal the previous tile, check to see if the current tile is walkable
            if (currentTile != previousTile)
            {
                //If its NOT walkable set the player's position back to the previous position
                if (currentTile->IsWalkable() == false)
                {
                    SetLocalPosition(previousPosition);
                    break;
                }
            }
        }

        //Check to see if the player has changed tiles since the start of the update method
        Tile* currentTile = GetTile();
        if (currentTile != previousTile)
        {
            HasChangedTiles(currentTile, previousTile);
        }
    }

    //Handles the collision of the player attacks
    void Hero::HeroAttackCollision(double aDelta)
    {
        //If the hero is attacking we need to see if the hero's sword has collided with any of the enemies
        if (m_HeroState == HeroAttacking)
        {
            //Get the subsection
            SubSection* subSection = m_World->GetSubSectionForPosition(GetLocalPosition());

            //Get the current subsection the hero is on
            subSection = GetSubSection();

            //Factor in the collision zone for the player's position, based on the direction the player is walking
            vec2 swordTipPosition = GetWorldPosition() + vec2(GetDirection() * HERO_SWORD_RANGE);

            //Factor in the sub-section the player is on
            swordTipPosition -= subSection->GetWorldPosition();

            //Get the tile the sword is on
            Tile* swordTile = subSection->GetTileForPosition(swordTipPosition);

            //Get the active enemies in the subsection
            vector<Enemy*> enemies;
            GetSubSection()->GetActiveEnemies(enemies);

            //Cycle through the active enemies and see if the tip of the sword collided with any of them
            for (unsigned int i = 0; i < enemies.size(); i++)
            {
                if (enemies.at(i)->GetTile() == swordTile)
                {
                    enemies.at(i)->ApplyDamage(GetAttackDamage());
                }
            }
        }

        HeroProjectileCollision(aDelta);//Check the collision against the projectiles
    }

    void Hero::HeroProjectileCollision(double aDelta)
    {
        //If the hero has a projectile
        if (m_HeroProjectile != nullptr)
        {
            if (m_HeroProjectile->IsEnabled() == true)//iF the projectile is enabled, has been fired
            {
                m_HeroProjectile->Update(aDelta);//Update the projectile

                //Get the enemyes active in the subsection
                vector<Enemy*> activeEnemies;
                GetSubSection()->GetActiveEnemies(activeEnemies);

                //Handle the projectile collision against the enemies
                for (unsigned int i = 0; i < activeEnemies.size(); i++)//Go through all the active enemies in the subsection
                {
                    if (activeEnemies.at(i) != nullptr)//If it is a valid enemy
                    {
                        if (activeEnemies.at(i)->GetTile() == m_HeroProjectile->GetProjectileTile())//Check if the enemy is in the same tile as the projectile 
                        {
                            activeEnemies.at(i)->ApplyDamage(m_HeroProjectile->GetAttackDamage());//Damage the enemy
                            m_HeroProjectile->DeactivateProjectile();//Deactivate the enemy projectile
                            break;//Break out of loop so that the damage is only applied to one enemy in the tile
                        }
                    }
                }
            }
        }
    }

    //Function to call with delay when the user has died
    void Hero::GoToMainMenu()
    {
        ServiceLocator::GetSceneManager()->PushScene(new MainMenu(), true);//Go back to main menu
    }

	//Stop all the sounds in the hero
	void Hero::StopAllSound()
	{
		if (m_SwordAttackSound!=nullptr)
		{
			m_SwordAttackSound->Stop();
		}

		if (m_LowHealthSound != nullptr)
		{
			m_LowHealthSound->Stop();
		}

		if (m_HeartPickupSound != nullptr)
		{
			m_HeartPickupSound->Stop();
		}

		if (m_RupeePickupSound != nullptr)
		{
			m_RupeePickupSound->Stop();
		}

		if (m_HeroHurtSound != nullptr)
		{
			m_HeroHurtSound->Stop();
		}
	}
}