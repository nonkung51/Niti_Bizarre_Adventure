#include <SFML/Graphics.hpp>

class GameObject {
public:
	GameObject(sf::Vector2f);
	bool shouldDraw = true;
	bool isTrigger = false;
	sf::Time lastAnimate = sf::Time::Zero;
	sf::Texture mTexture;
	sf::Sprite mSprite;
	sf::IntRect rect = sf::IntRect(0, 0, 24, 24);
	void setSprite(sf::IntRect);
	void animate(sf::Time);
};

GameObject::GameObject(sf::Vector2f start) : mTexture(), mSprite(){
	if (!mTexture.loadFromFile("res/img/gabe-idle-run.png")) {
		std::cout << "can\'t find picture" << std::endl;
	}
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(rect);
	mSprite.setPosition(start);
}

void GameObject::animate(sf::Time time) {
	if (time.asMilliseconds() - lastAnimate.asMilliseconds() >= 300.f) {
		if (rect.left > 120) {
			rect.left = 0;
		}
		rect.left += 24;
		mSprite.setTextureRect(rect);
		lastAnimate = time;
	}
}