#pragma once
#include "Entity.h"

class Wall : public Entity {
	public:
		bool destructable = false;
		bool destroyed = false;
		int hp = 5;
		bool trap = false;
		bool damage = 2;

			 Wall();
		//void update();
	private:
};
