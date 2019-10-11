#pragma once
#include "Entity.h"
#include "helper.h"

class Enemy : public Entity {
	public:
		int attackDamage = 2;
		int hp = 5; 
		bool alive = true;

			 Enemy();
		void update();
		void updateMovement();

	private:
		float movementSpeed = 1.0f;
		int movementLength = 75;
		int counterWalking = 0;
		int direction = 0; // 1 - up, 2 - down, 3 - left, 4 - right
		int counterIdle = 0;
};

