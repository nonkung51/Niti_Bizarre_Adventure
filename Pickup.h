#pragma once
#include "Entity.h"

class Pickup : public Entity {
public:
	int coinValue = 5;
	int cost = 5;
	bool isCoin = false;
	bool isPowerUp = false;
	bool destroyed = false;
	bool inShop = true;

	Pickup();
	void update();
};
