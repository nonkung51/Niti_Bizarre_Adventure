#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <vector>
#include <fstream>

#include "Game.h"

using namespace std;

int main() {
	sf::RenderWindow window(sf::VideoMode(1080, 760), "Niti Bizarre Adventure");
	Game game;
	window.setFramerateLimit(60);

	sf::Texture logoTexture;
	if (!logoTexture.loadFromFile("res/img/nitibizarreadventure.png")) {
		std::cout << "picture not loaded." << std::endl;
	}
	sf::Sprite logoSprite;
	logoSprite.setTexture(logoTexture);
	logoSprite.setScale({ 0.15, 0.15 });
	logoSprite.setPosition(window.getSize().x / 2 - logoTexture.getSize().x / 2 * 0.15, window.getSize().y / 4 - logoTexture.getSize().y / 2 * 0.15);

	int activeButton = 0;
	int state = 0;
	bool isPlaying = false;
	std::vector<std::string> buttonText{ "Start", "Score", "Close" };

	sf::Font font;
	if (!font.loadFromFile("res/font/manaspc.ttf")) {
		std::cout << "Can\'t load font" << std::endl;
	}

	// Extract save files
	std::map<int, std::string> score;
	std::ifstream fileReader;
	fileReader.open("save/score.txt");

	std::string word;
	//std::string save;
	do {
		fileReader >> word;
		auto first_token = word.substr(0, word.find(','));
		auto second_token = std::stoi(word.substr(word.find(',') + 1, word.length()));
		score[second_token] = first_token;
	} while (fileReader.good());
	while (window.isOpen()) {
		if (!isPlaying) {
			window.clear(sf::Color(168, 109, 171));
			if (state == 0) {
				window.draw(logoSprite);
				for (int button = 0; button < 3; button++) {
					sf::Text text(buttonText.at(button), font);
					text.setCharacterSize(30);
					text.setFillColor(sf::Color::White);
					if (button == activeButton) {
						text.setStyle(sf::Text::Bold);
						text.setFillColor(sf::Color::Red);
					}
					text.setPosition(250 + 300 * button, 500);
					window.draw(text);
				}
			}
			if (state == 1) {
				window.draw(logoSprite);
				sf::Text text("Enter to go back", font);
				text.setCharacterSize(30);
				text.setFillColor(sf::Color::White);

				std::map<int, std::string>::iterator end = score.end();
				std::map<int, std::string>::iterator beg = score.begin();
				end--;
				beg--;
				int currentDisplay = 0;
				for (std::map<int, std::string>::iterator it = end; it != beg; it--) {
					//std::cout << it->first << " : " << it->second << std::endl;
					text.setString(it->second);
					text.setPosition(350, 400 + 40 * currentDisplay);
					window.draw(text);
					text.setString(std::to_string(it->first));
					text.setPosition(700, 400 + 40 * currentDisplay);
					window.draw(text);
					currentDisplay++;
				}
				currentDisplay = 0;
				text.setString("Enter to go back");
				text.setPosition(350, 650);
				window.draw(text);
			}

			window.display();

			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::KeyPressed) {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
						if (state == 0) {
							if (activeButton == 0) {
								isPlaying = true;
								std::cout << "start playing" << std::endl;
							}
							if (activeButton == 1) {
								state = 1;
							}
							if (activeButton == 2) {
								return 0;
							}
						}
						else {
							state = 0;
						}
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
						activeButton--;
						if (activeButton < 0) {
							activeButton = 2;
						}
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
						activeButton++;
						if (activeButton > 2) {
							activeButton = 0;
						}
					}
				}
				if (event.type == sf::Event::Closed) {
					return 0;
				}
			}
		}
		else {
			game.run();
		}
	}
	//return 0;
}
