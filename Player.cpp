#include "Player.h"

Player::Player() {
	rect.setSize(sf::Vector2f(32, 32));
	rect.setPosition(600, 600);
	rect.setFillColor(sf::Color::Blue);

	sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
}

void Player::update() {
	sprite.setPosition(rect.getPosition());
}

void Player::updateMovement() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && cantMoveDi != 1 ) {
		rect.move({ 0.0f, -movementSpeed });
		sprite.setTextureRect(sf::IntRect(32 * counterWalking, 32, 32, 32));
		direction = 1;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && cantMoveDi != 2) {
		rect.move({ 0.0f, movementSpeed });
		sprite.setTextureRect(sf::IntRect(32 * counterWalking, 32 * 9, 32, 32));
		direction = 2;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && cantMoveDi != 3) {
		rect.move({ -movementSpeed, 0.0f });
		sprite.setTextureRect(sf::IntRect(32 * counterWalking, 32 * 9, 32, 32));
		direction = 3;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && cantMoveDi != 4) {
		rect.move({ movementSpeed, 0.0f });
		sprite.setTextureRect(sf::IntRect(32 * counterWalking, 32, 32, 32));
		direction = 4;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		movementSpeed = runningSpeed;
	} else {
		movementSpeed = walkSpeed;
	}
	counterWalking++;
	if (counterWalking == 8) {
		counterWalking = 0;
	}
	cantMoveDi = 0;
}