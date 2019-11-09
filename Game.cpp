#include "Game.h"
#include "DialogBox.h"
#include <iostream>

int heightToTile(int h) {
	//mountain
	if (h <= 15) {
		return 0;
	}

	// grass
	if (h > 15 && h < 80) {
		return 1;
	}

	// sand
	if (h >= 80) {
		return 2;
	}
}

Game::Game(sf::RenderWindow* w, bool* playing, int* score, int* st) : view(sf::FloatRect(200, 200, 300, 200)), dialogBox() {
	window = w;
	isPlaying = playing;
	playingScore = score;
	state = st;
	window->setFramerateLimit(60);

	view.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	view.setCenter(sf::Vector2f(view.getSize().x / 2, view.getSize().y / 2));
	window->setView(view);

	srand(time(NULL));

	if (!font.loadFromFile("res/font/manaspc.ttf")) {
		cout << "Can\'t load font" << endl;
	}

	if (!music.openFromFile("res/music/Theme.ogg")) {
		cout << "res/music/Theme.ogg not loaded." << endl;
	}

	if (!shotBuffer.loadFromFile("res/sound/oraora.wav")) {
		cout << "res/sound/oraora.wav not loaded." << endl;
	}

	if (!playerHitBuffer.loadFromFile("res/sound/playerHit.wav")) {
		cout << "res/sound/playerHit.wav not loaded." << endl;
	}

	if (!collisionHitBuffer.loadFromFile("res/sound/enemyHit.wav")) {
		cout << "res/sound/enemyHit.wav not loaded." << endl;
	}

	if (!yareYareBuffer.loadFromFile("res/sound/yare-yare.wav")) {
		cout << "res/sound/enemyHit.wav not loaded." << endl;
	}

	soundShot.setBuffer(shotBuffer);
	soundPlayerHit.setBuffer(playerHitBuffer);
	soundCollisionHit.setBuffer(collisionHitBuffer);
	yareYare.setBuffer(yareYareBuffer);

	// theme music
	if (!music.openFromFile("res/music/Theme.ogg")) {
		cout << "res/music/Theme.ogg not loaded." << endl;
	}
	music.setLoop(true);
	//music.play();

	if (!textureEnvironment.loadFromFile("res/img/environment.png")) {
		cout << "res/img/environment.png not loaded." << endl;
	}

	if (!texturePlayer.loadFromFile("res/img/niti.png")) {
		cout << "res/img/player.png not loaded." << endl;
	}

	if (!textureStand.loadFromFile("res/img/stand.png")) {
		cout << "res/img/stand.png not loaded." << endl;
	}

	if (!textureEnemy.loadFromFile("res/img/enemy2.png")) {
		cout << "res/img/enemy2.png not loaded." << endl;
	}

	if (!textureCoin.loadFromFile("res/img/item.png")) {
		cout << "res/img/item.png not loaded." << endl;
	}

	if (!textureFireball.loadFromFile("res/img/fireball.png")) {
		cout << "res/img/fireball.png not loaded." << endl;
	}

	if (!textureMerchant.loadFromFile("res/img/merchant.png")) {
		cout << "res/img/merchant.png not loaded." << endl;
	}

	if (!textureLoading.loadFromFile("res/img/loading.png")) {
		cout << "res/img/loading.png not loaded." << endl;
	}

	player.sprite.setTexture(texturePlayer);
	stand.sprite.setTexture(textureStand);
	stand.sprite.setColor(sf::Color(255, 255, 255, 100));

	player.sprite.setTextureRect(sf::IntRect(0, texturePlayer.getSize().y / 21 * 10,
		texturePlayer.getSize().x / 13, texturePlayer.getSize().y / 21));
	stand.sprite.setTextureRect(sf::IntRect(0, texturePlayer.getSize().y / 21 * 10,
		texturePlayer.getSize().x / 13, texturePlayer.getSize().y / 21));

	//player.rect.setSize(sf::Vector2f(texturePlayer.getSize()));

	pickup.text.setFont(font);
	pickup.text.setCharacterSize(8);
	pickup.text.setFillColor(sf::Color::Yellow);
	pickup.text.setString("");

	projectile.sprite.setTexture(textureFireball);
	projectile.sprite.setScale(
		projectile.sprite.getScale().x / 2,
		projectile.sprite.getScale().y / 2
	);
	projectile.sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));

	enemy.sprite.setTexture(textureEnemy);
	enemy.sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
	enemy.text.setFont(font);
	enemy.text.setCharacterSize(8);
	enemy.text.setFillColor(sf::Color::Red);
	enemy.text.setString(to_string(enemy.hp) + "/" + to_string(enemy.maxHp));
	enemy.rect.setPosition(500, 200);
	//enemyArray.push_back(enemy);

	textDisplay.text.setFont(font);

	currentStageShow.setFont(font);
	currentStageShow.setCharacterSize(25);
	extraShow.setFont(font);
	extraShow.setCharacterSize(25);
	hpShow.setFont(font);
	hpShow.setCharacterSize(25);
	isUsingStandShow.setFont(font);
	isUsingStandShow.setCharacterSize(25);

	dialogBox.box.setPosition(0, 2 * window->getSize().y / 3);
	dialogBox.box.setSize(sf::Vector2f(window->getSize().x, window->getSize().y / 3));

	dialogBox.text.setFont(font);
	dialogBox.text.setPosition(20, ((2 * window->getSize().y) / 3) + 20);
	dialogBox.conversation = vector<string>{ "this is first sentence", "And this is second one" };
	dialogBox.text.setString(dialogBox.conversation.at(0));
	//dialogBox.update();
	
	//World sprite
	if (!tiles.loadFromFile("res/img/tiles.png")) {
		cout << "res/img/tiles.png not loaded." << endl;
	}

	// world
	world.CreateMatrix();
	world.Interpolation();
	//world.CreateGraphics();
	generateMap();

	// Enemy Type
	enemySetup();
	//Npc
	npcSetup();
	generateGameObjects();

	// Stage dialog
	stageDialog();
}


