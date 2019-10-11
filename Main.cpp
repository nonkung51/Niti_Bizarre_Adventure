#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <vector>

#include "Player.h"
#include "Entity.h"
#include "Projectile.h"
#include "Enemy.h"
#include "TextDisplay.h"
#include "helper.h"

using namespace std;

int main() {
	sf::Clock attackClock, enemyAttackPlayerClock;
	sf::RenderWindow window(sf::VideoMode(1000, 800), "Niti Bizarre Adventure");
	window.setFramerateLimit(60);
	srand(time(NULL));

	
	sf::Font font;
	if (!font.loadFromFile("res/font/manaspc.ttf")) {
		cout << "Hey" << endl;
	}
	/*
	sf::Text t("Hi There", font, 22);
	
	sf::Music music;
	if (!music.openFromFile("res/music/Windless Slopes.mp3")) {
		cout << "Hey" << endl;
	}

	//music.play();
	*/

	sf::Texture texturePlayer;
	if (!texturePlayer.loadFromFile("res/img/player.png"))
	{
		cout << "Hey" << endl;
	}

	sf::Texture textureEnemy;
	if (!textureEnemy.loadFromFile("res/img/enemy.png"))
	{
		cout << "Hey" << endl;
	}

	Player player;
	player.sprite.setTexture(texturePlayer);

	vector<Projectile> projectileArray;
	Projectile projectile;

	vector<Enemy> enemyArray;
	Enemy enemy;
	enemy.sprite.setTexture(textureEnemy);
	enemy.sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
	enemy.rect.setPosition(500,200);
	enemyArray.push_back(enemy);

	vector<TextDisplay> textDisplayArray;
	TextDisplay textDisplay;
	textDisplay.text.setFont(font);

	while (window.isOpen()) {
		window.clear();

		// Enemy attack player
		sf::Time enemyAttackPlayer = enemyAttackPlayerClock.getElapsedTime();
		if (enemyAttackPlayer.asSeconds() >= 0.30f) {
			enemyAttackPlayerClock.restart();
			for (vector<Enemy>::iterator eIt = enemyArray.begin(); eIt != enemyArray.end(); eIt++) {
				if (player.rect.getGlobalBounds().intersects((*eIt).rect.getGlobalBounds())) {
					
					textDisplay.text.setPosition(
						player.rect.getPosition().x + player.rect.getSize().x / 2,
						player.rect.getPosition().y - player.rect.getSize().y / 2
					);
					textDisplay.text.setFillColor(sf::Color::Yellow);
					textDisplay.text.setString(to_string((*eIt).attackDamage));
					textDisplayArray.push_back(textDisplay);

					player.hp -= (*eIt).attackDamage;
					cout << player.hp << endl;
				}
			}
		}

		//Check collision
		for (vector<Projectile>::iterator pIt = projectileArray.begin(); pIt != projectileArray.end(); pIt++) {
			for (vector<Enemy>::iterator eIt = enemyArray.begin(); eIt != enemyArray.end(); eIt++) {
				if ((*pIt).rect.getGlobalBounds().intersects(
					(*eIt).rect.getGlobalBounds())) {
					
					//Damage display
					textDisplay.text.setFillColor(sf::Color::Red);
					textDisplay.text.setString(to_string((*pIt).attackDamage));
					textDisplay.text.setPosition(
						(*eIt).rect.getPosition().x + (*eIt).rect.getSize().x / 2,
						(*eIt).rect.getPosition().y - (*eIt).rect.getSize().y / 2
					);
					textDisplayArray.push_back(textDisplay);

					(*pIt).destroyed = true;
					(*eIt).hp -= (*pIt).attackDamage;
					if ((*eIt).hp <= 0) {
						(*eIt).alive = false;
					}
				}
			}
		}
		//Delete dead enemy
		for (vector<Enemy>::iterator eIt = enemyArray.begin(); eIt != enemyArray.end(); eIt++) {
			if ((*eIt).alive == false) {
				enemyArray.erase(eIt);
				break;
			}
		}

		//Delete Bullet
		for (vector<Projectile>::iterator pIt = projectileArray.begin(); pIt != projectileArray.end(); pIt++) {
			if ((*pIt).destroyed) {
				projectileArray.erase(pIt);
				break;
			}
		}

		//Delete TextDisplay
		for (vector<TextDisplay>::iterator tIt = textDisplayArray.begin(); tIt != textDisplayArray.end(); tIt++) {
			if ((*tIt).destroyed) {
				textDisplayArray.erase(tIt);
				break;
			}
		}

		//Attack
		sf::Time checkAttack = attackClock.getElapsedTime();
		if (checkAttack.asSeconds() >= 0.15f) {
			attackClock.restart();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				projectile.rect.setPosition(
					player.rect.getPosition().x + player.rect.getSize().x / 2 - projectile.rect.getSize().x / 2,
					player.rect.getPosition().y + player.rect.getSize().y / 2 - projectile.rect.getSize().y / 2
				);
				projectile.direction = player.direction;
				projectileArray.push_back(projectile);
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {
			enemy.rect.setPosition(
				generateRandom(window.getSize().x), 
				generateRandom(window.getSize().y)
			);
			enemyArray.push_back(enemy);
		}

		//Update Bullet
		for (vector<Projectile>::iterator pIt = projectileArray.begin(); pIt != projectileArray.end(); pIt++) {
			(*pIt).update();
			window.draw((*pIt).rect);
		}

		//Update Enemy
		for (vector<Enemy>::iterator eIt = enemyArray.begin(); eIt != enemyArray.end(); eIt++) {
			(*eIt).update();
			(*eIt).updateMovement();
			//window.draw(enemyArray[counter].rect);
			window.draw((*eIt).sprite);
		}

		player.updateMovement();
		player.update();
		window.draw(player.sprite);

		// Draw damage effect
		for (vector<TextDisplay>::iterator tIt = textDisplayArray.begin(); tIt != textDisplayArray.end(); tIt++) {
			(*tIt).update();
			window.draw((*tIt).text);
		}

		window.display();
	}
}
