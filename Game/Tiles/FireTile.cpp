/*******************************************************************
FireTile.cpp

Student:        Alvaro Chavez Mixco
Algonquin College
Date:           December 12, 2015
Course Number:  GAM1540
Professor:      David McCue
Purpose:
The Fire Tile basically handles displaying the correct sprite. Setting
if it is walkable, load the sprite corresponding to the different variants
it has, and returning the correct tile type; it also animates the sprite.
*********************************************************************/

#include "FireTile.h"
#include "../../Source/UI/UI.h"

namespace GameDev2D
{
	FireTile::FireTile(SubSection* aSubSection, uvec2 aCoordinates, unsigned int aVariant) : Tile("Fire-1", aSubSection, aCoordinates) ,
		m_AnimationFrameRate(FIRE_TILE_DEFAULT_ANIMATION_FRAMERATE),
		m_CurrentFrame(false)
	{
		//Set the variant
		SetVariant(aVariant);

		//Set that the fire tile ISN'T walkable
		SetIsWalkable(false);
	}

	FireTile::~FireTile()
	{
	}

	void FireTile::Update(double aDelta)
	{
		
		m_AnimationFrameRate -= aDelta;//Decrease the timer

		//If is time to change animatio
		if (m_AnimationFrameRate <= 0.0)
		{
			changeAnimation();//Change the current frame of animation
			m_AnimationFrameRate = FIRE_TILE_DEFAULT_ANIMATION_FRAMERATE;//Reset the timer
		}

		Tile::Update(aDelta);
	}

	TileType FireTile::GetTileType()
	{
		return TileTypeFire;
	}

	unsigned int FireTile::GetVariantCount()
	{
		return 1; //Only return 1 variant count, since there is only 1 variant. But animated
	}

	void FireTile::GetAtlasKeyForVariant(unsigned int aVariant, string& aAtlasKey)
	{
		switch (aVariant)
		{
		case 0:
			aAtlasKey = "Fire-1";
			break;

		case 1:
			aAtlasKey = "Fire-2";
			break;

		default:
			break;
		}
	}

	//Changes the current sprite showing
	////Since we only have 2 animations we can easily use a bool to change them
	void FireTile::changeAnimation()
	{			
		m_CurrentFrame = !m_CurrentFrame;//Since we only have 2 frames, by reversing it we will be changing between the 0 and 1 frame.
		SetVariant(m_CurrentFrame);
	}
}