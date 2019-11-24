#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <vector>
#include <fstream>

#include "Game.h"

using namespace std;

int main() {
	int activeButton = 0;
	int state = 2;
	int playingScore = 0;
	bool isPlaying = false;
	sf::RenderWindow window(sf::VideoMode(1960, 1080), "Niti Bizarre Adventure", sf::Style::Fullscreen);
	Game game(&window, &isPlaying, &playingScore, &state);
	window.setFramerateLimit(60);

	sf::Texture logoTexture;
	if (!logoTexture.loadFromFile("res/img/nitibizarreadventure.png")) {
		std::cout << "picture not loaded." << std::endl;
	}
	sf::Sprite logoSprite;
	logoSprite.setTexture(logoTexture);
	logoSprite.setScale({ 0.2, 0.2 });
	logoSprite.setPosition(600, 75);
	
	sf::Texture bgTexture;
	if (!bgTexture.loadFromFile("res/img/bg.png")) {
		std::cout << "bg.png picture not loaded." << std::endl;
	}
	sf::Sprite bgSprite;
	bgSprite.setTexture(bgTexture);
	bgSprite.setScale({ 1.5, 1.5 });
	//bgSprite.setPosition(window.getSize().x / 2 - bgTexture.getSize().x / 2, window.getSize().y / 4 - bgTexture.getSize().y / 2);

	sf::Texture starTexture;
	if (!starTexture.loadFromFile("res/img/stars.png")) {
		std::cout << "picture not loaded." << std::endl;
	}
	sf::Sprite starSprite;
	starSprite.setTexture(starTexture);
	starSprite.setScale({1.6, 1.6});
	//starSprite.setPosition(window.getSize().x / 2 - logoTexture.getSize().x / 2 * 0.15, window.getSize().y / 4 - logoTexture.getSize().y / 2 * 0.15);


	std::vector<std::string> buttonText{ "Start", "Score", "Close" };

	sf::Font font;
	if (!font.loadFromFile("res/font/manaspc.ttf")) {
		std::cout << "Can\'t load font" << std::endl;
	}

	sf::Text myname("62010452 Nonthakon Jitchiranant", font);
	myname.setFillColor(sf::Color::Yellow);
	myname.setPosition({1300, 1000});

	//Output player score
	std::ofstream fileWriter;
	sf::String playerInput;

	// Extract save files
	std::map<int, std::string> score;
	std::ifstream fileReader;

	std::string word;

	int starSpriteAnimation = 0;

	while (window.isOpen()) {
		if (!isPlaying) {
			window.setView(sf::View({ 1960/2,1080 /2 }, { 1960, 1080 }));
			window.clear(sf::Color(168, 109, 171));
			if (starSpriteAnimation <= 10) {
				starSprite.move({ 1, -1 });
				logoSprite.move({ -0.5, 0.5 });
				myname.move({ 0, 0.5 });
			}
			else {
				starSprite.move({ -1, 1 });
				logoSprite.move({ 0.5, -0.5 });
				myname.move({ 0, -0.5 });
				if (starSpriteAnimation == 20) {
					starSpriteAnimation = 0;
				}
			}
			starSpriteAnimation++;
			if (state == 0) {
				window.draw(bgSprite);
				window.draw(starSprite);
				window.draw(logoSprite);
				window.draw(myname);
				for (int button = 0; button < 3; button++) {
					sf::Text text(buttonText.at(button), font);
					text.setCharacterSize(30);
					text.setFillColor(sf::Color::White);
					if (button == activeButton) {
						text.setStyle(sf::Text::Bold);
						text.setFillColor(sf::Color::Red);
					}
					text.setPosition(550 + 300 * button, 800);
					window.draw(text);
				}
			}
			if (state == 1) {
				window.draw(bgSprite);
				window.draw(starSprite);
				window.draw(logoSprite);
				window.draw(myname);
				sf::Text text("Enter to go back", font);
				text.setCharacterSize(30);
				text.setFillColor(sf::Color::White);

				fileReader.open("save/score.txt");
				do {
					fileReader >> word;
					auto first_token = word.substr(0, word.find(','));
					auto second_token = std::stoi(word.substr(word.find(',') + 1, word.length()));
					score[second_token] = first_token;
				} while (fileReader.good());
				fileReader.close();

				std::map<int, std::string>::iterator end = score.end();
				std::map<int, std::string>::iterator beg = score.begin();
				end--;
				beg--;
				int currentDisplay = 0;
				for (std::map<int, std::string>::iterator it = end; it != beg; it--) {
					text.setString(it->second);
					text.setPosition(800, 700 + 40 * currentDisplay);
					window.draw(text);
					text.setString(std::to_string(it->first));
					text.setPosition(1400, 700 + 40 * currentDisplay);
					window.draw(text);
					currentDisplay++;
					if (currentDisplay == 5) {
						break;
					}
				}
				currentDisplay = 0;
				text.setString("Enter to go back");
				text.setPosition(800, 960);
				window.draw(text);
			}
			if (state == 2) {
				window.draw(bgSprite);
				window.draw(starSprite);
				window.draw(logoSprite);
				window.draw(myname);
				sf::Text text("", font);
				text.setString("Score: " + to_string(playingScore));
				text.setPosition(700, 500);
				window.draw(text);
				text.setString("Enter your name: ");
				text.setPosition(700, 650);
				window.draw(text);
				text.setString(playerInput);
				text.setPosition(800, 750);
				window.draw(text);
				text.setString("Press enter to save");
				text.setPosition(900, 950);
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
						} else if(state == 1){
							state = 0;
						} else if (state == 2) {
							std::cout << "enter" << std::endl;
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
				if (event.type == sf::Event::TextEntered && state == 2) {
					if (event.text.unicode == 13) { //enter
						fileWriter.open("save/score.txt", std::ios::out | std::ios::app);
						fileWriter << playerInput.toAnsiString() << "," << playingScore << '\n';
						fileWriter.close();
						playerInput.clear();
						state = 0;
						playingScore = 0;
					}
					if (event.text.unicode == 8) { //backspace
						playerInput = playerInput.substring(0, playerInput.getSize() - 1);
					}
					else if (event.text.unicode == 32) {
						playerInput += '_';
					}
					else {
						playerInput += event.text.unicode;
					}
				}
			}
		}
		else {
			game.run();
		}
	}
	return 0;
}
