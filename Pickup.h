#pragma once
#include "Entity.h"

class Pickup : public Entity {
public:
	bool isCoin = true;
	bool destroyed = false;
	int coinValue = 5;

	Pickup();
	void update();
};
