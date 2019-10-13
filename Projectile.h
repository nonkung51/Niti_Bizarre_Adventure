#pragma once

#include "Entity.h"
#include "SFML/Graphics.hpp"

class Projectile : public Entity {
	public:
		int attackDamage = 1;
		int direction = 0; // 1 - up, 2 - down, 3 - left, 4 - right
		bool destroyed = false;

			 Projectile();
		void update();
	private:
		int animCounter = 0;
		int lifeTime = 2;
		int counterLifetime = 0;
		float movementSpeed = 10.0f;
};

