//
//  Enemy.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

/*
ChangeLog
_______________________________________________
-Implemented the A* pathfinding algorithm in Enemy. Made the functions StatWalking, Walk , and ChangeState. This according to video.
-Implemented enemy projectiles.
-Changed Enemy.cpp to fire projectiles, and for his projectiles to check when it collides against the player.
-Made variable in Enemy to regulate how often the enemy will shoot.
-Makes the enemy.cpp drop a HeartPickup.
-Randomized enemy behavior as shown in video in Enemy.cpp..
-Implemented Enemy Attacking state in Set Enemy State in Enemy.cpp..
-Randomized the wait time for enemy idle state in Enemy.cpp.
-Added randomized item drop according to the assignment conditions by modifying HasDied in Enemy.cpp.
-Overwrote function ApplyDamage in Enemy.cpp so that the damaged sound could be played.
-Added hit and death sounds in Enemy.cpp.
-Implemented walking state
*/

#include "Enemy.h"
#include "Hero.h"
#include "../World.h"
#include "../SubSection.h"
#include "../Tiles/Tile.h"
#include "../PathFinding/PathFinder.h"
#include "../../Source/UI/UI.h"
#include "../../Source/Animation/Random.h"
#include "../../Source/Audio/Audio.h"
#include "../Projectile.h"
#include "../Pickups/HeartPickup.h"
#include "../Pickups/GreenRupeePickup.h"
#include "../Pickups/BlueRupeePickup.h"

namespace GameDev2D
{
	Enemy::Enemy(World* aWorld, Tile* aSpawnTile) : Player("Enemy", aWorld),
		m_PathFinder(nullptr),
		m_EnemyState(EnemyUnknown),
		m_Random(nullptr),
		m_PathIndex(0),
		m_DamagedSound(nullptr),
		m_DeadSound(nullptr),
		m_ProjectilePool(ENEMY_DEFAULT_NUMBER_PROJECTILES)
	{
		//Set the local position of the hero, based on the center of the spawn tile
		SetLocalPosition(aSpawnTile->GetCenter(true));

		//Create the path finder object
		SubSection* subSection = m_World->GetSubSectionForPlayer(this);
		m_PathFinder = new PathFinder(subSection);

		//Create the random number generator object
		m_Random = new Random();
		m_Random->RandomizeSeed();

		//Cycle through and create the walking and attacking sprites for all 4 directions
		for (unsigned int i = 0; i < PLAYER_DIRECTION_COUNT; i++)
		{
			//Create the walking sprite for the current direction
			m_WalkingSprite[i] = new Sprite("MainAtlas", ENEMY_WALKING_ATLAS_KEY_1[i]);
			m_WalkingSprite[i]->AddFrame("MainAtlas", ENEMY_WALKING_ATLAS_KEY_2[i]);

			m_WalkingSprite[i]->SetAnchorPoint(0.5f, 0.5f);
			m_WalkingSprite[i]->SetDoesLoop(true);
			m_WalkingSprite[i]->SetIsEnabled(false);
			AddChild(m_WalkingSprite[i], false);
		}



		//Create sounds
		m_DamagedSound = new Audio("EnemyHit", "wav", false, false);
		m_DeadSound = new Audio("EnemyDeath", "wav", false, false);

		//Set the enemy's state to idle
		SetState(EnemyIdle);
	}

	Enemy::~Enemy()
	{
		//Delete the path finder object
		SafeDelete(m_PathFinder);

		//Delete the random number generator object
		SafeDelete(m_Random);

		//Cycle through and delete the enemy Sprites
		for (unsigned int i = 0; i < PLAYER_DIRECTION_COUNT; i++)
		{
			SafeDelete(m_WalkingSprite[i]);
		}

		//Delete sounds
		SafeDelete(m_DamagedSound);
		SafeDelete(m_DeadSound);
	}

