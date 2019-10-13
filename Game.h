#pragma once
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

class Game {
public:
	Game();
	void run();
	void update();
	void render();
	void inputProcess();
	void clearJunk();
	void collisionRelated();
	void enemyRelated();
	void playerAttack();
	void itemRelated();
	void setStandPos();

	bool isUsingStand = false;

private:
	sf::Clock attackClock, enemyAttackPlayerClock, aggressiveEnemyClock, switchStandClock;
	sf::RenderWindow window;
	sf::View view;

	//Resources
	sf::Font font;
	sf::Music music;

	sf::SoundBuffer shotBuffer;
	sf::SoundBuffer playerHitBuffer;
	sf::SoundBuffer collisionHitBuffer;

	sf::Sound soundShot;
	sf::Sound soundPlayerHit;
	sf::Sound soundCollisionHit;

	sf::Texture textureEnvironment;
	sf::Texture texturePlayer;
	sf::Texture textureEnemy;
	sf::Texture textureCoin;
	sf::Texture textureFireball;

	//Gameobjects
	Player player;
	Player stand;

	vector<Projectile> projectileArray;
	Projectile projectile;

	vector<Enemy> enemyArray;
	Enemy enemy;

	vector<TextDisplay> textDisplayArray;
	TextDisplay textDisplay;

	vector<Pickup> pickupArray;
	Pickup pickup;

	vector<Wall> wallArray;
	Wall wall;

	// UI related
	sf::Text coinShow;
	sf::Text hpShow;
	sf::Text isUsingStandShow;
};
