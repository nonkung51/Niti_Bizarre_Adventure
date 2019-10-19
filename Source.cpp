#include <SFML\Graphics.hpp>
#include <iostream>

#include "WorldGenerator.h"

int _main()
{
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "ProzedualeLevelgenerierung");
	window.setFramerateLimit(144);

	sf::Event event;
	WorldGenerator _WorldGenerator;
	_WorldGenerator.CreateMatrix();
	_WorldGenerator.Interpolation();
	_WorldGenerator.CreateGraphics();


	while (window.isOpen())
	{
		window.clear();

		_WorldGenerator.Render(&window);

		window.display();

		//_WorldGenerator.HandleEvents(&window);

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				return 0;
			}
		}
	}


	system("pause");
	return 0;
}