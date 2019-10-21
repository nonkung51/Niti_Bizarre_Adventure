#pragma once
#include <SFML/Graphics.hpp>
#include <string>
class DialogBox {
public:
	sf::Text text;
	sf::RectangleShape box;
	bool isShow = false;
	DialogBox();
	void update();
	void reset();
	std::vector<std::string> conversation;
	int curIndex = 0;
};