void Game::generateMap() {
	mapDrawer.setTexture(tiles);
	for (int y = 0; y < 65; y++) {
		for (int x = 0; x < 65; x++) {
			// mountain (set wall so people can't pass through)
			if (heightToTile(world._Matrix[y][x]) == 0) {
				mapDrawer.setTextureRect(sf::IntRect(5 * 16, 16 * 2, 16, 16));
				/////
				if (heightToTile(world._Matrix[y][x - 1]) == 1 && heightToTile(world._Matrix[y - 1][x]) == 1 && heightToTile(world._Matrix[y][x + 1]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(17 * 16, 16 * 5, 16, 16));
				}
				else if (heightToTile(world._Matrix[y][x - 1]) == 1 && heightToTile(world._Matrix[y][x + 1]) == 1
					&& heightToTile(world._Matrix[y + 1][x]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(17 * 16, 16 * 7, 16, 16));
				}
				else if (heightToTile(world._Matrix[y][x - 1]) == 1 && heightToTile(world._Matrix[y][x + 1]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(17 * 16, 16 * 6, 16, 16));
				}
				///
				else if (heightToTile(world._Matrix[y][x - 1]) == 1 && heightToTile(world._Matrix[y + 1][x]) == 1
					&& heightToTile(world._Matrix[y - 1][x]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(14 * 16, 16 * 5, 16, 16));
				}
				else if (heightToTile(world._Matrix[y][x + 1]) == 1 && heightToTile(world._Matrix[y + 1][x]) == 1
					&& heightToTile(world._Matrix[y - 1][x]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(16 * 16, 16 * 5, 16, 16));
				}
				else if (heightToTile(world._Matrix[y + 1][x]) == 1 && heightToTile(world._Matrix[y - 1][x]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(15 * 16, 16 * 5, 16, 16));
				}
				///
				else if (heightToTile(world._Matrix[y][x - 1]) == 1 && heightToTile(world._Matrix[y - 1][x]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(4 * 16, 16 * 1, 16, 16));
				}
				else if (heightToTile(world._Matrix[y - 1][x]) == 1 && heightToTile(world._Matrix[y][x + 1]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(6 * 16, 16 * 1, 16, 16));
				}
				else if (heightToTile(world._Matrix[y + 1][x]) == 1 && heightToTile(world._Matrix[y][x - 1]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(4 * 16, 16 * 3, 16, 16));
				}
				else if (heightToTile(world._Matrix[y + 1][x]) == 1 && heightToTile(world._Matrix[y][x + 1]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(6 * 16, 16 * 3, 16, 16));
				}//
				else if (heightToTile(world._Matrix[y][x - 1]) != 0) {
					mapDrawer.setTextureRect(sf::IntRect(4 * 16, 16 * 2, 16, 16));
				}
				else if (heightToTile(world._Matrix[y][x + 1]) != 0) {
					mapDrawer.setTextureRect(sf::IntRect(6 * 16, 16 * 2, 16, 16));
				}
				else if (heightToTile(world._Matrix[y + 1][x]) != 0) {
					mapDrawer.setTextureRect(sf::IntRect(5 * 16, 16 * 3, 16, 16));
				}
				else if (heightToTile(world._Matrix[y - 1][x]) != 0) {
					mapDrawer.setTextureRect(sf::IntRect(5 * 16, 16 * 1, 16, 16));
				}
				/////
				mapDrawer.setPosition(sf::Vector2f(x * 64, y * 64));
				mapDrawer.setScale(4.0f, 4.0f);
				mapSprite.push_back(mapDrawer);
				if (heightToTile(world._Matrix[y - 1][x - 1]) == 1 
					|| heightToTile(world._Matrix[y - 1][x]) == 1
					|| heightToTile(world._Matrix[y - 1][x + 1]) == 1
					|| heightToTile(world._Matrix[y][x - 1]) == 1
					|| heightToTile(world._Matrix[y][x + 1]) == 1
					|| heightToTile(world._Matrix[y + 1][x - 1]) == 1
					|| heightToTile(world._Matrix[y + 1][x]) == 1
					|| heightToTile(world._Matrix[y + 1][x +1]) == 1) {
					Wall wall;
					wall.rect.setPosition(sf::Vector2f(x * 64, y * 64));
					wallArray.push_back(wall);
				}

			}

			// grass
			if (heightToTile(world._Matrix[y][x]) == 1) {
				int generateGrass = generateRandom(25);
				mapDrawer.setTextureRect(sf::IntRect(5 * 16, 16 * 2, 16, 16));
				if(generateGrass == 1) mapDrawer.setTextureRect(sf::IntRect(7 * 16, 16 * 0, 16, 16));
				else if (generateGrass == 2) mapDrawer.setTextureRect(sf::IntRect(7 * 16, 16 * 0, 16, 16));
				else if (generateGrass == 3) mapDrawer.setTextureRect(sf::IntRect(7 * 16, 16 * 1, 16, 16));
				else if (generateGrass == 4) mapDrawer.setTextureRect(sf::IntRect(8 * 16, 16 * 0, 16, 16));
				else if (generateGrass == 5) mapDrawer.setTextureRect(sf::IntRect(10 * 16, 16 * 6, 16, 16));
				else if (generateGrass == 6) mapDrawer.setTextureRect(sf::IntRect(9 * 16, 16 * 6, 16, 16));
				mapDrawer.setPosition(sf::Vector2f(x * 64, y * 64));
				mapDrawer.setScale(4.0f, 4.0f);
				mapSprite.push_back(mapDrawer);
			}

			// sand
			if (heightToTile(world._Matrix[y][x]) == 2) {
				mapDrawer.setTextureRect(sf::IntRect(1 * 16, 5 * 16, 16, 16));
				if (heightToTile(world._Matrix[y][x - 1]) == 1 && heightToTile(world._Matrix[y - 1][x]) == 1 
					&& heightToTile(world._Matrix[y][x + 1]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(18 * 16, 16 * 5, 16, 16));
				}
				else if (heightToTile(world._Matrix[y][x - 1]) == 1 && heightToTile(world._Matrix[y][x + 1]) == 1
					&& heightToTile(world._Matrix[y + 1][x]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(18 * 16, 16 * 7, 16, 16));
				}
				else if (heightToTile(world._Matrix[y][x - 1]) == 1 && heightToTile(world._Matrix[y][x + 1]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(18 * 16, 16 * 6, 16, 16));
				}
				///
				else if (heightToTile(world._Matrix[y][x - 1]) == 1 && heightToTile(world._Matrix[y + 1][x]) == 1
					&& heightToTile(world._Matrix[y - 1][x]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(14 * 16, 16 * 6, 16, 16));
				}
				else if (heightToTile(world._Matrix[y][x + 1]) == 1 && heightToTile(world._Matrix[y + 1][x]) == 1
					&& heightToTile(world._Matrix[y - 1][x]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(16 * 16, 16 * 6, 16, 16));
				}
				else if (heightToTile(world._Matrix[y + 1][x]) == 1 && heightToTile(world._Matrix[y - 1][x]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(15 * 16, 16 * 6, 16, 16));
				}
				///
				else if (heightToTile(world._Matrix[y][x - 1]) == 1 && heightToTile(world._Matrix[y - 1][x]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(0 * 16, 16 * 4, 16, 16));
				}
				else if (heightToTile(world._Matrix[y - 1][x]) == 1 && heightToTile(world._Matrix[y][x + 1]) == 2) {
					mapDrawer.setTextureRect(sf::IntRect(1 * 16, 16 * 4, 16, 16));
				}
				else if (heightToTile(world._Matrix[y - 1][x]) == 1 && heightToTile(world._Matrix[y][x + 1]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(2 * 16, 16 * 4, 16, 16));
				}
				else if (heightToTile(world._Matrix[y + 1][x]) == 1 && heightToTile(world._Matrix[y][x - 1]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(0 * 16, 16 * 6, 16, 16));
				}
				else if (heightToTile(world._Matrix[y + 1][x]) == 1 && heightToTile(world._Matrix[y][x + 1]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(2 * 16, 16 * 6, 16, 16));
				}
				else if (heightToTile(world._Matrix[y][x - 1]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(0 * 16, 16 * 5, 16, 16));
				}
				else if (heightToTile(world._Matrix[y][x + 1]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(2 * 16, 16 * 5, 16, 16));
				}
				else if (heightToTile(world._Matrix[y + 1][x]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(1 * 16, 16 * 6, 16, 16));
				}
				mapDrawer.setPosition(sf::Vector2f(x * 64, y * 64));
				mapDrawer.setScale(4.0f, 4.0f);
				mapSprite.push_back(mapDrawer);
			}
		}
	}
}

