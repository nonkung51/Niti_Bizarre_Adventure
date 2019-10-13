#include "Enemy.h"

Enemy::Enemy() {
	rect.setSize(sf::Vector2f(64, 64));
	rect.setFillColor(sf::Color::Blue);

	sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
}

void Enemy::update() {
	sprite.setPosition(rect.getPosition());
	text.setPosition(rect.getPosition().x + rect.getSize().y / 2, rect.getPosition().y);
}

void Enemy::updateMovement() {
	if (direction == 1 && cantMoveDi != 1) {
		rect.move({ 0.0f, -movementSpeed });
		sprite.setTextureRect(sf::IntRect(64 * counterWalking, 64, 64, 64));
	} else if (direction == 2 && cantMoveDi != 2) {
		rect.move({ 0.0f, movementSpeed });
		sprite.setTextureRect(sf::IntRect(64 * counterWalking, 64 * 11, 64, 64));
	} else if (direction == 3 && cantMoveDi != 3) {
		rect.move({ -movementSpeed, 0.0f });
		sprite.setTextureRect(sf::IntRect(64 * counterWalking, 64 * 11, 64, 64));
	} else if (direction == 4 && cantMoveDi != 4) {
		rect.move({ movementSpeed, 0.0f });
		sprite.setTextureRect(sf::IntRect(64 * counterWalking, 64, 64, 64));
	} else {
		sprite.setTextureRect(sf::IntRect(64 * (counterIdle % 15), 0, 64, 64));
		// No movement
	}
	counterWalking++;
	if (counterWalking == 11) {
		counterWalking = 0;
	}
	counterIdle++;
	if ((counterIdle >= movementLength || cantMoveDi != 0) && !isAggressive) {
		direction = generateRandom(10);
		counterIdle = 0;
		cantMoveDi = 0;
	}
}