	void Enemy::Update(double aDelta)
	{

		//Get the hero and the tile the hero is on
		Hero* hero = m_World->GetHero();
		Tile* heroTile = hero->GetTile();

		if (IsEnabled() == true)
		{
			//If the hero tile is the same tile the enemy is on, apply some damage
			if (heroTile == GetTile())
			{
				hero->ApplyDamage(GetAttackDamage());
			}

			//Cache the tile before we update the player's position
			Tile* previousTile = GetTile();

			//Update the base class
			Player::Update(aDelta);

			//Check to see if the player has changed tiles since the start of the update method
			Tile* currentTile = GetTile();
			if (currentTile != previousTile)
			{
				HasChangedTiles(currentTile, previousTile);
			}

		}

		//Update the enemy active projectiles, this is done even if the enemy is disabled
		vector<Projectile*> activeProjectiles = m_ProjectilePool.GetCurrentlyActiveObjects();
		for (unsigned int i = 0; i < activeProjectiles.size(); i++)
		{
			if (activeProjectiles.at(i) != nullptr)//If it is a valid object
			{
				activeProjectiles.at(i)->Update(aDelta);//Update it

				//Collision check against hero
				//If the hero tile is the same tile as the projectile, 
				if (heroTile == activeProjectiles.at(i)->GetProjectileTile())
				{
					hero->ApplyDamage(activeProjectiles.at(i)->GetAttackDamage()); //Apply damage
					activeProjectiles.at(i)->DeactivateProjectile(); //Deactivate the projectile
				}

			}
		}
	}

	void Enemy::Draw()
	{
		Player::Draw();//Draw the parent draw function

		//Draw the enemy active projectiles
		vector<Projectile*> activeProjectiles = m_ProjectilePool.GetCurrentlyActiveObjects();
		for (unsigned int i = 0; i < activeProjectiles.size(); i++)
		{
			if (activeProjectiles.at(i) != nullptr)//If it is a valid object
			{
				activeProjectiles.at(i)->Draw();//Draw it
			}
		}
	}


	void Enemy::DebugDraw()
	{
#if DEBUG
		m_PathFinder->DebugDraw();
#endif
	}

	void Enemy::Reset()
	{
		//Reset the enemy state
		SetState(EnemyIdle);

		//Reset the base class
		Player::Reset();
	}

	void Enemy::SetState(unsigned int aState)
	{
		//Set the enemy state
		m_EnemyState = (EnemyState)aState;

		//Log the enemy state
		Log("Enemy set state:%u", m_EnemyState);

		//Refresh the active sprite
		RefreshActiveSprite();

		//Handle the hero state switch
		switch (aState)
		{
		case EnemyIdle:
		{
			if (IsEnabled() == false)
			{
				//Ensure  the active sprite is enabled
				m_ActiveSprite->SetIsEnabled(true);

				//Ensure the enemy object is enabled
				SetIsEnabled(true);
			}

			//Generate a random idle time
			double idleDelayTime = m_Random->RandomRange(ENEMY_MIN_IDLE_TIME, ENEMY_MAX_IDLE_TIME);

			//Delay call the change state method
			DelayGameObjectMethod(&Enemy::ChangeState, idleDelayTime);
		}
		break;

		case EnemyWalking:
		{
			//Get the enemy's current subsection and the hero's
			SubSection* enemySubsection = GetSubSection();//the subsection of this enemy

			if (enemySubsection != nullptr)
			{
				Tile* destinationTile = nullptr;
				bool destinationTileWalkable = false;//Variable to check if the random tile chosen is walkable
				bool pathFound = false;

				//Search randomly for a tile until it fins a walkable one
				do
				{
					unsigned int destinationTileIndex = m_Random->RandomRange(0, enemySubsection->GetNumberOfTiles());//Get the index for a random tile in the subsection
					destinationTile = enemySubsection->GetTileForIndex(destinationTileIndex);//Get the tile according to the subsection

					destinationTileWalkable = destinationTile->IsWalkable();//Check if the tile is walkable
				} while (destinationTileWalkable == false);


				if (destinationTile != nullptr)
				{
					pathFound = m_PathFinder->FindPath(GetTile(), destinationTile);//Check for a path
				}

				//Check if a path was found
				if (pathFound == true)
				{
					StartWalking();//Start moving to destination
				}
				else
				{
					SetState(EnemyIdle);//A path can't be found, set enemy as idle
				}
			}
			else//If the hero isn't in the same subsection as the enemy
			{
				SetState(EnemyIdle);
			}
		}
		break;

		case EnemySearching:
		{
			//Get the enemy's current subsection and the hero's
			SubSection* enemySubsection = GetSubSection();//the subsection of this enemy
			SubSection* heroSubSection = m_World->GetHero()->GetSubSection();

			//Ensure the hero is on the same subsection as our enemy
			if (enemySubsection == heroSubSection)
			{
				//find the path to the hero
				Tile* destination = m_World->GetHero()->GetTile();//Set the destination to be the tile where the hero is
				bool pathFound = m_PathFinder->FindPath(GetTile(), destination);//Check for a path

				//Check if a path was found
				if (pathFound == true)
				{
					StartWalking();//Start moving to destination
				}
				else
				{
					SetState(EnemyIdle);//A path can't be found, set enemy as idle
				}
			}
			else//If the hero isn't in the same subsection as the enemy
			{
				SetState(EnemyIdle);
			}
		}
		break;

		case EnemyAttacking:
		{
			//Fire a projectile with delay, this method will also take care of later setting the state back to idle
			DelayGameObjectMethod(&Enemy::FireProjectile, ENEMY_PROJECTILE_DELAY);
		}
		break;

		case EnemyDead:
		{
			//Disable the active sprite
			m_ActiveSprite->SetIsEnabled(false);

			//Disable the enemy object
			SetIsEnabled(false);
		}
		break;

		default:
			break;
		}
	}