void Game::generateGameObjects() {
	Wall tree;
	tree.rect.setSize(sf::Vector2f(64, 96));
	tree.sprite.setTexture(tiles);
	tree.sprite.setTextureRect(sf::IntRect(6 * 16, 5 * 16, 32, 32));
	tree.sprite.setScale(sf::Vector2f(4.f, 4.f));
	Wall boulder;
	boulder.sprite.setTexture(tiles);
	boulder.sprite.setTextureRect(sf::IntRect(9 * 16, 5 * 16, 16, 16));
	boulder.sprite.setScale(sf::Vector2f(4.f, 4.f));
	Wall poisonBush;
	poisonBush.sprite.setTexture(tiles);
	poisonBush.sprite.setTextureRect(sf::IntRect(10 * 16, 7 * 16, 16, 16));
	poisonBush.sprite.setScale(sf::Vector2f(4.f, 4.f));
	poisonBush.trap = true;

	for (int x = 0, curObs = 0; x < 65 && curObs < 100; x++) {
		for (int y = 0; y < 65; y++) {
			bool gen = generateRandom(10) > 7;
			if (gen && heightToTile(world._Matrix[y][x]) == 1) {
				curObs++;
				int gObs = generateRandom(10);
				if (gObs > 4) {
					tree.rect.setPosition(x * 64 + 32, y * 64 + 32);
					tree.sprite.setPosition(x * 64, y * 64);
					wallArray.push_back(tree);
				}
				else if (gObs == 4) {
					poisonBush.rect.setPosition(x * 64, y * 64);
					poisonBush.sprite.setPosition(x * 64, y * 64);
					wallArray.push_back(poisonBush);
				}
				else {
					boulder.rect.setPosition(x * 64, y * 64);
					boulder.sprite.setPosition(x * 64, y * 64);
					wallArray.push_back(boulder);
				}
				x++; y++; // so object not spawn too close
			}
		}
	}
	//Only spawn player on sand tiles
	bool flag = true;
	int playerX;
	int playerY;
	while (flag) {
		playerX = generateRandom0(65);
		playerY = generateRandom0(65);
		flag = false;
		if (heightToTile(world._Matrix[playerY][playerX]) != 1) {
			flag = true;
		}
	}

	player.rect.setPosition(playerX * 64, playerY * 64);

	int enemyX;
	int enemyY;
	//gen enemy
	for (int i = 0; i < currentStage * 10; i++) {
		bool flagEnemy = true;
		while (flagEnemy) {
			enemyX = generateRandom0(65);
			enemyY = generateRandom0(65);
			flagEnemy = false;
			if (heightToTile(world._Matrix[enemyY][enemyX]) != 1 || distance(enemyX, enemyY,playerX,playerY) <= 5.f) {
				flagEnemy = true;
			}
		}
		int type = generateRandom0(currentStage); //generate monster according to current stage
		enemyType[type].rect.setPosition(enemyX * 64, enemyY * 64);
		enemyArray.push_back(enemyType[type]);
	}

	int stepLength = 1, step = 0;
	int bossX = playerX;
	int bossY = playerY;
	while (distance(playerX, playerY, bossX, bossY) < 25.f && step < 1500) {
		int di = generateRandom(4);
		if (di == 1 && bossY > 0) {
			bossY -= stepLength;
		}
		else if (di == 2 && bossY < 65) {
			bossY += stepLength;
		}
		else if (di == 3 && bossX > 0) {
			bossX -= stepLength;
		}
		else if (di == 4 && bossX < 65) {
			bossX += stepLength;
		}
		activeNpc.rect.setPosition(bossX * 64, bossY * 64);
		for (vector<Wall>::iterator wIt = wallArray.begin(); wIt != wallArray.end(); wIt++) {
			int bounce[4][2] = { {0,1}, {0,-1}, {1,0}, {-1,0} };
			if (activeNpc.rect.getGlobalBounds().intersects(wIt->rect.getGlobalBounds())) {
				activeNpc.rect.setPosition(bossX * 64 + bounce[di - 1][0] * stepLength * 70
					, bossY * 64 + bounce[di - 1][1] * stepLength * 70);
				bossX = bossX + bounce[di - 1][0] * stepLength;
				bossY = bossY + bounce[di - 1][1] * stepLength;
			}
		}
		activeNpc.update();
		step++;
	}

	// Shop
	pickup.inShop = true;
	pickup.isCoin = false;
	pickup.text.setString("Press F to by Potion\nfor 5 coin");
	pickup.isPowerUp = true;
	pickup.rect.setSize(sf::Vector2f(64, 64));
	pickup.sprite.setTexture(textureMerchant);
	pickup.sprite.setTextureRect(sf::IntRect(0, 128, 64, 64));
	/* set shop position */
	stepLength = 1, step = 0;
	int shopX = playerX;
	int shopY = playerY;
	while (distance(playerX, playerY, shopX, shopY) < 25.f && step < 1500) {
		int di = generateRandom(4);
		if (di == 1 && shopY > 0) {
			shopY -= stepLength;
		}
		else if (di == 2 && shopY < 65) {
			shopY += stepLength;
		}
		else if (di == 3 && shopX > 0) {
			shopX -= stepLength;
		}
		else if (di == 4 && shopX < 65) {
			shopX += stepLength;
		}
		pickup.rect.setPosition(shopX * 64, shopY * 64);
		for (vector<Wall>::iterator wIt = wallArray.begin(); wIt != wallArray.end(); wIt++) {
			int bounce[4][2] = { {0,1}, {0,-1}, {1,0}, {-1,0} };
			if (pickup.rect.getGlobalBounds().intersects(wIt->rect.getGlobalBounds())) {
				pickup.rect.setPosition(shopX * 64 + bounce[di - 1][0] * stepLength * 70
					, shopY * 64 + bounce[di - 1][1] * stepLength * 70);
				shopX = shopX + bounce[di - 1][0] * stepLength;
				shopY = shopY + bounce[di - 1][1] * stepLength;
			}
		}
		activeNpc.update();
		step++;
	}
	pickupArray.push_back(pickup);
}

