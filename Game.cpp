#include "Game.h"
#include "DialogBox.h"
#include <iostream>

int heightToTile(int h) {
	//mountain
	if (h < 35) {
		return 0;
	}

	// grass
	if (h > 34 && h < 50) {
		return 1;
	}

	// sand
	if (h > 49) {
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

	if (!music.openFromFile("res/music/Windless Slopes.ogg")) {
		cout << "res/music/Windless Slopes.ogg not loaded." << endl;
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
	if (!music.openFromFile("res/music/Windless Slopes.ogg")) {
		cout << "res/music/Windless Slopes.ogg not loaded." << endl;
	}
	music.setLoop(true);
	music.play();

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

	player.sprite.setTexture(texturePlayer);
	stand.sprite.setTexture(textureStand);

	player.sprite.setTextureRect(sf::IntRect(0, texturePlayer.getSize().y / 21 * 10,
		texturePlayer.getSize().x / 13, texturePlayer.getSize().y / 21));
	stand.sprite.setTextureRect(sf::IntRect(0, texturePlayer.getSize().y / 21 * 10,
		texturePlayer.getSize().x / 13, texturePlayer.getSize().y / 21));

	//player.rect.setSize(sf::Vector2f(texturePlayer.getSize()));

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

	coinShow.setFont(font);
	coinShow.setCharacterSize(25);
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

	//Npc
	npcTexture.push_back(sf::Texture());
	if (!npcTexture.at(npcTexture.size() - 1).loadFromFile("res/img/stand.png")) {
		cout << "res/img/stand.png not loaded." << endl;
	}
	npcArray.push_back(Npc());
	npcArray.at(npcArray.size() - 1).sprite.setTexture(npcTexture.at(npcArray.size() - 1));
	int spriteSizeX = npcArray.at(npcArray.size() - 1).sprite.getTexture()->getSize().x / 13;
	int spriteSizeY = npcArray.at(npcArray.size() - 1).sprite.getTexture()->getSize().y / 21;
	npcArray.at(npcArray.size() - 1).sprite.setTextureRect(sf::IntRect(0, 2 * spriteSizeY, spriteSizeX, spriteSizeY));
	npcArray.at(npcArray.size() - 1).rect.setPosition({ 200, 300 });
	npcArray.at(npcArray.size() - 1).update();
	
	generateGameObjects();
}


void Game::generateMap() {
	mapDrawer.setTexture(tiles);
	for (int y = 0; y < 89; y++) {
		for (int x = 0; x < 89; x++) {
			// mountain (set wall so people can't pass through)
			if (heightToTile(world._Matrix[y][x]) == 0) {
				mapDrawer.setTextureRect(sf::IntRect(5 * 16, 16 * 2, 16, 16));
				/////
				if (heightToTile(world._Matrix[y][x - 1]) == 1 && heightToTile(world._Matrix[y - 1][x]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(4 * 16, 16 * 1, 16, 16));
				}
				//else if (heightToTile(world._Matrix[y - 1][x]) == 1 && heightToTile(world._Matrix[y][x + 1]) == 2) {
				//	mapDrawer.setTextureRect(sf::IntRect(6 * 16, 16 * 1, 16, 16));
				//}
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
				mapDrawer.setTextureRect(sf::IntRect(5 * 16, 16 * 2, 16, 16));
				mapDrawer.setPosition(sf::Vector2f(x * 64, y * 64));
				mapDrawer.setScale(4.0f, 4.0f);
				mapSprite.push_back(mapDrawer);
			}

			// sand
			if (heightToTile(world._Matrix[y][x]) == 2) {
				mapDrawer.setTextureRect(sf::IntRect(1 * 16, 5 * 16, 16, 16));
				if (heightToTile(world._Matrix[y][x - 1]) == 1 && heightToTile(world._Matrix[y - 1][x]) == 1) {
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
	

	int treeX, treeY;
	Wall tree;
	tree.sprite.setTexture(tiles);
	tree.sprite.setTextureRect(sf::IntRect(6 * 16, 5 * 16, 32, 32));
	tree.sprite.setScale(sf::Vector2f(4.f, 4.f));

	//gen tree
	for (int i = 0; i < 55; i++) {
		bool flagTree = true;

		while (flagTree) {
			treeX = generateRandom0(89);
			treeY = generateRandom0(89);
			flagTree = false;
			if (heightToTile(world._Matrix[treeY][treeX]) != 1) {
				flagTree = true;
			}
		}

		tree.rect.setPosition(treeX * 64 + 32, treeY * 64 + 64);
		tree.sprite.setPosition(treeX * 64, treeY * 64);
		wallArray.push_back(tree);
	}

	// gen boulders

	int boulderX, boulderY;
	Wall boulder;
	boulder.sprite.setTexture(tiles);
	boulder.sprite.setTextureRect(sf::IntRect(9 * 16, 5 * 16, 16, 16));
	boulder.sprite.setScale(sf::Vector2f(4.f, 4.f));

	for (int i = 0; i < 20; i++) {
		bool flagBoulder = true;

		while (flagBoulder) {
			boulderX = generateRandom0(89);
			boulderY = generateRandom0(89);
			flagBoulder = false;
			if (heightToTile(world._Matrix[boulderY][boulderX]) != 1) {
				flagBoulder = true;
			}
		}

		boulder.rect.setPosition(boulderX * 64, boulderY * 64);
		boulder.sprite.setPosition(boulderX * 64, boulderY * 64);
		wallArray.push_back(boulder);
	}

	//Only spawn player on sand tiles
	bool flag = true;
	int playerX;
	int playerY;
	while (flag) {
		playerX = generateRandom0(89);
		playerY = generateRandom0(89);
		flag = false;
		if (heightToTile(world._Matrix[playerY][playerX]) != 1) {
			flag = true;
		}
	}

	player.rect.setPosition(playerX * 64, playerY * 64);

	int enemyX;
	int enemyY;
	//gen enemy
	for (int i = 0; i < 8; i++) {
		bool flagEnemy = true;

		while (flagEnemy) {
			enemyX = generateRandom0(89);
			enemyY = generateRandom0(89);
			flagEnemy = false;
			if (heightToTile(world._Matrix[enemyY][enemyX]) != 1) {
				flagEnemy = true;
			}
		}

		enemy.rect.setPosition(enemyX * 64, enemyY * 64);
		enemyArray.push_back(enemy);
	}
}

void Game::remap() {
	cout << *playingScore << endl;
	view.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	view.setCenter(sf::Vector2f(view.getSize().x / 2, view.getSize().y / 2));
	window->setView(view);

	player.sprite.setTextureRect(sf::IntRect(0, texturePlayer.getSize().y / 21 * 10,
		texturePlayer.getSize().x / 13, texturePlayer.getSize().y / 21));
	stand.sprite.setTextureRect(sf::IntRect(0, texturePlayer.getSize().y / 21 * 10,
		texturePlayer.getSize().x / 13, texturePlayer.getSize().y / 21));

	player.hp = 10;

	enemyArray.clear();
	projectileArray.clear();
	textDisplayArray.clear();
	pickupArray.clear();
	wallArray.clear();
	mapSprite.clear();

	world.CreateMatrix();
	world.Interpolation();
	generateMap();
	generateGameObjects();
}

void Game::run() {
	window->clear();
	view.setCenter(player.rect.getPosition());
	window->setView(view);
	update();
	clearJunk();
	inputProcess();
	render();
}

void Game::render() {
	//World
	//world.Render(&window);
	for (vector<sf::Sprite>::iterator it = mapSprite.begin(); it != mapSprite.end(); it++) {
		window->draw(*it);
	}

	window->draw(player.sprite);

	//Draw npc
	for (vector<Npc>::iterator nIt = npcArray.begin(); nIt != npcArray.end(); nIt++) {
		window->draw(nIt->sprite);
	}

	// Draw Pickup item
	for (vector<Pickup>::iterator piIt = pickupArray.begin(); piIt != pickupArray.end(); piIt++) {
		piIt->update();
		window->draw(piIt->sprite);
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
		window->draw(eIt->text);
		//window->draw(eIt->rect);
		window->draw(eIt->sprite);
	}

	// Draw wall
	for (vector<Wall>::iterator wIt = wallArray.begin(); wIt != wallArray.end(); wIt++) {
		window->draw(wIt->sprite);
		//window->draw(wIt->rect);
	}

	//drawing coin
	coinShow.setString("Coin : " + to_string(player.coin));
	coinShow.setPosition(player.rect.getPosition().x - window->getSize().x / 2, player.rect.getPosition().y - window->getSize().y / 2);
	window->draw(coinShow);
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
	//dialogBox.text.setString("Hi");
	//Draw dialog box
	if (dialogBox.isShow) {
		window->draw(dialogBox.box);
		window->draw(dialogBox.text);
		if (isUsingStand) {
			window->draw(stand.sprite);
		}
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
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
				dialogBox.isShow = false;
			}
			break;
		default:
			break;
		}
	}
}

void Game::clearJunk() {
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
	collisionRelated();
	itemRelated();
	enemyRelated();
	playerAttack();
}

void Game::collisionRelated() {
	//Player collide with wall
	for (vector<Wall>::iterator wIt = wallArray.begin(); wIt != wallArray.end(); wIt++) {
		if (player.rect.getGlobalBounds().intersects(wIt->rect.getGlobalBounds())) {
			player.cantMoveDi = player.direction;
			sf::Vector2f bounce[4] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
			player.rect.move(bounce[player.direction - 1] * player.movementSpeed);
			sf::sleep(sf::seconds(.01f));
		}
	}

	//Player collide with Npc
	for (vector<Npc>::iterator nIt = npcArray.begin(); nIt != npcArray.end(); nIt++) {
		if (player.rect.getGlobalBounds().intersects(nIt->rect.getGlobalBounds())) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
				dialogBox.reset();
				dialogBox.conversation = vector<string>{ "What are you doing?", "Go away!!", "!!"};
				dialogBox.isShow = true;
			}
			if (dialogBox.conversation.at(dialogBox.curIndex) == "!!") {
				enemy.rect.setPosition(nIt->rect.getPosition());
				enemyArray.push_back(enemy);
				dialogBox.reset();
			}
		}
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
			if (player.rect.getGlobalBounds().intersects(enemy.rect.getGlobalBounds())) {
				isUsingStand = false;
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
					*isPlaying = false;
					*state = 2;
				}
			}
		}
	}
	
	//Check collision attack enemy
	for (vector<Projectile>::iterator pIt = projectileArray.begin(); pIt != projectileArray.end(); pIt++) {
		for (vector<Enemy>::iterator eIt = enemyArray.begin(); eIt != enemyArray.end(); eIt++) {
			if (pIt->rect.getGlobalBounds().intersects(
				eIt->rect.getGlobalBounds())) {
				if (eIt->isAggressive != true) {
					dialogBox.text.setString("Ahhhhh!, You hit me!");
					dialogBox.isShow = true;
				}
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
	//Attack
	if (isUsingStand) {
		sf::Time checkAttack = attackClock.getElapsedTime();
		if (checkAttack.asSeconds() >= 0.15f) {
			attackClock.restart();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
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
					if (player.hp > 10) {
						player.hp = 10;
					}
					piIt->destroyed = true;
				}
			}
			else {
				// draw cost
				window->draw(piIt->text);
				if (piIt->isPowerUp && player.coin >= piIt->cost
					&& sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
					player.coin -= piIt->cost;
					player.hp += 5;
					piIt->destroyed = true;
				}
			}
		}
	}
}