	void Enemy::HasDied()
	{
		//Play a sound effect
		if (m_DeadSound != nullptr)
		{
			m_DeadSound->Play();
		}

		//Set the enemy state to dead
		SetState(EnemyDead);

		//Randomly drop a item pick up
		unsigned int value = m_Random->RandomRange(0, 100);//Generate a random number 

		//Get the heros health
		unsigned int heroCurrentHealth = m_World->GetHero()->GetHealth();

		//If the hero has low health
		if (heroCurrentHealth <= HERO_LOW_HEALTH_SOUND_TRIGGER)
		{
			//If the random value is between 0 and 50 (50% drop rate)
			if (value > ENEMY_DROP_RATE_LOW_HERO_HEALTH_HEART_PICKUP_MIN &&
				value < ENEMY_DROP_RATE_LOW_HERO_HEALTH_HEART_PICKUP_MAX)
			{
				GetTile()->AddPickup(new HeartPickup());//drop a heart
			}

			//Drop nothing (50% chance)
		}
		else if (heroCurrentHealth == m_World->GetHero()->GetHealthCapacity())//If the hero has full health
		{
			//If the random value is between 0 and 30 (30% drop rate)
			if (value > ENEMY_DROP_RATE_MAX_HERO_HEALTH_GREEN_RUPEE_PICKUP_MIN &&
				value < ENEMY_DROP_RATE_MAX_HERO_HEALTH_GREEN_RUPEE_PICKUP_MAX)
			{
				GetTile()->AddPickup(new GreenRupeePickup());//drop a green rupee
			}
			else if (value > ENEMY_DROP_RATE_MAX_HERO_HEALTH_BLUE_RUPEE_PICKUP_MIN &&
				value < ENEMY_DROP_RATE_MAX_HERO_HEALTH_BLUE_RUPEE_PICKUP_MAX)//If the random value is between 30 and 50 (20% drop rate)
			{
				GetTile()->AddPickup(new BlueRupeePickup());//drop a BLUE rupee
			}
			//Drop nothing (50% chance)
		}
		else//Normal drop rates
		{
			//If the random value is between 0 and 20 (20% drop rate)
			if (value > ENEMY_DROP_RATE_NORMAL_HEART_PICKUP_MIN &&
				value < ENEMY_DROP_RATE_NORMAL_HEART_PICKUP_MAX)
			{
				GetTile()->AddPickup(new HeartPickup());//drop a heart
			}
			else if (value > ENEMY_DROP_RATE_NORMAL_GREEN_RUPEE_PICKUP_MIN &&
				value < ENEMY_DROP_RATE_NORMAL_GREEN_RUPEE_PICKUP_MAX)//If the random value is between 20 and 40 (20% drop rate)
			{
				GetTile()->AddPickup(new GreenRupeePickup());//drop a green rupee
			}
			else if (value > ENEMY_DROP_RATE_NORMAL_BLUE_RUPEE_PICKUP_MIN &&
				value < ENEMY_DROP_RATE_NORMAL_BLUE_RUPEE_PICKUP_MAX)//If the random value is between 40 and 50 (10% drop rate)
			{
				GetTile()->AddPickup(new BlueRupeePickup());//drop a BLUE rupee
			}
			//Drop nothing (50% chance)
		}


	}

