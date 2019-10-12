#include "Wall.h"

Wall::Wall() {
	rect.setSize(sf::Vector2f(50, 50));
	rect.setPosition(0, 0);
	rect.setFillColor(sf::Color::White);

	//sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
}

/*
void Wall::update() {
	sprite.setPosition(rect.getPosition());
}*/