#pragma once
#include "Entity.h"

class Player : public Entity {
	public:
		int hp = 10;
		int direction = 0; // 1 - up, 2 - down, 3 - left, 4 - right

			 Player();
		void update();
		void updateMovement();
	private:
		float movementSpeed = 2.0f;
		int counterWalking = 0;
};