	//Called when the pathfinder found a path to the destination
	void Enemy::StartWalking()
	{
		//refresh the A* debug drawing if the flags are set
		if ((GetSubSection()->GetDebugDrawFlags()&DebugDrawPathFindingScores) > 0)
		{
			GetSubSection()->RefreshDebugDraw();
		}

		//Reset the path index
		m_PathIndex = 0;

		//Call the walk methods, this will be called on a recursive delay until we reach the destinatnion
		Walk();
	}

	//Walk to the next node in the path-called delayed recursively
	void Enemy::Walk()
	{
		if (m_PathFinder->GetPathSize() > 0)
		{
			PathNode* pathNode = m_PathFinder->GetPathNodeAtIndex(m_PathIndex);
			Tile* tile = pathNode->GetTile();

			//get the current position and  the destination position, calculate the direction
			vec2 currentpostion = GetWorldPosition();
			vec2 destinationPosition = tile->GetCenter(true);
			vec2 direction = destinationPosition - currentpostion;

			//Normalize the direction vector
			direction = normalize(direction);

			//Set the enemy's new direction and refresh the active sprite
			SetDirection(direction);
			RefreshActiveSprite();

			//Calculate teh distance and duration
			float distance = sqrtf((destinationPosition.x = currentpostion.x)*
				(destinationPosition.x = currentpostion.x) +
				(destinationPosition.y = currentpostion.y)*
				(destinationPosition.y = currentpostion.y));

			double duration = distance / ENEMY_WALKING_SPEED;

			//Animate the enemy to the center of the tile
			SetLocalPosition(tile->GetCenter(true), duration);

			//Increment the path index
			m_PathIndex++;

			//If the path index has reached the end , stop moving
			if (m_PathIndex == m_PathFinder->GetPathSize())
			{
				SetState(EnemyIdle);
			}
			else
			{
				//Recursively call walk function with a delay.
				DelayGameObjectMethod(&Enemy::Walk, duration);
			}

		}
		else
		{
			SetState(EnemyIdle);
		}
	}

	void Enemy::ChangeState()
	{

		//Randomize the next enemy state
		unsigned int state = m_EnemyState;
		Random randomGenerator;

		while (state == m_EnemyState)//While the enemy doesn't have a different state than the one it currently has
		{
			state = randomGenerator.RandomRange(EnemyIdle, EnemyStateCount);//get a random state including idle
		}

		SetState(state);//Set the new state of the enemy

	}

	void Enemy::RefreshActiveSprite()
	{
		//Disable the active Sprite
		if (m_ActiveSprite != nullptr)
		{
			m_ActiveSprite->SetIsEnabled(false);
		}

		//Set the active Sprite based on the direction
		m_ActiveSprite = m_WalkingSprite[m_DirectionIndex];

		//Set the frame speed
		m_ActiveSprite->SetFrameSpeed(ENEMY_WALKING_ANIMATION_FRAME_SPEED);

		//Reset the frame index to zero and enable it
		m_ActiveSprite->SetFrameIndex(0);
		m_ActiveSprite->SetIsEnabled(true);
	}

	void Enemy::FireProjectile()
	{
		Projectile* activeProjectile = m_ProjectilePool.GetActivateObject();//Get and activate a projectile
		if (activeProjectile != nullptr)
		{
			activeProjectile->Fire(GetSubSection(), GetLocalPosition(), GetDirection(), ENEMY_PROJECTILE_DEFAULT_SPEED, ENEMY_PROJECTILE_DEFAULT_ATTACK_DAMAGE);//Fire the projectile
		}

		//Set the state back to idle
		SetState(EnemyIdle);
	}

	void Enemy::ApplyDamage(unsigned int aAttackDamage)
	{
		//Call the parent class Apply Damage  function
		Player::ApplyDamage(aAttackDamage);

		//Play a sound effect
		if (m_DamagedSound != nullptr)
		{
			m_DamagedSound->Play();
		}
	}

}