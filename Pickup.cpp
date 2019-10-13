#include "Pickup.h"


Pickup::Pickup() {
	rect.setSize(sf::Vector2f(24, 24));
	rect.setPosition(0, 0);
	rect.setFillColor(sf::Color::Yellow);

	text.setCharacterSize(8);
}

void Pickup::update() {
	sprite.setPosition(rect.getPosition());
	text.setPosition(rect.getPosition().x - 20, rect.getPosition().y + 20);
}
