#include "Projectile.h"

Projectile::Projectile() {
	rect.setSize(sf::Vector2f(10, 10));
	rect.setFillColor(sf::Color::Green);
}

void Projectile::update() {
	counterLifetime++;
	if (direction == 1) {
		rect.move(0, -movementSpeed);
	}
	else if (direction == 2) {
		rect.move(0, movementSpeed);
	}
	else if (direction == 3) {
		rect.move(-movementSpeed, 0);
	}
	else if (direction == 4) {
		rect.move(movementSpeed, 0);
	}
	if (counterLifetime >= lifeTime) {
		destroyed = true;
	}
}