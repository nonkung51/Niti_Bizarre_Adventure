#include "Pickup.h"


Pickup::Pickup() {
	rect.setSize(sf::Vector2f(24, 24));
	rect.setPosition(0, 0);
	rect.setFillColor(sf::Color::Yellow);

	//sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
}

void Pickup::update() {
	sprite.setPosition(rect.getPosition());
}