void Game::dialogUpdate() {
	//When first met
	if (dialogBox.finish && dialogBox.active == activeNpc.name && dialogBox.conversation == activeNpc.conversation) {
		activeNpc.stand.rect.setPosition(activeNpc.rect.getPosition());
		activeNpc.stand.isAggressive = true;
		enemyArray.push_back(activeNpc.stand);
		dialogBox.conversation = {};
		dialogBox.active = "";
	}
	//after kill their stand
	if (dialogBox.finish && dialogBox.active == activeNpc.name && dialogBox.conversation == activeNpc.endConversation && dialogBox.conversation.size() > 0) {
		dialogBox.conversation = {};
		dialogBox.active = "";
		currentStage++;
		remap();
	}
	// stage dialog
	if (dialogBox.finish && dialogBox.active == "STAGE_CHANGE") {
		dialogBox.conversation = {};
		dialogBox.active = "";
	}
}

void Game::stageDialog() {
	vector<vector<string>> dialog = {
		{
			"You\'re so deseperate.\nYour father got killed in his own home.\nYour family treasure stolen.",
			"But it\'s not time to give up yet!\nyou find your new ability called stand!!",
			"Use it wisely, get your family honor\nback from the one who stole it!!"
		},
		{
			"You manage to kill Hibari, his baton was damn hurt",
			"But you still have to continue your journey.",
			"You\'ve heard the news there\'s stand user nearby. \nYou don\'t know his abilities \nbut It\'s not a bad idea to visit.",
			"Will he be friendly??"
		},
		{
			"With help of Karz",
			"You continue your journey. By going far north",
			"You know there\'s another stand user.",
			"And another thing you know is that.",
			"They\'re not friendly at all...."
		},
		{
			"After beat Orky the orc.",
			"You found a note in his bag.",
			"The Bald woman from the far North.",
			"Why everyone have to come from the North?",
			"You asked yourself.",
			"But still, Journey have to be continue."
		},
	};
	if (currentStage == 4) {
		return;
	}
	dialogBox.setDialog(dialog[currentStage - 1], "STAGE_CHANGE");
}

void Game::remap() {
	if (currentStage > 4) { //game ending
		*isPlaying = false;
		*state = 2;
		return;
	}
	window->clear();
	/*
		render loading here
	*/
	view.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	view.setCenter(sf::Vector2f(view.getSize().x / 2, view.getSize().y / 2));
	window->setView(view);
	sf::Sprite loadingSprite;
	loadingSprite.setPosition(10, 600);
	loadingSprite.setTexture(textureLoading);
	loadingSprite.setScale({ 0.3,0.3 });
	window->draw(loadingSprite);

	window->display();

	isUsingStand = false;
	player.update();
	player.updateMovement(false);
	dialogBox.isShow = false;
	/*view.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	view.setCenter(sf::Vector2f(view.getSize().x / 2, view.getSize().y / 2));
	window->setView(view);*/
	
	player.sprite.setTextureRect(sf::IntRect(0, texturePlayer.getSize().y / 21 * 10,
		texturePlayer.getSize().x / 13, texturePlayer.getSize().y / 21));
	stand.sprite.setTextureRect(sf::IntRect(0, texturePlayer.getSize().y / 21 * 10,
		texturePlayer.getSize().x / 13, texturePlayer.getSize().y / 21));

	player.hp = player.maxHp;
	
	enemyArray.clear();
	projectileArray.clear();
	textDisplayArray.clear();
	pickupArray.clear();
	wallArray.clear();
	mapSprite.clear();

	world.CreateMatrix();
	world.Interpolation();
	generateMap();
	//Npc
	npcSetup();

	generateGameObjects();
	// generate dialog for certain stage
	stageDialog();

	enemyKilled = 0;
}

