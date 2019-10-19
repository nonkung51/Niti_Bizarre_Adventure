#include "DialogBox.h"

DialogBox::DialogBox() {
	box.setFillColor(sf::Color::Blue);
	text.setCharacterSize(16);
	text.setFillColor(sf::Color::White);
}

void DialogBox::update(std::string str) {
	text.setString(str);
}