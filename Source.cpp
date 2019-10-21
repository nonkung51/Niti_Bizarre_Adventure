#include <SFML\Graphics.hpp>
#include <iostream>
#include <fstream>

int _main()
{
	sf::RenderWindow window(sf::VideoMode(1080, 760), "menu");
	window.setFramerateLimit(144);

	sf::String playerInput;
	sf::Text playerText;

	sf::Font font;
	if (!font.loadFromFile("res/font/manaspc.ttf")) {
		std::cout << "Can\'t load font" << std::endl;
	}

	std::ofstream fileWriter;
	

	std::string name;
	playerText.setFillColor(sf::Color::White);
	playerText.setFont(font);
	playerText.setCharacterSize(22);

	while (window.isOpen())
	{
		window.clear();
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode == 13) {
					fileWriter.open("save/score.txt", std::ios::out | std::ios::app);
					fileWriter << playerInput.toAnsiString() << "," << 500 << '\n';
					fileWriter.close();
					playerInput.clear();
				}
				playerInput += event.text.unicode;
				playerText.setString(playerInput);
			}
		}
		window.draw(playerText);
		window.display();
	}

	return 0;
}