void Game::enemySetup() {
	enemyTypeTexture[0].loadFromFile("res/img/enemy2.png");
	enemyTypeTexture[1].loadFromFile("res/img/enemy3.png");
	enemyTypeTexture[2].loadFromFile("res/img/enemy4.png");
	enemyTypeTexture[3].loadFromFile("res/img/enemy5.png");

	enemyType[0].sprite.setTexture(enemyTypeTexture[0]);
	enemyType[0].text.setFont(font);
	enemyType[0].text.setCharacterSize(8);
	enemyType[0].text.setFillColor(sf::Color::Red);
	enemyType[0].text.setString(to_string(enemyType[0].hp) + "/" + to_string(enemyType[0].maxHp));

	enemyType[1].sprite.setTexture(enemyTypeTexture[1]);
	enemyType[1].maxHp = 7;
	enemyType[1].hp = 7;
	enemyType[1].text.setFont(font);
	enemyType[1].text.setCharacterSize(8);
	enemyType[1].text.setFillColor(sf::Color::Red);
	enemyType[1].text.setString(to_string(enemyType[1].hp) + "/" + to_string(enemyType[1].maxHp));

	enemyType[2].sprite.setTexture(enemyTypeTexture[2]);
	enemyType[2].maxHp = 10;
	enemyType[2].hp = 10;
	enemyType[2].text.setFont(font);
	enemyType[2].text.setCharacterSize(8);
	enemyType[2].text.setFillColor(sf::Color::Red);
	enemyType[2].text.setString(to_string(enemyType[2].hp) + "/" + to_string(enemyType[2].maxHp));

	enemyType[3].sprite.setTexture(enemyTypeTexture[3]);
	enemyType[3].maxHp = 15;
	enemyType[3].hp = 15;
	enemyType[3].text.setFont(font);
	enemyType[3].text.setCharacterSize(8);
	enemyType[3].text.setFillColor(sf::Color::Red);
	enemyType[3].text.setString(to_string(enemyType[3].hp) + "/" + to_string(enemyType[3].maxHp));
}

void Game::npcSetup() {
	//////////////////////// hibari
	if (!npcTexture.at(0).loadFromFile("res/img/npc.png")) {
		cout << "res/img/npc.png not loaded." << endl;
	}
	npcArray.at(0).name = "Hibari";
	npcArray.at(0).conversation = vector<string>{ "What are you doing?", "Go away!!", "!!" };
	npcArray.at(0).endConversation = vector<string>{ "You killed my stand!!", "You killed my stand!! Damn you!" };
	npcArray.at(0).stand = enemyType[0];
	npcArray.at(0).stand.sprite.setColor(sf::Color(255, 255, 255, 100));
	npcArray.at(0).stand.hp = 10;
	npcArray.at(0).stand.maxHp = 10;
	npcArray.at(0).stand.text.setString("10/10");
	npcArray.at(0).stand.owner = npcArray.at(0).name;
	npcArray.at(0).sprite.setTexture(npcTexture.at(0));
	int spriteSizeX = npcArray.at(0).sprite.getTexture()->getSize().x / 13;
	int spriteSizeY = npcArray.at(0).sprite.getTexture()->getSize().y / 21;
	npcArray.at(0).sprite.setTextureRect(sf::IntRect(0, 2 * spriteSizeY, spriteSizeX, spriteSizeY));
	npcArray.at(0).rect.setPosition({ player.rect.getPosition().x - 64 * 8, player.rect.getPosition().y - 64 * 7 });
	npcArray.at(0).update();
	///////////////////////// karz
	if (!npcTexture.at(1).loadFromFile("res/img/npc2.png")) {
		cout << "res/img/npc2.png not loaded." << endl;
	}
	npcArray.at(1).name = "Karz";
	npcArray.at(1).conversation = vector<string>{ "You kill hibari??", "Damn I don\'t trust you!!", "But fine let\'s fight." };
	npcArray.at(1).endConversation = vector<string>{ "Ahhhh!!", "I am honor warrior", 
		"Fine I\'ll tell you where the jewelry is", "Going north, You\'ll find another stand user.", "Ask him!"};
	npcArray.at(1).stand = enemyType[1];
	npcArray.at(1).stand.sprite.setColor(sf::Color(255, 255, 255, 100));
	npcArray.at(1).stand.hp = 20;
	npcArray.at(1).stand.maxHp = 20;
	npcArray.at(1).stand.text.setString("20/20");
	npcArray.at(1).stand.owner = npcArray.at(1).name;
	npcArray.at(1).sprite.setTexture(npcTexture.at(1));
	npcArray.at(1).sprite.setTextureRect(sf::IntRect(0, 2 * spriteSizeY, spriteSizeX, spriteSizeY));
	npcArray.at(1).rect.setPosition({ player.rect.getPosition().x - 64 * 8, player.rect.getPosition().y - 64 * 7 });
	npcArray.at(1).update();
	
	if (!npcTexture.at(2).loadFromFile("res/img/npc4.png")) {
		cout << "res/img/npc4.png not loaded." << endl;
	}
	npcArray.at(2).name = "Orky";
	npcArray.at(2).conversation = vector<string>{ "My name is Orky.", "Orky the Orc!", 
		"Have you ever heard of it?", "Oh you talking about that jewelry?", "Haha it\'s with my boss", "Beat me! Idiot!" };
	npcArray.at(2).endConversation = vector<string>{ "You killed my stand!! Damn you!", "Go away before I call my boss!!" };
	npcArray.at(2).stand = enemyType[2];
	npcArray.at(2).stand.sprite.setColor(sf::Color(255, 255, 255, 100));
	npcArray.at(2).stand.hp = 25;
	npcArray.at(2).stand.maxHp = 25;
	npcArray.at(2).stand.text.setString("25/25");
	npcArray.at(2).stand.owner = npcArray.at(2).name;
	npcArray.at(2).sprite.setTexture(npcTexture.at(2));
	npcArray.at(2).sprite.setTextureRect(sf::IntRect(1 * spriteSizeX, 18 * spriteSizeY, spriteSizeX, spriteSizeY));
	npcArray.at(2).rect.setPosition({ player.rect.getPosition().x - 64 * 8, player.rect.getPosition().y - 64 * 7 });
	npcArray.at(2).update();

	if (!npcTexture.at(3).loadFromFile("res/img/npc5.png")) {
		cout << "res/img/npc5.png not loaded." << endl;
	}
	npcArray.at(3).name = "Ancient 2";
	npcArray.at(3).conversation = vector<string>{ "You travel so long little child.", "My name is Ancient Two",
		"The Queen of Nodernguard", "Khaleesi of steppe sahara", 
		"What?", "Your family jewelry?", "Oh! Shoot I pick the wrong thing", "Anyway you have to fight me before get it back!" };
	npcArray.at(3).endConversation = vector<string>{ "Ahhh!!", "I used to be an adventurer like you", "But I took the arrow in the knee", "A cursed one", "I\'m sorry for your lost my child..." };
	npcArray.at(3).stand = enemyType[3];
	npcArray.at(3).stand.sprite.setColor(sf::Color(255, 255, 255, 100));
	npcArray.at(3).stand.hp = 40;
	npcArray.at(3).stand.maxHp = 40;
	npcArray.at(3).stand.text.setString("40/40");
	npcArray.at(3).stand.owner = npcArray.at(3).name;
	npcArray.at(3).sprite.setTexture(npcTexture.at(3));
	npcArray.at(3).sprite.setTextureRect(sf::IntRect(1 * spriteSizeX, 18 * spriteSizeY, spriteSizeX, spriteSizeY));
	npcArray.at(3).rect.setPosition({ player.rect.getPosition().x - 64 * 8, player.rect.getPosition().y - 64 * 7 });
	npcArray.at(3).update();
	
	activeNpc = npcArray.at(currentStage - 1);
}

