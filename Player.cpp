#include "Player.h"

Player::Player() {
	rect.setSize(sf::Vector2f(32, 32));
	rect.setPosition(400, 200);
	rect.setFillColor(sf::Color::Blue);

	sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
}

void Player::update() {
	sprite.setPosition(rect.getPosition());
}

void Player::updateMovement() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		rect.move({ 0.0f, -movementSpeed });
		sprite.setTextureRect(sf::IntRect(32 * counterWalking, 32, 32, 32));
		direction = 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		rect.move({ 0.0f, movementSpeed });
		sprite.setTextureRect(sf::IntRect(32 * counterWalking, 32 * 9, 32, 32));
		direction = 2;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		rect.move({ -movementSpeed, 0.0f });
		sprite.setTextureRect(sf::IntRect(32 * counterWalking, 32 * 9, 32, 32));
		direction = 3;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		rect.move({ movementSpeed, 0.0f });
		sprite.setTextureRect(sf::IntRect(32 * counterWalking, 32, 32, 32));
		direction = 4;
		
	}
	counterWalking++;
	if (counterWalking == 8) {
		counterWalking = 0;
	}
}