#include "Player.h"

Player::Player() {
	rect.setSize(sf::Vector2f(32, 32));
	rect.setPosition(600, 600);
	rect.setFillColor(sf::Color::Blue);

	sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
	lastAnim = clock.getElapsedTime();
}

void Player::update() {
	sprite.setPosition(rect.getPosition().x - 16, rect.getPosition().y - 32);
}

void Player::updateMovement(bool isPlayAttack) {
	if (isPlayAttack) {
		isPlayingAttack = true;
	}
	lastAnim = clock.getElapsedTime();
	if (lastAnim.asSeconds() > 0.01) {
		clock.restart();
		int spriteSizeX = sprite.getTexture()->getSize().x / 13;
		int spriteSizeY = sprite.getTexture()->getSize().y / 21;
		if (isPlayingAttack) {
			int animDi[4] = {4,6,5,7};
			sprite.setTextureRect(sf::IntRect(spriteSizeX * counterAttack, spriteSizeY * animDi[direction - 1], spriteSizeX, spriteSizeY));
			counterAttack++;
			if (counterAttack == 8) {
				counterAttack = 0;
				sprite.setTextureRect(sf::IntRect(spriteSizeX * 0, spriteSizeY * animDi[direction - 1], spriteSizeX, spriteSizeY));
				isPlayingAttack = false;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && cantMoveDi != 1) {
			rect.move({ 0.0f, -movementSpeed });
			sprite.setTextureRect(sf::IntRect(spriteSizeX * counterWalking, spriteSizeY * 8, spriteSizeX, spriteSizeY));
			direction = 1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && cantMoveDi != 2) {
			rect.move({ 0.0f, movementSpeed });
			sprite.setTextureRect(sf::IntRect(spriteSizeX * counterWalking, spriteSizeY * 10, spriteSizeX, spriteSizeY));
			direction = 2;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && cantMoveDi != 3) {
			rect.move({ -movementSpeed, 0.0f });
			sprite.setTextureRect(sf::IntRect(spriteSizeX * counterWalking, spriteSizeY * 9, spriteSizeX, spriteSizeY));
			direction = 3;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && cantMoveDi != 4) {
			rect.move({ movementSpeed, 0.0f });
			sprite.setTextureRect(sf::IntRect(spriteSizeX * counterWalking, spriteSizeY * 11, spriteSizeX, spriteSizeY));
			direction = 4;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
			movementSpeed = runningSpeed;
		}
		else {
			movementSpeed = walkSpeed;
		}
		counterWalking++;
		if (counterWalking == 8) {
			counterWalking = 0;
		}
		cantMoveDi = 0;
	}
}