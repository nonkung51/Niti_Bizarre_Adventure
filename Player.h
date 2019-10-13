#pragma once
#include "Entity.h"

class Player : public Entity {
	public:
		int hp = 10;
		int maxHp = 10;
		int direction = 0; // 1 - up, 2 - down, 3 - left, 4 - right
		int coin = 0;
		int cantMoveDi = 0;
		bool powerUp = false;
		float movementSpeed = 2.0f;

			 Player();
		void update();
		void updateMovement();
	private:
		float walkSpeed = 2.0f;
		float runningSpeed = 4.0f;
		int counterWalking = 0;
};

