#include "Game.h"
#include "DialogBox.h"
#include <iostream>

Game::Game() : window(sf::VideoMode(1080, 760), "Niti Bizarre Adventure"), view(sf::FloatRect(200, 200, 300, 200)), dialogBox() {
	window.setFramerateLimit(60);

	view.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
	view.setCenter(sf::Vector2f(view.getSize().x / 2, view.getSize().y / 2));
	window.setView(view);

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

	if (!textureEnemy.loadFromFile("res/img/enemy.png")) {
		cout << "res/img/enemy.png not loaded." << endl;
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
	enemyArray.push_back(enemy);

	textDisplay.text.setFont(font);

	coinShow.setFont(font);
	coinShow.setCharacterSize(25);
	hpShow.setFont(font);
	hpShow.setCharacterSize(25);
	isUsingStandShow.setFont(font);
	isUsingStandShow.setCharacterSize(25);

	dialogBox.box.setPosition(0, 2 * window.getSize().y / 3);
	dialogBox.box.setSize(sf::Vector2f(window.getSize().x, window.getSize().y / 3));

	dialogBox.text.setFont(font);
	dialogBox.text.setPosition(20, ((2 * window.getSize().y) / 3) + 20);
	
	//World sprite
	if (!tiles.loadFromFile("res/img/tiles.png")) {
		cout << "res/img/tiles.png not loaded." << endl;
	}

	// world
	world.CreateMatrix();
	world.Interpolation();
	//world.CreateGraphics();
	generateMap();

}

int heightToTile(int h) {
	if (h < 35) {
		return 0;
	}

	// Sand - Beaches 
	if (h > 34 && h < 50) {
		return 1;
	}

	// Grass
	if (h > 49) {
		return 2;
	}
}

void Game::generateMap() {
	mapDrawer.setTexture(tiles);
	for (int y = 0; y < 89; y++) {
		for (int x = 0; x < 89; x++) {
			// water
			if (heightToTile(world._Matrix[y][x]) == 0) {
				mapDrawer.setTextureRect(sf::IntRect(13 * 16, 16 * 0, 16, 16));
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

			// Sand - Beaches 
			if (heightToTile(world._Matrix[y][x]) == 1) {
				mapDrawer.setTextureRect(sf::IntRect(1 * 16, 16 * 5, 16, 16));
				if (heightToTile(world._Matrix[y][x - 1]) == 2 && heightToTile(world._Matrix[y-1][x]) == 2) {
					mapDrawer.setTextureRect(sf::IntRect(0 * 16, 16 * 4, 16, 16));
				}
				else if (heightToTile(world._Matrix[y - 1][x]) == 2 && heightToTile(world._Matrix[y][x + 1]) == 1) {
					mapDrawer.setTextureRect(sf::IntRect(1 * 16, 16 * 4, 16, 16));
				}
				else if (heightToTile(world._Matrix[y - 1][x]) == 2 && heightToTile(world._Matrix[y][x+1]) == 2) {
					mapDrawer.setTextureRect(sf::IntRect(2 * 16, 16 * 4, 16, 16));
				}
				else if (heightToTile(world._Matrix[y + 1][x]) == 2 && heightToTile(world._Matrix[y][x - 1]) == 2) {
					mapDrawer.setTextureRect(sf::IntRect(0 * 16, 16 * 6, 16, 16));
				}
				else if (heightToTile(world._Matrix[y + 1][x]) == 2 && heightToTile(world._Matrix[y][x + 1]) == 2) {
					mapDrawer.setTextureRect(sf::IntRect(2 * 16, 16 * 6, 16, 16));
				}
				else if (heightToTile(world._Matrix[y][x - 1]) == 2) {
					mapDrawer.setTextureRect(sf::IntRect(0 * 16, 16 * 5, 16, 16));
				}
				else if (heightToTile(world._Matrix[y][x + 1]) == 2) {
					mapDrawer.setTextureRect(sf::IntRect(2 * 16, 16 * 5, 16, 16));
				}
				else if (heightToTile(world._Matrix[y + 1][x]) == 2) {
					mapDrawer.setTextureRect(sf::IntRect(1 * 16, 16 * 6, 16, 16));
				}
				mapDrawer.setPosition(sf::Vector2f(x * 64, y * 64));
				mapDrawer.setScale(4.0f, 4.0f);
				mapSprite.push_back(mapDrawer);
			}

			// Grass
			if (heightToTile(world._Matrix[y][x]) == 2) {
				mapDrawer.setTextureRect(sf::IntRect(5 * 16, 16 * 2, 16, 16));
				mapDrawer.setPosition(sf::Vector2f(x * 64, y * 64));
				mapDrawer.setScale(4.0f, 4.0f);
				mapSprite.push_back(mapDrawer);
			}
		}
	}
}



void Game::run() {
	while (window.isOpen()) {
		window.clear();
		view.setCenter(player.rect.getPosition());
		window.setView(view);
		update();
		clearJunk();
		inputProcess();
		render();
	}
}

void Game::render() {
	//World
	//world.Render(&window);
	for (vector<sf::Sprite>::iterator it = mapSprite.begin(); it != mapSprite.end(); it++) {
		window.draw(*it);
	}

	// Draw wall
	/*
	for (vector<Wall>::iterator wIt = wallArray.begin(); wIt != wallArray.end(); wIt++) {
		window.draw(wIt->rect);
	}*/

	// Draw Pickup item
	for (vector<Pickup>::iterator piIt = pickupArray.begin(); piIt != pickupArray.end(); piIt++) {
		piIt->update();
		window.draw(piIt->sprite);
	}

	//Update Bullet
	for (vector<Projectile>::iterator pIt = projectileArray.begin(); pIt != projectileArray.end(); pIt++) {
		pIt->update();
		//window.draw(pIt->sprite);
		//window.draw(pIt->rect);
	}

	//Update Enemy
	for (vector<Enemy>::iterator eIt = enemyArray.begin(); eIt != enemyArray.end(); eIt++) {
		eIt->update();
		eIt->updateMovement();
		window.draw(eIt->text);
		window.draw(eIt->rect);
		window.draw(eIt->sprite);
	}

	//drawing coin
	coinShow.setString("Coin : " + to_string(player.coin));
	coinShow.setPosition(player.rect.getPosition().x - window.getSize().x / 2, player.rect.getPosition().y - window.getSize().y / 2);
	window.draw(coinShow);
	//drawing player hp
	hpShow.setString("HP : " + to_string(player.hp) + "/" + to_string(player.maxHp));
	hpShow.setPosition(player.rect.getPosition().x - window.getSize().x / 2, player.rect.getPosition().y - window.getSize().y / 2 + 24);
	window.draw(hpShow);
	//drawing is using stand
	if (isUsingStand) {
		isUsingStandShow.setString("Stand on");
		isUsingStandShow.setPosition(player.rect.getPosition().x - window.getSize().x / 2, player.rect.getPosition().y - window.getSize().y / 2 + 48);
		window.draw(isUsingStandShow);
	}
	window.draw(player.sprite);

	if (!dialogBox.isShow) {
		if (!isUsingStand) {
			player.updateMovement(false);
			player.update();
		}
		else {
			window.draw(stand.sprite);
			stand.updateMovement(false);
			stand.update();
		}
	}

	// Draw damage effect
	for (vector<TextDisplay>::iterator tIt = textDisplayArray.begin(); tIt != textDisplayArray.end(); tIt++) {
		tIt->update();
		window.draw(tIt->text);
	}

	dialogBox.box.setPosition(player.rect.getPosition().x - window.getSize().x / 2, player.rect.getPosition().y + window.getSize().y / 6);
	dialogBox.text.setPosition(20 + player.rect.getPosition().x - window.getSize().x / 2, 20 + player.rect.getPosition().y + window.getSize().y / 6);
	//dialogBox.text.setString("Hi");
	//Draw dialog box
	if (dialogBox.isShow) {
		window.draw(dialogBox.box);
		window.draw(dialogBox.text);
		window.draw(stand.sprite);
	}

	
	window.display();
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
			cout << "Stand" << endl;
			isUsingStand = !isUsingStand;
		}
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window.close();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
		dialogBox.isShow = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {
		enemy.rect.setPosition(
			player.rect.getPosition().x + generateRandom(window.getSize().x),
			player.rect.getPosition().y + generateRandom(window.getSize().y)
		);
		enemyArray.push_back(enemy);
	}
	/*
	sf::Event event;
	while (window.pollEvent(event)) {
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;

		case sf::Event::KeyPressed:
			break;
		default:
			break;
		}
	}*/
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
		for (vector<Enemy>::iterator eIt = enemyArray.begin(); eIt != enemyArray.end(); eIt++) {
			if (player.rect.getGlobalBounds().intersects(eIt->rect.getGlobalBounds())) {
				isUsingStand = false; // stop using stand if player got hit
				eIt->isAggressive = true; // enemy got agressive when see player
				stand.rect.setPosition(player.rect.getPosition());
				soundPlayerHit.play();

				textDisplay.text.setPosition(
					player.rect.getPosition().x + player.rect.getSize().x / 2,
					player.rect.getPosition().y + player.rect.getSize().y
				);
				textDisplay.text.setFillColor(sf::Color::Yellow);
				textDisplay.text.setString(to_string(eIt->attackDamage));
				textDisplayArray.push_back(textDisplay);

				player.hp -= eIt->attackDamage;
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
			}
		}
	}

	//Aggressive enemy
	sf::Time aggressiveEnemyTimer = aggressiveEnemyClock.getElapsedTime();
	for (vector<Enemy>::iterator eIt = enemyArray.begin(); eIt != enemyArray.end(); eIt++) {
		if (eIt->isAggressive) {
			if (aggressiveEnemyTimer.asSeconds() >= 1.0f) {
				aggressiveEnemyClock.restart();
				cout << eIt->direction << endl;
				if (abs(player.rect.getPosition().x - eIt->rect.getPosition().x) >=
					abs(player.rect.getPosition().y - eIt->rect.getPosition().y)) {
					if (player.rect.getPosition().x > eIt->rect.getPosition().x) {
						eIt->direction = 4;
						cout << "player is to the right" << endl;
					}
					else if (player.rect.getPosition().x < eIt->rect.getPosition().x) {
						eIt->direction = 3;
						cout << "player is to the left" << endl;
					}
				}
				else {
					if (player.rect.getPosition().y > eIt->rect.getPosition().y) {
						eIt->direction = 2;
						cout << "player is to the bottom" << endl;
					}
					else if (player.rect.getPosition().y < eIt->rect.getPosition().y) {
						eIt->direction = 1;
						cout << "player is to the top" << endl;
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
					piIt->destroyed = true;
				}
			}
			else {
				// draw cost
				window.draw(piIt->text);
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