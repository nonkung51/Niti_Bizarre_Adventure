#pragma once
#include "Entity.h"

class Player : public Entity {
	public:
		int hp = 20;
		int maxHp = 20;
		int direction = 0; // 1 - up, 2 - down, 3 - left, 4 - right
		int coin = 0;
		int cantMoveDi = 0;
		float movementSpeed = 2.0f;

			 Player();
		void update();
		void updateMovement(bool);
	private:
		sf::Time lastAnim;
		sf::Clock clock;
		bool isPlayingAttack = false;
		float walkSpeed = 2.0f;
		float runningSpeed = 4.0f;
		int counterWalking = 0;
		int counterAttack = 0;
};

