#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>

class WorldGenerator {
	public:
		void CreateMatrix();
		void Interpolation();
		void CreateGraphics();

		void Render(sf::RenderWindow* window);

		int _Matrix[89][89];

	private:
		std::vector<sf::RectangleShape*> _Graphics;

};