void Game::run() {
	if (loadingClock.getElapsedTime().asSeconds() > 5.f) {
		window->clear();
		if (player.rect.getPosition().x < 0 || player.rect.getPosition().y < 0) {
			remap();
		}
		view.setCenter(player.rect.getPosition());
		window->setView(view);
		update();
		clearJunk();
		inputProcess();
		render();
	}
	else {
		remap();
	}
}

void Game::render() {
	//World
	for (vector<sf::Sprite>::iterator it = mapSprite.begin(); it != mapSprite.end(); it++) {
		window->draw(*it);
	}
	if (isUsingStand) {
		window->draw(stand.sprite);
	}
	window->draw(player.sprite);
	window->draw(activeNpc.sprite);

	// Draw Pickup item
	for (vector<Pickup>::iterator piIt = pickupArray.begin(); piIt != pickupArray.end(); piIt++) {
		piIt->update();
		window->draw(piIt->sprite);
		if (piIt->inShop) {
			window->draw(piIt->text);
		}
	}

	//Update Bullet
	for (vector<Projectile>::iterator pIt = projectileArray.begin(); pIt != projectileArray.end(); pIt++) {
		pIt->update();
		//window->draw(pIt->sprite);
		//window->draw(pIt->rect);
	}

	//Update Enemy
	for (vector<Enemy>::iterator eIt = enemyArray.begin(); eIt != enemyArray.end(); eIt++) {
		eIt->update();
		eIt->updateMovement(false);
		if (eIt->isAggressive) {
			window->draw(eIt->text);
		}
		window->draw(eIt->sprite);
	}

	// Draw wall
	for (vector<Wall>::iterator wIt = wallArray.begin(); wIt != wallArray.end(); wIt++) {
		window->draw(wIt->sprite);
		//window->draw(wIt->rect);
	}

	//drawing current stage
	currentStageShow.setString("Current stage: " + to_string(currentStage));
	currentStageShow.setPosition(player.rect.getPosition().x + window->getSize().x / 2 - 300, player.rect.getPosition().y - window->getSize().y / 2);
	window->draw(currentStageShow);
	//drawing coin
	extraShow.setString("coin : " + to_string(player.coin));
	extraShow.setPosition(player.rect.getPosition().x - window->getSize().x / 2, player.rect.getPosition().y - window->getSize().y / 2);
	window->draw(extraShow);
	//drawing player hp
	hpShow.setString("HP : " + to_string(player.hp) + "/" + to_string(player.maxHp));
	hpShow.setPosition(player.rect.getPosition().x - window->getSize().x / 2, player.rect.getPosition().y - window->getSize().y / 2 + 24);
	window->draw(hpShow);
	//drawing is using stand
	if (isUsingStand) {
		isUsingStandShow.setString("Stand on");
		isUsingStandShow.setPosition(player.rect.getPosition().x - window->getSize().x / 2, player.rect.getPosition().y - window->getSize().y / 2 + 48);
		window->draw(isUsingStandShow);
	}

	if (!dialogBox.isShow) {
		if (!isUsingStand) {
			player.updateMovement(false);
			player.update();
		}
		else {
			player.updateMovement(false);
			player.update();
			window->draw(stand.sprite);
			stand.updateMovement(false);
			stand.update();
		}
	}

	// Draw damage effect
	for (vector<TextDisplay>::iterator tIt = textDisplayArray.begin(); tIt != textDisplayArray.end(); tIt++) {
		tIt->update();
		window->draw(tIt->text);
	}

	dialogBox.box.setPosition(player.rect.getPosition().x - window->getSize().x / 2, player.rect.getPosition().y + window->getSize().y / 6);
	dialogBox.text.setPosition(20 + player.rect.getPosition().x - window->getSize().x / 2, 20 + player.rect.getPosition().y + window->getSize().y / 6);

	//Draw dialog box
	if (dialogBox.active == "STAGE_CHANGE" && dialogBox.isShow) {
		sf::RectangleShape black;
		black.setFillColor(sf::Color::Black);
		black.setSize(sf::Vector2f{ 89.f * 64.f, 89.f * 64.f });
		window->draw(black);
	}
	if (dialogBox.isShow) {
		window->draw(dialogBox.box);
		window->draw(dialogBox.text);
	/*if (isUsingStand) {
			window->draw(stand.sprite);
		}*/
	}

	window->display();
}

void Game::inputProcess() {
	sf::Time usingStandTime = switchStandClock.getElapsedTime();
	if (usingStandTime.asSeconds() >= 0.2f) {
		switchStandClock.restart();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			if (!isUsingStand) {
				yareYare.play();
			}
			stand.rect.setPosition(player.rect.getPosition());
			isUsingStand = !isUsingStand;
		}
	}

	sf::Time dialogTime = dialogClock.getElapsedTime();
	if (dialogTime.asSeconds() >= .2f) {
		dialogClock.restart();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && dialogBox.isShow) {
			dialogBox.update();
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window->close();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {
		remap();
	}

	sf::Event event;
	while (window->pollEvent(event)) {
		switch (event.type)
		{
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == 12) { //M
				cout << "Boss: " << activeNpc.rect.getPosition().x / 64 << " " << activeNpc.rect.getPosition().y / 64 << endl;
			}
			if (dialogBox.isShow && event.key.code == 5) {
				sf::sleep(sf::seconds(.1f));
				dialogBox.update();
			}
			break;
		default:
			break;
		}
	}
}

