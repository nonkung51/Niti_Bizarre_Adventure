#pragma once
#include <SFML/Graphics.hpp>
#include <string>
class DialogBox {
public:
	sf::Text text;
	sf::RectangleShape box;
	bool isShow = false;
	bool finish = false;
	std::string active = "";
	DialogBox();
	void update();
	void reset();
	std::vector<std::string> conversation;
	int curIndex = 0;

	void setDialog(std::vector<std::string>, std::string);
};
