#pragma once
#include "Entity.h"
#include "SFML/Graphics.hpp"

class TextDisplay : public Entity {
	public:
		bool destroyed = false;

			 TextDisplay();
		void update();
	private:
		int lifeTime = 100;
		int counter = 0;
		int movementSpeed = 3;
};

