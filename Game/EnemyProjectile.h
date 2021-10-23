#ifndef __GameDev2D__EnemyProjectile__
#define __GameDev2D__EnemyProjectile__

#include "Projectile.h"

using namespace glm;

namespace GameDev2D
{

	class EnemyProjectile : public Projectile
	{
	public:
		EnemyProjectile();
		~EnemyProjectile();

		void LoadSprite();
	};
}

#endif /* defined(__GameDev2D__EnemyProjectile__) */