void Game::clearJunk() {
	//player kill npc stand
	for (auto& enemy : enemyArray) {
		//for (vector<Npc>::iterator nIt = npcArray.begin(); nIt != npcArray.end(); nIt++) {
			if (enemy.hp <= 0 && enemy.owner == activeNpc.name) {
				//cout << nIt->endConversation[0] << endl;
				dialogBox.setDialog(activeNpc.endConversation, activeNpc.name);
				sf::sleep(sf::seconds(.1f));
			}
		//}
	}
	//Delete dead enemy
	for (vector<Enemy>::iterator eIt = enemyArray.begin(); eIt != enemyArray.end(); eIt++) {
		if (eIt->alive == false) {
			//random generate coin
			if (generateRandom(3) == 1) {
				pickup.inShop = false;
				pickup.isCoin = true;
				pickup.isPowerUp = false;
				pickup.sprite.setTexture(textureCoin);
				pickup.sprite.setTextureRect(sf::IntRect(24 * 6.8, 24 * 5.5, 24, 24));
				pickup.rect.setPosition(eIt->rect.getPosition());
				pickupArray.push_back(pickup);
			}
			//random generate powerup
			if (generateRandom(3) == 1) {
				pickup.inShop = false;
				pickup.isPowerUp = true;
				pickup.isCoin = false;
				pickup.sprite.setTexture(textureCoin);
				pickup.sprite.setTextureRect(sf::IntRect(24 * 1.2, 24 * 6.8, 24, 24));
				pickup.rect.setPosition(eIt->rect.getPosition());
				pickupArray.push_back(pickup);
			}
			enemyArray.erase(eIt);
			break;
		}
	}

	//Delete Bullet
	for (vector<Projectile>::iterator pIt = projectileArray.begin(); pIt != projectileArray.end(); pIt++) {
		if (pIt->destroyed) {
			projectileArray.erase(pIt);
			break;
		}
	}

	//Delete TextDisplay
	for (vector<TextDisplay>::iterator tIt = textDisplayArray.begin(); tIt != textDisplayArray.end(); tIt++) {
		if (tIt->destroyed) {
			textDisplayArray.erase(tIt);
			break;
		}
	}

	//Delete Item
	for (vector<Pickup>::iterator piIt = pickupArray.begin(); piIt != pickupArray.end(); piIt++) {
		if (piIt->destroyed) {
			pickupArray.erase(piIt);
			break;
		}
	}

	//Delete Destructable wall
	for (vector<Wall>::iterator wIt = wallArray.begin(); wIt != wallArray.end(); wIt++) {
		if (wIt->destroyed) {
			//random generate coin
			if (generateRandom(3) == 1) {
				pickup.inShop = false;
				pickup.isCoin = true;
				pickup.isPowerUp = false;
				pickup.sprite.setTexture(textureCoin);
				pickup.sprite.setTextureRect(sf::IntRect(24 * 6.8, 24 * 5.5, 24, 24));
				pickup.rect.setPosition(wIt->rect.getPosition());
				pickupArray.push_back(pickup);
			}
			//random generate powerup
			if (generateRandom(3) == 1) {
				pickup.inShop = false;
				pickup.isPowerUp = true;
				pickup.isCoin = false;
				pickup.sprite.setTexture(textureCoin);
				pickup.sprite.setTextureRect(sf::IntRect(24 * 1.2, 24 * 6.8, 24, 24));
				pickup.rect.setPosition(wIt->rect.getPosition());
				pickupArray.push_back(pickup);
			}
			wallArray.erase(wIt);
			break;
		}
	}
}

void Game::update() {
	dialogUpdate();
	collisionRelated();
	itemRelated();
	enemyRelated();
	playerAttack();
}

void Game::collisionRelated() {
	//Player collide with wall
	for (vector<Wall>::iterator wIt = wallArray.begin(); wIt != wallArray.end(); wIt++) {
		if (player.rect.getGlobalBounds().intersects(wIt->rect.getGlobalBounds()) && !(wIt->trap)) {
			player.cantMoveDi = player.direction;
			sf::Vector2f bounce[4] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
			player.rect.move(bounce[player.direction - 1] * player.movementSpeed);
			sf::sleep(sf::seconds(.01f));
		}
	}

	//Player collide with Npc
	if(distance(player.rect.getPosition().x, player.rect.getPosition().y,
		activeNpc.rect.getPosition().x, activeNpc.rect.getPosition().y) <= 64 * 2 
		&& !activeNpc.usingStand && enemyKilled >= currentStage * 3){
		dialogBox.setDialog(activeNpc.conversation, activeNpc.name);
		activeNpc.usingStand = true;
	}

	//Projectile collide with wall
	for (vector<Projectile>::iterator pIt = projectileArray.begin(); pIt != projectileArray.end(); pIt++) {
		for (vector<Wall>::iterator wIt = wallArray.begin(); wIt != wallArray.end(); wIt++) {
			if (pIt->rect.getGlobalBounds().intersects(wIt->rect.getGlobalBounds())) {
				if (wIt->destructable) {
					soundCollisionHit.play();
					wIt->hp -= pIt->attackDamage;
					if (wIt->hp <= 0) {
						wIt->destroyed = true;// destroy wall
					}
				}
				pIt->destroyed = true;
			}
		}
	}

	//Enemy collide with wall
	for (vector<Wall>::iterator wIt = wallArray.begin(); wIt != wallArray.end(); wIt++) {
		for (vector<Enemy>::iterator eIt = enemyArray.begin(); eIt != enemyArray.end(); eIt++) {
			if (eIt->rect.getGlobalBounds().intersects(wIt->rect.getGlobalBounds())) {
				eIt->cantMoveDi = eIt->direction;
				sf::Vector2f bounce[4] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
				eIt->rect.move(bounce[eIt->direction - 1] * eIt->movementSpeed);
			}
		}
	}
}

