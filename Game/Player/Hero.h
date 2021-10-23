//
//  Hero.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-14.
//  Copyright (c) 2014 Algonquin College. All rights reserved.

/*
ChangeLog
_______________________________________________
-Added dead state to palyer.
-Added different sound variables to player
-Added projectile variable and fucntion to player
*/


#ifndef __GameDev2D__Hero__
#define __GameDev2D__Hero__

#include "Player.h"
#include "../../Source/Input/KeyCodes.h"


namespace GameDev2D
{
    //Enum to help manage the hero's internal state
    enum HeroState
    {
        HeroIdle = 0,
        HeroWalking,
        HeroAttacking,
        HeroDead,
        HeroUnknown
    };

    //Local constants
    const string HERO_WALKING_ATLAS_KEY_1[] = { "LinkUp-1", "LinkDown-1", "LinkLeft-1", "LinkRight-1" };
    const string HERO_WALKING_ATLAS_KEY_2[] = { "LinkUp-2", "LinkDown-2", "LinkLeft-2", "LinkRight-2" };
    const string HERO_ATTACKING_ATLAS_KEY[] = { "LinkAttack-Up", "LinkAttack-Down", "LinkAttack-Left", "LinkAttack-Right" };
    const vec2 HERO_ATTACKING_ANCHOR_POINT[] = { vec2(0.5f, 0.3f), vec2(0.5f, 0.675f), vec2(0.7f, 0.5f), vec2(0.3f, 0.5f) };
    const double HERO_ATTACK_TIME = 0.2;
    const float HERO_WALKING_SPEED = 200.0f;
    const float HERO_WALKING_ANIMATION_FRAME_SPEED = 8.0f;
    const unsigned int HERO_INITIAL_HEALTH = 6;
    const unsigned int HERO_INITIAL_HEALTH_CAPACITY = 6;
    const unsigned char HERO_MAX_BOMBS = 6;
    const vec2 HERO_SWORD_RANGE = vec2(70.0f, 68.0f);
    const double HERO_INVINCIBILITY_DURATION = 0.65;
    const float HERO_COLLISION_ZONE = 20.0f;
    const bool HERO_HAS_SWORD_BY_DEFAULT = true;

    const float HERO_PROJECTILE_DEFAULT_SPEED = 300.0f;
    const unsigned int HERO_PROJECTILE_DEFAULT_ATTACK_DAMAGE = 1;//Different from the hero normal damage

    //Hero low health count, the number of HP the hero has before teh low health count starts
    const unsigned int HERO_LOW_HEALTH_SOUND_TRIGGER = 2;

    //Values of rupees
    const unsigned int HERO_MAX_RUPEES = 255;
    const unsigned int HERO_GREEN_RUPEE_VALUE = 1;
    const unsigned int HERO_BLUE_RUPEE_VALUE = 5;

    //Because in the game a heart has full and half state, with 3 full healths the hero actually have 6 hp
    const unsigned int HERO_HALF_HEART = 1;
    const unsigned int HERO_FULL_HEART = 2;

    const double TRANSITION_BACK_TO_MAIN_MENU_TIME_FROM_HERO_DEAD = 1.0;//Time the game will wait before it takes the player back to main menu

    const float HERO_FLICKERING_MIN_ALPHA = 0.2f;
    const float HERO_FLICKER_MAX_ALPHA = 0.8f;

    const unsigned int HERO_NUMBER_OF_SPRITES = 4;

    //Forward Declarations
    class Sprite;
    class Tile;
    class Audio;
    class Circle;
    class HeroProjectile;

    //The Hero class is controlled by keyboard input
    class Hero : public Player, EventHandler
    {
    public:
        Hero(World* world, Tile* spawnTile);
        ~Hero();

        //Update method, used to determine if an enemy has been attacked
        void Update(double delta);
        void Draw();

        //Used to reset the hero
        void Reset();

        //Pure virtual function used to manage the hero's internal state
        void SetState(unsigned int state);

        //Returns wether the hero has his sword or not
        bool HasSword();

        //Call this method when the hero picks up a sword, allows the
        //hero to now be able to swing his sword and attack with it
        void PickedUpSword();

        //Adds rupees to the hero's rupee stock-pile
        void AddRupees(unsigned short rupees);

        //Returns the number of rupees the hero has
        unsigned short GetRupees();

        //Returns wether the hero is invincible or not
        bool IsInvincible();

        //Applies damage to the hero
        void ApplyDamage(unsigned int attackDamage);

        void HandleEvent(Event* event);//Made public so that it can be handled in game.cpp

    private:
        //Called when the Hero has ceased to exist
        void HasDied();
        void GoToMainMenu();

		void StopAllSound();

        //Methods to handle hero input
        void HandleKeyDown(KeyCode keyCode);
        void HandleKeyUp(KeyCode keyCode);

        void FireProjectile();

        //Convenience method to separate code
        void HeroMovementCollision(double delta);
        void HeroAttackCollision(double delta);
        void HeroProjectileCollision(double delta);

        //Method used to stop attacking, called from a delayed method
        void StopAttacking();

        void HasChangedTiles(Tile* newTile, Tile* oldTile);

        //Member variables
        Sprite* m_AttackingSprite[HERO_NUMBER_OF_SPRITES];
        HeroState m_HeroState;
        unsigned short m_Rupees;
        bool m_HasSword;
        Timer* m_InvincibilityTimer;
        HeroProjectile* m_HeroProjectile;

        //Sounds
        Audio* m_SwordAttackSound;
        Audio* m_LowHealthSound;
		Audio* m_HeroHurtSound;
        Audio* m_HeartPickupSound;
        Audio* m_RupeePickupSound;
    };
}

#endif /* defined(__GameDev2D__Hero__) */
