#include "DialogBox.h"

DialogBox::DialogBox() {
	box.setFillColor(sf::Color::Blue);
	text.setCharacterSize(16);
	text.setFillColor(sf::Color::White);
}

void DialogBox::update() {
	curIndex++;
	if (curIndex == conversation.size()) {
		reset();
		finish = true;
		isShow = false;
		return;
	}
	text.setString(conversation.at(curIndex));
}

void DialogBox::reset() {
	curIndex = 0;
	text.setString(conversation.at(curIndex));
}

void DialogBox::setDialog(std::vector<std::string> conv, std::string act) {
	conversation = conv;
	active = act;
	isShow = true;
	finish = false;
	text.setString(conversation.at(curIndex));
}
