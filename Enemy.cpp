#include "Enemy.h"
#include <iostream>

Enemy::Enemy() {
	rect.setSize(sf::Vector2f(64, 64));
	rect.setFillColor(sf::Color::Blue);

	sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
}

void Enemy::update() {
	sprite.setPosition(rect.getPosition());
	text.setPosition(rect.getPosition().x + rect.getSize().y / 2, rect.getPosition().y);
}

void Enemy::updateMovement(bool isAttack) {
	if (isAttack) {
		isPlayingAttack = true;
	}
	lastAnim = animClock.getElapsedTime();
	if (lastAnim.asSeconds() > 0.04f) {
		animClock.restart();
		int spriteSizeX = sprite.getTexture()->getSize().x / 13;
		int spriteSizeY = sprite.getTexture()->getSize().y / 21;
		if (isPlayingAttack) {
			std::cout << "attack" << std::endl;
			int animDi[4] = { 4,6,5,7 };
			sprite.setTextureRect(sf::IntRect(spriteSizeX * counterAttack, spriteSizeY * animDi[direction - 1], spriteSizeX, spriteSizeY));
			counterAttack++;
			if (counterAttack == 8) {
				counterAttack = 0;
				sprite.setTextureRect(sf::IntRect(spriteSizeX * 0, spriteSizeY * animDi[direction - 1], spriteSizeX, spriteSizeY));
				isPlayingAttack = false;
			}
			return;
		}
		if (direction == 1 && cantMoveDi != 1) {
			rect.move({ 0.0f, -movementSpeed });
			sprite.setTextureRect(sf::IntRect(spriteSizeX * counterWalking, spriteSizeY *8, spriteSizeX, spriteSizeY));
		}
		else if (direction == 2 && cantMoveDi != 2) {
			rect.move({ 0.0f, movementSpeed });
			sprite.setTextureRect(sf::IntRect(spriteSizeX * counterWalking, spriteSizeY * 10, spriteSizeX, spriteSizeY));
		}
		else if (direction == 3 && cantMoveDi != 3) {
			rect.move({ -movementSpeed, 0.0f });
			sprite.setTextureRect(sf::IntRect(spriteSizeX * counterWalking, spriteSizeY * 9, spriteSizeX, spriteSizeY));
		}
		else if (direction == 4 && cantMoveDi != 4) {
			rect.move({ movementSpeed, 0.0f });
			sprite.setTextureRect(sf::IntRect(spriteSizeX * counterWalking, spriteSizeY * 11, spriteSizeX, spriteSizeY));
		}
		else {
			sprite.setTextureRect(sf::IntRect(spriteSizeX * (counterIdle % 6), 20 * spriteSizeY, spriteSizeX, spriteSizeY));
			// No movement
		}
		counterWalking++;
		if (counterWalking == 9) {
			counterWalking = 0;
		}
		counterIdle++;
		if ((counterIdle >= movementLength || cantMoveDi != 0) && !isAggressive) {
			direction = generateRandom(6);
			counterIdle = 0;
			cantMoveDi = 0;
		}
	}
}
