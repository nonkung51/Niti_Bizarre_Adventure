#include "Projectile.h"

Projectile::Projectile() {
	rect.setSize(sf::Vector2f(10, 10));
	rect.setFillColor(sf::Color::Green);
}

void Projectile::update() {
	counterLifetime++;
	animCounter++;
	if (direction == 1) {
		sprite.setTextureRect(sf::IntRect(64 * animCounter, 64 * 2, 64, 64));
		rect.move(0, -movementSpeed);
	}
	else if (direction == 2) {
		sprite.setTextureRect(sf::IntRect(64 * animCounter, 64 * 6, 64, 64));
		rect.move(0, movementSpeed);
	}
	else if (direction == 3) {
		sprite.setTextureRect(sf::IntRect(64 * animCounter, 64 * 0, 64, 64));
		rect.move(-movementSpeed, 0);
	}
	else if (direction == 4) {
		sprite.setTextureRect(sf::IntRect(64 * animCounter, 64 * 4, 64, 64));
		rect.move(movementSpeed, 0);
	}
	if (counterLifetime >= lifeTime) {
		destroyed = true;
	}
	if (animCounter == 7) {
		animCounter = 0;
	}

	sprite.setPosition(rect.getPosition());
}