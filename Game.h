class Game {
public:
	Game();
	void run();
private:
	void processEvents();
	void handlePlayerInput(sf::Keyboard::Key, bool);
	void update(sf::Time);
	void render();
	bool isCollision(GameObject, GameObject);
private:
	sf::RenderWindow mWindow;
	GameObject player;
	sf::IntRect playerRect = { 0,0,24,24 };
	std::vector<GameObject*> gameObjects;
	sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
	double PlayerSpeed = 200.f;
	bool mIsMovingUp = false;
	bool mIsMovingDown = false;
	bool mIsMovingLeft = false;
	bool mIsMovingRight = false;
};


bool Game::isCollision(GameObject a, GameObject b) {
	return !a.isTrigger && !b.isTrigger && a.mSprite.getGlobalBounds().intersects(b.mSprite.getGlobalBounds());
}

Game::Game() : mWindow(sf::VideoMode(1080, 760), "SFML Application"), player({ 0.f, 0.f }) {
	gameObjects.push_back(&player);
}

void Game::run() {
	sf::Clock clock;
	sf::Clock globalClock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	GameObject b({ 500.f,500.f });
	gameObjects.push_back(&b);
	while (mWindow.isOpen()) {
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > TimePerFrame) {
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
			player.animate(globalClock.getElapsedTime());
			update(TimePerFrame);
		}
		render();
	}
}

void Game::processEvents() {
	sf::Event event;
	while (mWindow.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;
		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;
		case sf::Event::Closed:
			mWindow.close();
			break;
		default:
			break;
		}
	}
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
	if (key == sf::Keyboard::W) {
		mIsMovingUp = isPressed;
	}
	else if (key == sf::Keyboard::S) {
		mIsMovingDown = isPressed;
	}
	else if (key == sf::Keyboard::A) {
		mIsMovingLeft = isPressed;
	}
	else if (key == sf::Keyboard::D) {
		mIsMovingRight = isPressed;
	}
}

void Game::update(sf::Time deltaTime) {
	sf::Vector2f movement(0.f, 0.f);
	bool move = true;

	if (mIsMovingUp) {
		movement.y -= PlayerSpeed;
	}
	if (mIsMovingDown) {
		movement.y += PlayerSpeed;
	}
	if (mIsMovingLeft) {
		movement.x -= PlayerSpeed;
	}
	if (mIsMovingRight) {
		movement.x += PlayerSpeed;
	}

	for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++) {
		if (*it != &player) {
			if (isCollision(player, *(*it))) {
				movement.x = movement.x * -1.f;
				movement.y = movement.y * -1.f;
				sf::sleep(sf::milliseconds(30));
				break;
			}
		}
	}
	player.mSprite.move(movement * deltaTime.asSeconds());

}

void Game::render() {
	mWindow.clear();
	sf::View view;
	view.setCenter(player.mSprite.getPosition());

	//Draw all the game object in gameObjects that should be drawn.
	for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++) {
		if ((*(*it)).shouldDraw) {
			mWindow.draw((*(*it)).mSprite);
		}
	}
	mWindow.setView(view);
	mWindow.display();
}