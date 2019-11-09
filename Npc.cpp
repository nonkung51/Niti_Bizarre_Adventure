#include "Npc.h"

Npc::Npc() {
	rect.setSize(sf::Vector2f(64, 64));
	rect.setFillColor(sf::Color::Green);
}

void Npc::update() {
	sprite.setPosition(rect.getPosition().x - 32, rect.getPosition().y - 64);
}