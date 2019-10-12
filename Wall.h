#pragma once
#include "Entity.h"

class Wall : public Entity {
	public:
		bool destructable = false;
		bool destroyed = false;
		int hp = 5;

			 Wall();
		//void update();
	private:
};
