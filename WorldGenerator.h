#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>

class WorldGenerator {
	public:
		void CreateMatrix();
		void Interpolation();

		int _Matrix[65][65];

	private:
		std::vector<sf::RectangleShape*> _Graphics;

};