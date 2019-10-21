#include "Npc.h"

Npc::Npc() {
	rect.setSize(sf::Vector2f(32, 32));
	rect.setFillColor(sf::Color::Green);
}

void Npc::update() {
	sprite.setPosition(rect.getPosition().x - 16, rect.getPosition().y - 32);
}