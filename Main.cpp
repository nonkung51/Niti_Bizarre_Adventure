#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <vector>

#include "Player.h"
#include "Entity.h"
#include "Projectile.h"
#include "Enemy.h"
#include "TextDisplay.h"
#include "Pickup.h"
#include "Wall.h"
#include "helper.h"

using namespace std;

int main() {
	sf::Clock attackClock, enemyAttackPlayerClock, aggressiveEnemyClock;
	sf::RenderWindow window(sf::VideoMode(1080, 760), "Niti Bizarre Adventure");
	window.setFramerateLimit(60);

	//View
	sf::View view(sf::FloatRect(200, 200, 300, 200));
	view.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
	view.setCenter(sf::Vector2f(view.getSize().x / 2, view.getSize().y / 2));
	window.setView(view);

	srand(time(NULL));

	
	sf::Font font;
	if (!font.loadFromFile("res/font/manaspc.ttf")) {
		cout << "Can\'t load font" << endl;
	}
	
	// Theme song
	sf::Music music;
	if (!music.openFromFile("res/music/Windless Slopes.ogg")) {
		cout << "res/music/Windless Slopes.ogg not loaded." << endl;
	}
	music.setLoop(true);
	music.play();
	

	//Sound effect
	sf::SoundBuffer shotBuffer;
	if (!shotBuffer.loadFromFile("res/sound/fire.wav")) {
		cout << "res/sound/fire.wav not loaded." << endl;
	}

	sf::SoundBuffer playerHitBuffer;
	if (!playerHitBuffer.loadFromFile("res/sound/playerHit.wav")) {
		cout << "res/sound/playerHit.wav not loaded." << endl;
	}

	sf::SoundBuffer collisionHitBuffer;
	if (!collisionHitBuffer.loadFromFile("res/sound/enemyHit.wav")) {
		cout << "res/sound/enemyHit.wav not loaded." << endl;
	}

	sf::Sound soundShot;
	soundShot.setBuffer(shotBuffer);

	sf::Sound soundPlayerHit;
	soundPlayerHit.setBuffer(playerHitBuffer);

	sf::Sound soundCollisionHit;
	soundCollisionHit.setBuffer(collisionHitBuffer);

	//Texture
	sf::Texture textureEnvironment;
	if (!textureEnvironment.loadFromFile("res/img/environment.png")) {
		cout << "res/img/environment.png not loaded." << endl;
	}

	sf::Texture texturePlayer;
	if (!texturePlayer.loadFromFile("res/img/player.png")) {
		cout << "res/img/player.png not loaded." << endl;
	}

	sf::Texture textureEnemy;
	if (!textureEnemy.loadFromFile("res/img/enemy.png")) {
		cout << "res/img/enemy.png not loaded." << endl;
	}

	sf::Texture textureCoin;
	if (!textureCoin.loadFromFile("res/img/item.png")) {
		cout << "res/img/item.png not loaded." << endl;
	}

	sf::Texture textureFireball;
	if (!textureFireball.loadFromFile("res/img/fireball.png")) {
		cout << "res/img/fireball.png not loaded." << endl;
	}

	//Game objects
	Player player;
	player.sprite.setTexture(texturePlayer);

	vector<Projectile> projectileArray;
	Projectile projectile;
	projectile.sprite.setTexture(textureFireball);
	projectile.sprite.setScale(
		projectile.sprite.getScale().x / 2,
		projectile.sprite.getScale().y / 2
	);
	projectile.sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));

	vector<Enemy> enemyArray;
	Enemy enemy;
	enemy.sprite.setTexture(textureEnemy);
	enemy.sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
	enemy.text.setFont(font);
	enemy.text.setCharacterSize(8);
	enemy.text.setFillColor(sf::Color::Red);
	enemy.text.setString(to_string(enemy.hp) + "/" + to_string(enemy.maxHp));
	enemy.rect.setPosition(500,200);
	enemyArray.push_back(enemy);

	vector<TextDisplay> textDisplayArray;
	TextDisplay textDisplay;
	textDisplay.text.setFont(font);

	vector<Pickup> pickupArray;
	Pickup pickup;

	vector<Wall> wallArray;
	Wall wall;

	sf::Text coinShow("Coin: ", font, 25);
	coinShow.setFillColor(sf::Color::Yellow);
	sf::Text hpShow("Coin: ", font, 25);
	hpShow.setFillColor(sf::Color::Red);

	////// Create environment
	sf::Sprite environment(textureEnvironment);

	pickup.inShop = true;
	pickup.isPowerUp = true;
	pickup.isCoin = false;
	pickup.text.setFillColor(sf::Color::White);
	pickup.text.setFont(font);
	pickup.text.setString("Press F to buy for " + to_string(pickup.cost) + " coins");
	pickup.sprite.setTexture(textureCoin);
	pickup.sprite.setTextureRect(sf::IntRect(24 * 1.2, 24 * 6.8, 24, 24));
	pickup.rect.setPosition(500, 300);
	pickupArray.push_back(pickup);

	int roomSize = generateRandom(10) + 3;
	int verticalDoorLocation = generateRandom(roomSize);
	int roomStartX = generateRandom(200) + 100;
	int roomStartY = generateRandom(200) + 100;

	for (int w=0; w < roomSize; w++) {
		wall.rect.setPosition(50*w + roomStartX, roomStartY);
		wallArray.push_back(wall);
	}

	for (int w = 0; w < roomSize; w++) {
		wall.rect.setPosition(50 * w + roomStartX, 50*roomSize + roomStartY);
		wallArray.push_back(wall);
	}

	for (int w = 0; w < roomSize; w++) {
		wall.rect.setPosition(0 + roomStartX, 50 * w + roomStartY);
		wallArray.push_back(wall);
	}

	for (int w = 0; w < roomSize+1; w++) {
		if (w != verticalDoorLocation) {
			wall.rect.setPosition(50 * roomSize + roomStartX, 50 * w + roomStartY);
			wallArray.push_back(wall);
		}
	}

	while (window.isOpen()) {
		window.clear();
		view.setCenter(player.rect.getPosition());
		window.setView(view);
		window.draw(environment);

		//Player collide with wall
		for (vector<Wall>::iterator wIt = wallArray.begin(); wIt != wallArray.end(); wIt++) {
			if (player.rect.getGlobalBounds().intersects((*wIt).rect.getGlobalBounds())) {
				player.cantMoveDi = player.direction;
				sf::Vector2f bounce[4] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
				player.rect.move(bounce[player.direction - 1] * player.movementSpeed);
			}
		}

		//Projectile collide with wall
		for (vector<Projectile>::iterator pIt = projectileArray.begin(); pIt != projectileArray.end(); pIt++) {
			for (vector<Wall>::iterator wIt = wallArray.begin(); wIt != wallArray.end(); wIt++) {
				if ((*pIt).rect.getGlobalBounds().intersects((*wIt).rect.getGlobalBounds())) {
					if ((*wIt).destructable) {
						soundCollisionHit.play();
						(*wIt).hp -= (*pIt).attackDamage;
						if ((*wIt).hp <= 0) {
							(*wIt).destroyed = true;// destroy wall
						}
					}
					(*pIt).destroyed = true;
				}
			}
		}

		//Enemy collide with wall
		for (vector<Wall>::iterator wIt = wallArray.begin(); wIt != wallArray.end(); wIt++) {
			for (vector<Enemy>::iterator eIt = enemyArray.begin(); eIt != enemyArray.end(); eIt++) {
				if ((*eIt).rect.getGlobalBounds().intersects((*wIt).rect.getGlobalBounds())) {
					(*eIt).cantMoveDi = (*eIt).direction;
					sf::Vector2f bounce[4] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
					(*eIt).rect.move(bounce[(*eIt).direction - 1] * (*eIt).movementSpeed);
				}
			}
		}

		//Player Collide with pickup
		for (vector<Pickup>::iterator piIt = pickupArray.begin(); piIt != pickupArray.end(); piIt++) {
			if (player.rect.getGlobalBounds().intersects((*piIt).rect.getGlobalBounds())) {
				if (!(*piIt).inShop) {
					if ((*piIt).isCoin) {
						player.coin += (*piIt).coinValue;
						(*piIt).destroyed = true;
					}
					else if ((*piIt).isPowerUp) {
						player.powerUp = true;
						(*piIt).destroyed = true;
					}
				} else {
					// draw cost
					window.draw((*piIt).text);
					if ((*piIt).isPowerUp && player.coin >= (*piIt).cost 
						&& sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
						player.coin -= (*piIt).cost;
						player.powerUp = true;
						(*piIt).destroyed = true;
					}
				}
			}
		}

		// Enemy attack player
		sf::Time enemyAttackPlayer = enemyAttackPlayerClock.getElapsedTime();
		if (enemyAttackPlayer.asSeconds() >= 0.30f) {
			enemyAttackPlayerClock.restart();
			for (vector<Enemy>::iterator eIt = enemyArray.begin(); eIt != enemyArray.end(); eIt++) {
				if (player.rect.getGlobalBounds().intersects((*eIt).rect.getGlobalBounds())) {
					soundPlayerHit.play();

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

		//Check collision attack enemy
		for (vector<Projectile>::iterator pIt = projectileArray.begin(); pIt != projectileArray.end(); pIt++) {
			for (vector<Enemy>::iterator eIt = enemyArray.begin(); eIt != enemyArray.end(); eIt++) {
				if ((*pIt).rect.getGlobalBounds().intersects(
					(*eIt).rect.getGlobalBounds())) {
					(*eIt).isAggressive = true; //Set to chase player
					
					// Enemy got hit
					soundCollisionHit.play();	
					//Damage display
					textDisplay.text.setFillColor(sf::Color::Red);
					textDisplay.text.setString(to_string((*pIt).attackDamage));
					textDisplay.text.setPosition(
						(*eIt).rect.getPosition().x + (*eIt).rect.getSize().x / 2,
						(*eIt).rect.getPosition().y - (*eIt).rect.getSize().y / 2
					);
					textDisplayArray.push_back(textDisplay);

					//Update enemy health
					(*pIt).destroyed = true;
					(*eIt).hp -= (*pIt).attackDamage;
					(*eIt).text.setString(to_string((*eIt).hp) + "/" + to_string((*eIt).maxHp));
					if ((*eIt).hp <= 0) {
						(*eIt).alive = false;
					}
				}
			}
		}

		//Aggressive enemy
		sf::Time aggressiveEnemyTimer = aggressiveEnemyClock.getElapsedTime();
		for (vector<Enemy>::iterator eIt = enemyArray.begin(); eIt != enemyArray.end(); eIt++) {
			if ((*eIt).isAggressive) {
				if (aggressiveEnemyTimer.asSeconds() >= 1.0f) {
					aggressiveEnemyClock.restart();
					cout << (*eIt).direction << endl;
					if (abs(player.rect.getPosition().x - (*eIt).rect.getPosition().x) >=
						abs(player.rect.getPosition().y - (*eIt).rect.getPosition().y)) {
						if (player.rect.getPosition().x > (*eIt).rect.getPosition().x) {
							(*eIt).direction = 4;
							cout << "player is to the right" << endl;
						}
						else if (player.rect.getPosition().x < (*eIt).rect.getPosition().x) {
							(*eIt).direction = 3;
							cout << "player is to the left" << endl;
						}
					}
					else {
						if (player.rect.getPosition().y > (*eIt).rect.getPosition().y) {
							(*eIt).direction = 2;
							cout << "player is to the bottom" << endl;
						}
						else if (player.rect.getPosition().y < (*eIt).rect.getPosition().y) {
							(*eIt).direction = 1;
							cout << "player is to the top" << endl;
						}
					}
				}
			}
		}

		//Delete dead enemy
		for (vector<Enemy>::iterator eIt = enemyArray.begin(); eIt != enemyArray.end(); eIt++) {
			if ((*eIt).alive == false) {
				//random generate coin
				if (generateRandom(3) == 1) {
					pickup.inShop = false;
					pickup.isCoin = true;
					pickup.isPowerUp = false;
					pickup.sprite.setTexture(textureCoin);
					pickup.sprite.setTextureRect(sf::IntRect(24 * 6.8, 24 * 5.5, 24, 24));
					pickup.rect.setPosition((*eIt).rect.getPosition());
					pickupArray.push_back(pickup);
				}
				//random generate powerup
				if (generateRandom(3) == 1) {
					pickup.inShop = false;
					pickup.isPowerUp = true;
					pickup.isCoin = false;
					pickup.sprite.setTexture(textureCoin);
					pickup.sprite.setTextureRect(sf::IntRect(24 * 1.2, 24 * 6.8, 24, 24));
					pickup.rect.setPosition((*eIt).rect.getPosition());
					pickupArray.push_back(pickup);
				}
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

		//Delete Item
		for (vector<Pickup>::iterator piIt = pickupArray.begin(); piIt != pickupArray.end(); piIt++) {
			if ((*piIt).destroyed) {
				pickupArray.erase(piIt);
				break;
			}
		}

		//Delete Destructable wall
		for (vector<Wall>::iterator wIt = wallArray.begin(); wIt != wallArray.end(); wIt++) {
			if ((*wIt).destroyed) {
				//random generate coin
				if (generateRandom(3) == 1) {
					pickup.inShop = false;
					pickup.isCoin = true;
					pickup.isPowerUp = false;
					pickup.sprite.setTexture(textureCoin);
					pickup.sprite.setTextureRect(sf::IntRect(24 * 6.8, 24 * 5.5, 24, 24));
					pickup.rect.setPosition((*wIt).rect.getPosition());
					pickupArray.push_back(pickup);
				}
				//random generate powerup
				if (generateRandom(3) == 1) {
					pickup.inShop = false;
					pickup.isPowerUp = true;
					pickup.isCoin = false;
					pickup.sprite.setTexture(textureCoin);
					pickup.sprite.setTextureRect(sf::IntRect(24 * 1.2, 24 * 6.8, 24, 24));
					pickup.rect.setPosition((*wIt).rect.getPosition());
					pickupArray.push_back(pickup);
				}
				wallArray.erase(wIt);
				break;
			}
		}

		//Attack
		sf::Time checkAttack = attackClock.getElapsedTime();
		if (checkAttack.asSeconds() >= 0.15f) {
			attackClock.restart();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				soundShot.play();
				if (player.powerUp) {
					projectile.rect.setPosition(
						player.rect.getPosition().x + player.rect.getSize().x / 2 - projectile.rect.getSize().x / 2,
						player.rect.getPosition().y + player.rect.getSize().y / 2 - projectile.rect.getSize().y / 2
					);
					projectile.direction = player.direction;
					projectileArray.push_back(projectile);
					projectile.rect.setPosition(
						player.rect.getPosition().x + player.rect.getSize().x / 2 - projectile.rect.getSize().x / 2 + generateRandom(50),
						player.rect.getPosition().y + player.rect.getSize().y / 2 - projectile.rect.getSize().y / 2 + generateRandom(50)
					);
					projectile.direction = player.direction;
					projectileArray.push_back(projectile);
				}
				else {
					projectile.rect.setPosition(
						player.rect.getPosition().x + player.rect.getSize().x / 2 - projectile.rect.getSize().x / 2,
						player.rect.getPosition().y + player.rect.getSize().y / 2 - projectile.rect.getSize().y / 2
					);
					projectile.direction = player.direction;
					projectileArray.push_back(projectile);
				}
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

		// Draw wall
		for (vector<Wall>::iterator wIt = wallArray.begin(); wIt != wallArray.end(); wIt++) {
			window.draw((*wIt).rect);
		}

		// Draw Pickup item
		for (vector<Pickup>::iterator piIt = pickupArray.begin(); piIt != pickupArray.end(); piIt++) {
			(*piIt).update();
			window.draw((*piIt).sprite);
		}

		//Update Bullet
		for (vector<Projectile>::iterator pIt = projectileArray.begin(); pIt != projectileArray.end(); pIt++) {
			(*pIt).update();
			window.draw((*pIt).sprite);
			//window.draw((*pIt).rect);
		}

		//Update Enemy
		for (vector<Enemy>::iterator eIt = enemyArray.begin(); eIt != enemyArray.end(); eIt++) {
			(*eIt).update();
			(*eIt).updateMovement();
			window.draw((*eIt).text);
			window.draw((*eIt).rect);
			window.draw((*eIt).sprite);
		}

		//drawing coin
		coinShow.setString("Coin : " + to_string(player.coin));
		coinShow.setPosition(player.rect.getPosition().x - window.getSize().x/2, player.rect.getPosition().y - window.getSize().y / 2);
		window.draw(coinShow);
		//drawing player hp
		hpShow.setString("HP : " + to_string(player.hp) + "/" + to_string(player.maxHp));
		hpShow.setPosition(player.rect.getPosition().x - window.getSize().x / 2, player.rect.getPosition().y - window.getSize().y / 2 + 24);
		window.draw(hpShow);

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
