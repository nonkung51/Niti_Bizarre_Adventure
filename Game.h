#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <vector>

#include "Player.h"
#include "Entity.h"
#include "Npc.h"
#include "Projectile.h"
#include "Enemy.h"
#include "TextDisplay.h"
#include "Pickup.h"
#include "Wall.h"
#include "DialogBox.h"
#include "WorldGenerator.h"
#include "helper.h"


class Game {
public:
	Game(sf::RenderWindow*, bool*, int*, int*);
	void run();
	void update();
	void render();
	void inputProcess();
	void clearJunk();
	void collisionRelated();
	void enemyRelated();
	void playerAttack();
	void itemRelated();
	void generateMap();
	void generateGameObjects();
	void remap();
	void dialogUpdate();
	void stageDialog();

	bool isUsingStand = false;
	sf::RenderWindow *window;
	int* state;
	int* playingScore = 0;
	int currentStage = 1;

private:
	sf::Clock attackClock, enemyAttackPlayerClock, 
		aggressiveEnemyClock, switchStandClock,
		dialogClock, loadingClock, shopClock, trapClock;
	sf::View view;

	//Resources
	sf::Font font;
	sf::Music music;

	sf::SoundBuffer shotBuffer;
	sf::SoundBuffer playerHitBuffer;
	sf::SoundBuffer collisionHitBuffer;
	sf::SoundBuffer yareYareBuffer;

	sf::Sound soundShot;
	sf::Sound soundPlayerHit;
	sf::Sound soundCollisionHit;
	sf::Sound yareYare;

	sf::Texture textureEnvironment;
	sf::Texture texturePlayer;
	sf::Texture textureStand;
	sf::Texture textureEnemy;
	sf::Texture textureCoin;
	sf::Texture textureFireball;
	sf::Texture textureMerchant;
	sf::Texture textureLoading;
	sf::Texture textureShader;
	sf::Texture textureBlood;

	sf::Sprite shaderSprite;
	sf::Sprite bloodSprite;

	//Gameobjects
	Player player;
	Player stand;

	vector<Projectile> projectileArray;
	Projectile projectile;

	vector<Enemy> enemyType = { Enemy(), Enemy(), Enemy(), Enemy() };
	vector<sf::Texture> enemyTypeTexture{ sf::Texture() , sf::Texture() , sf::Texture() , sf::Texture() }; // 4 type of enemy
	vector<Enemy> enemyArray;
	Enemy enemy;

	vector<TextDisplay> textDisplayArray;
	TextDisplay textDisplay;

	vector<Pickup> pickupArray;
	Pickup pickup;

	vector<Wall> wallArray;
	Wall wall;
	WorldGenerator world;
	sf::Texture tiles;
	sf::Sprite mapDrawer;
	vector <sf::Sprite> mapSprite;


	// UI related
	sf::Text currentStageShow;
	sf::Text extraShow;
	sf::Text hpShow;
	sf::Text isUsingStandShow;
	sf::Vector2f focusPoint;

	DialogBox dialogBox;

	// Npc
	Npc activeNpc;
	vector<Npc> npcArray = { Npc(), Npc(), Npc(), Npc() };
	vector<sf::Texture> npcTexture = { sf::Texture(), sf::Texture(), sf::Texture(), sf::Texture() };

	//connect with menu
	bool* isPlaying;

	void enemySetup();
	void npcSetup();
	void renderLoading();

	int enemyKilled = 0;
};
