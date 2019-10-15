/*#include <iostream>
#include <SFML/Graphics.hpp>


int main()
{
	sf::RenderWindow window(sf::VideoMode(1080, 720), "Game from scratch!");

	sf::CircleShape collision(100.f);
	collision.setPosition({ 200.f, 200.f });
	collision.setFillColor(sf::Color::Red);

	sf::Texture playerTexture;
	if (!playerTexture.loadFromFile("res/images/002.png"))
	{
		std::cout << "Load failed" << std::endl;
	}

	sf::Sprite shapeSprite;
	shapeSprite.setTexture(playerTexture);
	shapeSprite.setTextureRect(sf::IntRect(0, 0, 32, 48));

	sf::Vector2f spawnPoint = { 0.f, 0.f };
	int animationFrame = 0;
	shapeSprite.setPosition(spawnPoint);

	while (window.isOpen())
	{
		window.draw(collision);
		window.draw(shapeSprite);
		window.display();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			shapeSprite.move(.1f, 0.f);
			shapeSprite.setTextureRect(sf::IntRect(32 * animationFrame, 48 * 2, 32, 48));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			shapeSprite.move(-.1f, 0.f);
			shapeSprite.setTextureRect(sf::IntRect(32 * animationFrame, 48 * 1, 32, 48));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			shapeSprite.move(0.f, -.1f);
			shapeSprite.setTextureRect(sf::IntRect(32 * animationFrame, 48 * 3, 32, 48));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			shapeSprite.move(0.f, .1f);
			shapeSprite.setTextureRect(sf::IntRect(32 * animationFrame, 0, 32, 48));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			//shapeSprite.move(.1f, 0.f);
			//shapeSprite.setTextureRect(sf::IntRect(32 * animationFrame, 48 * 2, 32, 48));
			window.close();
		}
		animationFrame++;

		if (animationFrame >= 4) {
			animationFrame = 0;
		}

		if (collision.getGlobalBounds().intersects(shapeSprite.getGlobalBounds())) {
			shapeSprite.setPosition(spawnPoint);
		}

		window.clear();
	}
	return 0;
}*/

#include <SFML/Graphics.hpp>
#include <iostream>

class DialogBox {
public:
	sf::Text text;
	sf::RectangleShape box;
	std::string conversation[];
	DialogBox();
	void update();
};

DialogBox::DialogBox() {
	box.setFillColor(sf::Color::Blue);
	text.setCharacterSize(16);
	text.setFillColor(sf::Color::White);
}

int main()
{
	sf::Clock clock;
	sf::RenderWindow window(sf::VideoMode(1080, 720), "Game from scratch!");

	std::string textList[3] = {
		"Go!",
		"Hi!",
		"My name is jonathan joestar"
	};

	int curT = 0;

	DialogBox dialogBox;
	dialogBox.box.setPosition(0, 2 * window.getSize().y / 3);
	dialogBox.box.setSize(sf::Vector2f(window.getSize().x, window.getSize().y / 3));

	sf::Font font;
	if (!font.loadFromFile("res/font/manaspc.ttf")) {
		std::cout << "Can\'t load font";
	}
	dialogBox.text.setFont(font);
	dialogBox.text.setPosition(20, ((2 * window.getSize().y) / 3) + 20);

	dialogBox.text.setString(textList[0]);

	while (window.isOpen())
	{
		window.draw(dialogBox.box);
		window.draw(dialogBox.text);
		window.display();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
		sf::Time t = clock.getElapsedTime();
		if (t.asSeconds() >= 0.15f) {
			clock.restart();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				dialogBox.text.setString(textList[curT]);
				curT++;
				if (curT == 3) curT = 0;
			}
		}

		window.clear();
	}
	return 0;
}
