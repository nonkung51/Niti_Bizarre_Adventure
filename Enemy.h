#pragma once
#include "Entity.h"
#include "helper.h"

class Enemy : public Entity {
	public:
		int direction = 0; // 1 - up, 2 - down, 3 - left, 4 - right
		int attackDamage = 2;
		int hp = 5; 
		bool alive = true;
		int cantMoveDi = 0;
		float movementSpeed = 1.0f;
			 Enemy();
		void update();
		void updateMovement();

	private:
		int movementLength = 75;
		int counterWalking = 0;
		int counterIdle = 0;
};

