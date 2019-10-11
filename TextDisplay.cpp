#include "TextDisplay.h"

TextDisplay::TextDisplay() {
	text.setCharacterSize(16);
}

void TextDisplay::update() {
	text.move(0, -movementSpeed);
	counter++;
	if (counter >= lifeTime) {
		destroyed = true;
	}
}