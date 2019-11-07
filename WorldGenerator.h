#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>

class WorldGenerator {
	public:
		void CreateMatrix();
		void Interpolation();

		int _Matrix[49][49];

	private:
		std::vector<sf::RectangleShape*> _Graphics;

};