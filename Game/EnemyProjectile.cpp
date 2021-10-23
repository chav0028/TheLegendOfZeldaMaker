#include "EnemyProjectile.h"
#include "../../Source/UI/Sprite/Sprite.h"

namespace GameDev2D
{
	EnemyProjectile::EnemyProjectile()
	{
		LoadSprite();
	}

	EnemyProjectile::~EnemyProjectile()
	{
	}

	void EnemyProjectile::LoadSprite()
	{
		// create sprite and add it as a child.
		m_Sprite = new Sprite("MainAtlas", "EnemyProjectile");
		m_Sprite->SetAnchorPoint(0.5, 0.5);
		AddChild(m_Sprite, true);
	}

}
