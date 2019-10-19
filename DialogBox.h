#pragma once
#include <SFML/Graphics.hpp>
class DialogBox {
public:
	sf::Text text;
	sf::RectangleShape box;
	bool isShow = false;
	DialogBox();
	void update(std::string);
};