void Game::enemyRelated() {
	// Enemy attack player
	sf::Time enemyAttackPlayer = enemyAttackPlayerClock.getElapsedTime();
	if (enemyAttackPlayer.asSeconds() >= 0.30f) {
		enemyAttackPlayerClock.restart();
		for (auto &enemy : enemyArray) {
			if (player.rect.getGlobalBounds().intersects(enemy.rect.getGlobalBounds()) && enemy.hp > 0) {
				enemy.isAggressive = true;
				enemy.updateMovement(true);
				player.hp -= enemy.attackDamage;
				stand.rect.setPosition(player.rect.getPosition());
				soundPlayerHit.play();
				textDisplay.text.setPosition(
					player.rect.getPosition().x + player.rect.getSize().x / 2,
					player.rect.getPosition().y + player.rect.getSize().y
				);
				textDisplay.text.setFillColor(sf::Color::Yellow);
				textDisplay.text.setString(to_string(enemy.attackDamage));
				textDisplayArray.push_back(textDisplay);
				if (player.hp <= 0) {
					remap();
					*playingScore += player.coin * 10;
					*isPlaying = false;
					*state = 2;
				}
			}
		}
	}

	//Trap
	sf::Time wallAttackPlayer = trapClock.getElapsedTime();
	if (wallAttackPlayer.asSeconds() >= 0.30f) {
		trapClock.restart();
		for (auto& wall : wallArray) {
			if (player.rect.getGlobalBounds().intersects(wall.rect.getGlobalBounds()) && wall.trap) {
				player.hp -= wall.damage;
				soundPlayerHit.play();
				textDisplay.text.setPosition(
					player.rect.getPosition().x + player.rect.getSize().x / 2,
					player.rect.getPosition().y + player.rect.getSize().y
				);
				textDisplay.text.setFillColor(sf::Color::Yellow);
				textDisplay.text.setString(to_string(wall.damage));
				textDisplayArray.push_back(textDisplay);
				if (player.hp <= 0) {
					remap();
					*playingScore += player.coin * 10;
					*isPlaying = false;
					*state = 2;
				}
			}
		}
	}
	
	//Agressive if getting close
	for (vector<Enemy>::iterator eIt = enemyArray.begin(); eIt != enemyArray.end(); eIt++) {
		if (distance(eIt->rect.getPosition().x, eIt->rect.getPosition().y, 
			player.rect.getPosition().x, player.rect.getPosition().y) <= 3.f * 64) {
			(*eIt).isAggressive = true;
		}
	}

	//Check collision attack enemy
	for (vector<Projectile>::iterator pIt = projectileArray.begin(); pIt != projectileArray.end(); pIt++) {
		for (vector<Enemy>::iterator eIt = enemyArray.begin(); eIt != enemyArray.end(); eIt++) {
			if (pIt->rect.getGlobalBounds().intersects(
				eIt->rect.getGlobalBounds())) {
				eIt->isAggressive = true; //Set to chase player
				// Enemy got hit
				soundCollisionHit.play();
				//Damage display
				textDisplay.text.setFillColor(sf::Color::Red);
				textDisplay.text.setString(to_string(pIt->attackDamage));
				textDisplay.text.setPosition(
					eIt->rect.getPosition().x + eIt->rect.getSize().x / 2,
					eIt->rect.getPosition().y - eIt->rect.getSize().y / 2
				);
				textDisplayArray.push_back(textDisplay);

				//Update enemy health
				pIt->destroyed = true;
				eIt->hp -= pIt->attackDamage;
				eIt->text.setString(to_string(eIt->hp) + "/" + to_string(eIt->maxHp));
				if (eIt->hp <= 0) {
					enemyKilled++;
					eIt->alive = false;
				}
				*playingScore += pIt->attackDamage * 100;
			}
		}
	}



	//Aggressive enemy
	sf::Time aggressiveEnemyTimer = aggressiveEnemyClock.getElapsedTime();
	for (vector<Enemy>::iterator eIt = enemyArray.begin(); eIt != enemyArray.end(); eIt++) {
		if (eIt->isAggressive) {
			if (aggressiveEnemyTimer.asSeconds() >= 1.0f) {
				aggressiveEnemyClock.restart();
				if (abs(player.rect.getPosition().x - eIt->rect.getPosition().x) >=
					abs(player.rect.getPosition().y - eIt->rect.getPosition().y)) {
					if (player.rect.getPosition().x > eIt->rect.getPosition().x) {
						eIt->direction = 4;
					}
					else if (player.rect.getPosition().x < eIt->rect.getPosition().x) {
						eIt->direction = 3;
					}
				}
				else {
					if (player.rect.getPosition().y > eIt->rect.getPosition().y) {
						eIt->direction = 2;
					}
					else if (player.rect.getPosition().y < eIt->rect.getPosition().y) {
						eIt->direction = 1;
					}
				}
			}
		}
	}
}

void Game::playerAttack() {
	//Set stand position
	int positionLap[4][2] = { {20, 20}, {-20, -20}, {20, -20}, {-20, 20} };
	stand.rect.setPosition(
		player.rect.getPosition().x + positionLap[player.direction-1][0], 
		player.rect.getPosition().y + positionLap[player.direction - 1][1]
	);

	//Attack
	if (isUsingStand) {
		sf::Time checkAttack = attackClock.getElapsedTime();
		if (checkAttack.asSeconds() >= 0.15f) {
			attackClock.restart();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !dialogBox.isShow) {
				stand.updateMovement(true);
				soundShot.play();
				projectile.rect.setPosition(
					stand.rect.getPosition().x + stand.rect.getSize().x / 2 - projectile.rect.getSize().x / 2,
					stand.rect.getPosition().y + stand.rect.getSize().y / 2 - projectile.rect.getSize().y / 2
				);
				projectile.direction = stand.direction;
				projectileArray.push_back(projectile);
			}
		}
	}
}

void Game::itemRelated() {
	//Player Collide with pickup
	for (vector<Pickup>::iterator piIt = pickupArray.begin(); piIt != pickupArray.end(); piIt++) {
		if (player.rect.getGlobalBounds().intersects(piIt->rect.getGlobalBounds())) {
			if (!(piIt->inShop)) {
				if (piIt->isCoin) {
					player.coin += piIt->coinValue;
					piIt->destroyed = true;
				}
				else if (piIt->isPowerUp) {
					player.hp += 5;
					if (player.hp > player.maxHp) {
						player.hp = player.maxHp;
					}
					piIt->destroyed = true;
				}
			}
			else {
				sf::Time inShopTimer = shopClock.getElapsedTime();
				if (inShopTimer.asSeconds() >= 0.5f) {
					shopClock.restart();
					cout << "in shop!" << endl;
					if (piIt->isPowerUp && player.coin >= piIt->cost
						&& sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
						player.coin -= piIt->cost;
						player.hp += 5;
						if (player.hp > player.maxHp) {
							player.hp = player.maxHp;
						}
					}
				}
			}
		}
	}
}