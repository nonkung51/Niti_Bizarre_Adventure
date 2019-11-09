#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <windows.h>
#include <conio.h> 
#include <time.h>
#include <string>
#include <sstream> 
#include "stdlib.h"
#include <SFML/Network.hpp>
using namespace std;


int randomkey()
{
	int key[12] = { 255,875,560,1182,1182,560,875,1182,255,255,875,560 };
	int x;
	srand(time(NULL));
	x = rand() % 12;
	return key[x];
}
int randomkey2()
{
	int key[12] = { 1182,875,560,1182,255,875,560,560,255,1182,875,560 };
	int x;
	srand(time(NULL));
	x = rand() % 12;
	return key[x];
}

int _main()
{
	// create the window
	sf::RenderWindow window;
	window.create(sf::VideoMode(1900, 850), "NiSu", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
	sf::Image Icon;
	Icon.loadFromFile("Pic/osu/osu.png");
	window.setIcon(600, 600, Icon.getPixelsPtr());
	sf::Cursor cursor;
	sf::Image image_cursor;
	std::string cursorEiei = "Pic/osu/cursor.png";
	image_cursor.loadFromFile(cursorEiei);

	if (cursor.loadFromPixels(image_cursor.getPixelsPtr(), image_cursor.getSize(), { 50, 50 })) {
		window.setMouseCursor(cursor);
		std::cout << "เปลี่ยนสำเร็จ" << std::endl;
	}
	else std::cout << "เปลี่ยนไม่สำเร็จ" << std::endl;

	// run the program as long as the window is open
	sf::Event event;

	//wallpaper
	sf::Texture background;
	if (!background.loadFromFile("Pic/osu/background.jpg"))
	{
		// error...
		std::cout << "Load Failed" << std::endl;
		system("pause");
	}
	sf::Sprite spritebackground;
	spritebackground.setTexture(background);
	spritebackground.setScale(sf::Vector2f(1.f, 1.f)); // absolute scale factor
	spritebackground.setPosition(sf::Vector2f(0.f, 0.f)); // absolute position
	background.setSmooth(true);

	sf::Texture wallpaper;
	if (!wallpaper.loadFromFile("Pic/kimetsu.png"))
	{
		// error...
		std::cout << "Load Failed" << std::endl;
		system("pause");
	}
	sf::Sprite spritewallpaper;
	spritewallpaper.setTexture(wallpaper);
	spritewallpaper.setScale(sf::Vector2f(0.63f, 0.8f)); // absolute scale factor
	spritewallpaper.setPosition(sf::Vector2f(210.f, 0.f)); // absolute position
	wallpaper.setSmooth(true);

	sf::Texture wallpaperboruto;
	if (!wallpaperboruto.loadFromFile("Pic/boruto.jpg"))
	{
		// error...
		std::cout << "Load Failed" << std::endl;
		system("pause");
	}
	sf::Sprite spritewallpaperboruto;
	spritewallpaperboruto.setTexture(wallpaperboruto);
	spritewallpaperboruto.setScale(sf::Vector2f(0.32f, 0.4f)); // absolute scale factor
	spritewallpaperboruto.setPosition(sf::Vector2f(210.f, 0.f)); // absolute position
	wallpaperboruto.setSmooth(true);

	//nezuko
	int frame = 0;
	int row = 0;
	sf::Texture spriteSheet;
	sf::Sprite playerSprite;
	int frameCounter = 0;
	if (!spriteSheet.loadFromFile("Pic/nezuko.png"))
	{
		std::cout << "ERROR" << std::endl;
	}
	playerSprite.setTexture(spriteSheet);

	//song
	sf::SoundBuffer Lisasong;
	if (!Lisasong.loadFromFile("Song/Gurenge_LiSA .wav"))
	{
		std::cout << "ERROR" << std::endl;
	}
	sf::SoundBuffer Sayonaramoon;
	if (!Sayonaramoon.loadFromFile("Song/sayonaramoon.wav"))
	{
		std::cout << "ERROR" << std::endl;
	}
	if (!Lisasong.loadFromFile("Song/Gurenge_LiSA .wav"))
	{
		std::cout << "ERROR" << std::endl;
	}
	sf::Sound sound;
	bool soundplay;
	soundplay = false;

	//drum
	sf::SoundBuffer drum;
	if (!drum.loadFromFile("Song/drum.wav"))
	{
		std::cout << "ERROR" << std::endl;
	}
	sf::Sound sounddrum;
	sounddrum.setBuffer(drum);
	sounddrum.setVolume(40);

	//button
	sf::Texture button;
	sf::Texture buttonpress;
	if (!button.loadFromFile("Pic/buttongameplay.png"))
	{
		// error...
		std::cout << "Load Failed" << std::endl;
		system("pause");
	}
	if (!buttonpress.loadFromFile("Pic/buttongameplaypress.png"))
	{
		// error...
		std::cout << "Load Failed" << std::endl;
		system("pause");
	}
	sf::Sprite spritebutton[4];
	button.setSmooth(true);
	buttonpress.setSmooth(true);

	//line 
	sf::RectangleShape line1(sf::Vector2f(850.f, 2.f));
	sf::RectangleShape line2(sf::Vector2f(850.f, 2.f));
	sf::RectangleShape line3(sf::Vector2f(850.f, 2.f));
	sf::RectangleShape line4(sf::Vector2f(850.f, 2.f));
	sf::RectangleShape line5(sf::Vector2f(850.f, 2.f));
	line1.rotate(90.f);
	line2.rotate(90.f);
	line3.rotate(90.f);
	line4.rotate(90.f);
	line5.rotate(90.f);
	line1.setPosition(sf::Vector2f(210.f, 0.f)); // absolute position
	line2.setPosition(sf::Vector2f(503.f, 0.f)); // absolute position
	line3.setPosition(sf::Vector2f(816.f, 0.f)); // absolute position
	line4.setPosition(sf::Vector2f(1129.f, 0.f)); // absolute position
	line5.setPosition(sf::Vector2f(1422.f, 0.f)); // absolute position

	//Time
	sf::Clock clock;
	int time;
	sf::Time elapsed;

	//combo
	int comboscore = 0;
	sf::Font fontcombo;
	fontcombo.loadFromFile("fontscore.ttf");

	std::ostringstream ssScore;
	ssScore << "Combo: " << comboscore;

	sf::Text combo;
	combo.setCharacterSize(30);
	combo.setPosition({ 10,10 });
	combo.setString(ssScore.str());
	combo.setFont(fontcombo);

	//mode
	int openkey[2] = { 0,0 };
	int direcy[2] = { randomkey(),randomkey() };
	float usekeytime = 1.55521;
	int direckeydown[2] = { 0,0 };
	sf::Texture note6;
	sf::Texture note4;
	if (!note6.loadFromFile("Pic/osu/mania-note6.png"))
	{
		// error...
		std::cout << "Load Failed" << std::endl;
		system("pause");
	}
	if (!note4.loadFromFile("Pic/osu/mania-note4.png"))
	{
		// error...
		std::cout << "Load Failed" << std::endl;
		system("pause");
	}
	sf::Sprite spritenote6[2];
	sf::Sprite spritenote4[2];

	//menufirst
	int gamemode = 1;
	sf::Texture button_menu[2];
	if (!button_menu[0].loadFromFile("Pic/buttonplay.png"))
	{
		// error...
		std::cout << "Load Failed" << std::endl;
		system("pause");
	}
	if (!button_menu[1].loadFromFile("Pic/buttonhowtoplay.png"))
	{
		// error...
		std::cout << "Load Failed" << std::endl;
		system("pause");
	}
	sf::Sprite spritebutton_munu[2];
	spritebutton_munu[0].setTexture(button_menu[0]);
	spritebutton_munu[0].setPosition(sf::Vector2f(700.f, 200.f));
	spritebutton_munu[0].setScale(sf::Vector2f(0.2f, 0.2f));
	spritebutton_munu[1].setTexture(button_menu[1]);
	spritebutton_munu[1].setPosition(sf::Vector2f(700.f, 400.f));
	spritebutton_munu[1].setScale(sf::Vector2f(0.2f, 0.2f));
	button_menu[0].setSmooth(true);
	button_menu[1].setSmooth(true);

	//menusong
	int Song = 0;
	sf::Texture namesong[2];
	if (!namesong[0].loadFromFile("Pic/namesong/gurenge.png"))
	{
		std::cout << "Load Failed" << std::endl;
		system("pause");
	}
	if (!namesong[1].loadFromFile("Pic/namesong/sayonaramoon.png"))
	{
		std::cout << "Load Failed" << std::endl;
		system("pause");
	}
	sf::Texture back;
	if (!back.loadFromFile("Pic/osu/back.png"))
	{
		std::cout << "Load Failed" << std::endl;
		system("pause");
	}
	sf::Sprite spriteback;
	spriteback.setTexture(back);
	spriteback.setPosition(sf::Vector2f(0.f, 760.f));
	spriteback.setScale(sf::Vector2f(1.f, 1.f));
	namesong[1].setSmooth(true);
	sf::Sprite spritenamesong[2];
	spritenamesong[1].setTexture(namesong[1]);
	spritenamesong[1].setPosition(sf::Vector2f(600.f, 200.f));
	spritenamesong[1].setScale(sf::Vector2f(2.f, 1.2f));
	namesong[1].setSmooth(true);
	spritenamesong[0].setTexture(namesong[0]);
	spritenamesong[0].setPosition(sf::Vector2f(600.f, 200.f));
	spritenamesong[0].setScale(sf::Vector2f(2.f, 1.2f));
	namesong[0].setSmooth(true);

	//file
	string line;
	ifstream gurengefile("file/gurenge.txt");

	//get username
	sf::RectangleShape usernameblock;
	usernameblock.setSize(sf::Vector2f(300.0f, 70.0f));
	usernameblock.setOrigin(sf::Vector2f(150.0f, 35.0f));
	usernameblock.setPosition(sf::Vector2f(800, 450.0f));

	sf::RectangleShape cursorusernameblock;
	cursorusernameblock.setSize(sf::Vector2f(5.f, 64.0f));
	cursorusernameblock.setOrigin(sf::Vector2f(2.5f, 32.0f));
	cursorusernameblock.setPosition(sf::Vector2f(655, 450.0f));
	cursorusernameblock.setFillColor(sf::Color::Black);

	float totalTime = 0;
	sf::Clock clockcursor;
	bool stateclock = false;

	// state 0
	sf::Text nameDisplay; // Text เอาไว้แสดงที่คนเล่นพิมพ์
	nameDisplay.setCharacterSize(30);
	nameDisplay.setPosition({ 10,10 });
	nameDisplay.setString("");
	nameDisplay.setFont(fontcombo);

	string s = ""; // เอาไว้เก็บที่คนเล่นพิมพ์มาจะได้แสดงใน Text
	std::ofstream fileWriter; /*ตัวอ่านไฟล์*/

	while (window.isOpen())
	{
		elapsed = clock.getElapsedTime();
		if (gamemode == 0)
		{
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::TextEntered) /*อันนี้เอาไว้เช็คเวลาคนเล่นพิมพ์*/
				{
					if (event.text.unicode == 13) // ถ้าคนเล่นกด enter
					{
						/* จะเซฟลงไฟล์ */
						fileWriter.open("save/score.txt", std::ios::out | std::ios::app);
						fileWriter << s /*ชื่อ*/ << "," << "5000"/*คะแนน*/ << '\n';
					}
					else if (event.text.unicode == 8) //backspace
					{ 
						/*เวลากดลบจะลบตัวล่าสุดออก*/
						s.pop_back(); /*s เป็น string ถ้าอยากลบตัวหลังสุดออกก็ใช้ pop_back() สมมติ abcdef ถ้า pop_back() จะกลายเป๋น abcde */
						nameDisplay.setString(s); /*สั่งให้มันเอา s ไปแสดง*/
					}
					else 
					{
						s += event.text.unicode; /*ถ้ากดปุ่มอื่นๆที่ไม่ใช่ enter backspace แสดงว่าพิมพ์ชื่อ ก็เอาตัวนั้นไปบวกกับ s*/
						/*สมมติ s เป็น abcd ถ้ากด b มันจะกลายเป็น abcdb*/
						nameDisplay.setString(s);
					}
				}
			}
			window.clear();
			window.draw(nameDisplay);
			window.display();
		}

		if (gamemode == 1)
		{
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
				}
			}
			if (sf::Mouse::getPosition(window).x >= 730 && sf::Mouse::getPosition(window).x <= 1153 && sf::Mouse::getPosition(window).y >= 253 && sf::Mouse::getPosition(window).y <= 366)
			{
				spritebutton_munu[0].setPosition(sf::Vector2f(730.f, 220.f));
				spritebutton_munu[0].setScale(sf::Vector2f(0.18f, 0.18f));
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					gamemode = 2;
					elapsed = clock.restart();
				}
			}
			else
			{
				spritebutton_munu[0].setPosition(sf::Vector2f(700.f, 200.f));
				spritebutton_munu[0].setScale(sf::Vector2f(0.2f, 0.2f));
			}
			if (sf::Mouse::getPosition(window).x >= 730 && sf::Mouse::getPosition(window).x <= 1153 && sf::Mouse::getPosition(window).y >= 490 && sf::Mouse::getPosition(window).y <= 565)
			{
				spritebutton_munu[1].setPosition(sf::Vector2f(730.f, 420.f));
				spritebutton_munu[1].setScale(sf::Vector2f(0.18f, 0.18f));
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					gamemode = 3;
				}
			}
			else
			{
				spritebutton_munu[1].setPosition(sf::Vector2f(700.f, 400.f));
				spritebutton_munu[1].setScale(sf::Vector2f(0.2f, 0.2f));
			}


			window.clear();
			window.draw(spritebackground);
			window.draw(spritebutton_munu[0]);
			window.draw(spritebutton_munu[1]);
			window.display();
		}
		else if (gamemode == 2)
		{
			if (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
				}
			}
			if (sf::Mouse::getPosition(window).x >= 0 && sf::Mouse::getPosition(window).x <= 157 && sf::Mouse::getPosition(window).y >= 751 && sf::Mouse::getPosition(window).y <= 820)
			{
				if (elapsed.asSeconds() > 1 && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					gamemode = 1;
				}
			}
			if (sf::Mouse::getPosition(window).x >= 825 && sf::Mouse::getPosition(window).x <= 1850 && sf::Mouse::getPosition(window).y >= 226 && sf::Mouse::getPosition(window).y <= 326)
			{
				spritenamesong[0].setPosition(sf::Vector2f(500.f, 200.f));
				if (elapsed.asSeconds() > 1 && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					Song = 1;
					gamemode = 4;
					soundplay = true;
				}
			}
			else
			{
				spritenamesong[0].setPosition(sf::Vector2f(600.f, 200.f));
			}
			if (sf::Mouse::getPosition(window).x >= 825 && sf::Mouse::getPosition(window).x <= 1850 && sf::Mouse::getPosition(window).y >= 430 && sf::Mouse::getPosition(window).y <= 522)
			{
				spritenamesong[1].setPosition(sf::Vector2f(500.f, 400.f));
				if (elapsed.asSeconds() > 1 && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					Song = 2;
					gamemode = 4;
					soundplay = true;
				}
			}
			else
			{
				spritenamesong[1].setPosition(sf::Vector2f(600.f, 400.f));
			}
			std::cout << sf::Mouse::getPosition(window).x << std::endl;

			window.clear();
			window.draw(spritebackground);
			window.draw(spriteback);
			window.draw(spritenamesong[0]);
			window.draw(spritenamesong[1]);
			window.display();

		}
		else if (gamemode == 3)
		{
			if (sf::Mouse::getPosition(window).x >= 0 && sf::Mouse::getPosition(window).x <= 157 && sf::Mouse::getPosition(window).y >= 751 && sf::Mouse::getPosition(window).y <= 820)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					gamemode = 1;
				}
			}
			window.clear();
			window.draw(spritebackground);
			window.draw(spriteback);
			window.display();
		}
		else if (gamemode == 4)//เริ่มเล่นเกม 
		{
			if (Song == 1)
			{
				if (soundplay == true)
				{
					sound.setBuffer(Lisasong);
					sound.setVolume(100);
					sound.play();
					elapsed = clock.restart();
					soundplay = false;
				}
				if (elapsed.asSeconds() >= 91)
				{
					Song = 0;
					gamemode = 2;
					elapsed = clock.restart();
				}
				if (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
					{
						window.close();
					}

					if (event.key.code == 100)
					{
						if (direckeydown[1] > 750 && direckeydown[1] <= 800)
						{
							comboscore++;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());

						}
						else if (direckeydown[0] > 750 && direckeydown[0] <= 800)
						{
							comboscore++;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						else
						{
							comboscore = 0;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						spritebutton[0].setTexture(buttonpress);
						spritebutton[0].setScale(sf::Vector2f(0.35f, 0.40f));
						spritebutton[0].setPosition(sf::Vector2f(240.f, 750.f));
						sounddrum.play();
					}
					else if (event.key.code == 102)
					{
						if (direckeydown[1] > 750 && direckeydown[1] <= 800)
						{
							comboscore++;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						else if (direckeydown[0] > 750 && direckeydown[0] <= 800)
						{
							comboscore++;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						else
						{
							comboscore = 0;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						spritebutton[1].setTexture(buttonpress);
						spritebutton[1].setScale(sf::Vector2f(0.35f, 0.40f));
						spritebutton[1].setPosition(sf::Vector2f(545.f, 750.f));
						sounddrum.play();
					}
					else if (event.key.code == 104)
					{
						if (direckeydown[1] > 750 && direckeydown[1] <= 800)
						{
							comboscore++;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						else if (direckeydown[0] > 750 && direckeydown[0] <= 800)
						{
							comboscore++;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						else
						{
							comboscore = 0;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						spritebutton[2].setTexture(buttonpress);
						spritebutton[2].setScale(sf::Vector2f(0.35f, 0.40f));
						spritebutton[2].setPosition(sf::Vector2f(860.f, 750.f));
						sounddrum.play();
					}
					else if (event.key.code == 106)
					{
						if (direckeydown[1] > 750 && direckeydown[1] <= 800)
						{
							comboscore++;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						else if (direckeydown[0] > 750 && direckeydown[0] <= 800)
						{
							comboscore++;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						else
						{
							comboscore = 0;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						spritebutton[3].setTexture(buttonpress);
						spritebutton[3].setScale(sf::Vector2f(0.35f, 0.40f));
						spritebutton[3].setPosition(sf::Vector2f(1165.f, 750.f));
						sounddrum.play();
					}
					else
					{
						spritebutton[0].setTexture(button);
						spritebutton[1].setTexture(button);
						spritebutton[2].setTexture(button);
						spritebutton[3].setTexture(button);
						spritebutton[0].setScale(sf::Vector2f(0.4f, 0.47f));
						spritebutton[1].setScale(sf::Vector2f(0.4f, 0.47f));
						spritebutton[2].setScale(sf::Vector2f(0.4f, 0.47f));
						spritebutton[3].setScale(sf::Vector2f(0.4f, 0.47f));
						spritebutton[0].setPosition(sf::Vector2f(223.f, 735.f));
						spritebutton[1].setPosition(sf::Vector2f(529.f, 735.f));
						spritebutton[2].setPosition(sf::Vector2f(842.f, 735.f));
						spritebutton[3].setPosition(sf::Vector2f(1148.f, 735.f));
					}
				}
				elapsed = clock.getElapsedTime();
				time = (int)elapsed.asSeconds();
				//printf("%d", time);

				playerSprite.setTextureRect(sf::IntRect(321 * frame, row, 321, 720));
				if (frameCounter == 200)
				{
					frame = (frame + 1) % 4;
					frameCounter = 0;
				}
				frameCounter++;
				playerSprite.setPosition(sf::Vector2f(1500.f, 165.f));
				playerSprite.setScale(sf::Vector2f(1.f, 1.f));

				if (time % 3 == 0)
				{
					openkey[0] = 1;
				}
				else if (time % 1 == 0)
				{
					openkey[1] = 1;
				}

				if (openkey[0] == 1)
				{
					if (direckeydown[0] < 800)
					{
						spritenote6[0].setTexture(note6);
						note6.setSmooth(true);
						spritenote6[0].setScale(sf::Vector2f(0.75f, 0.60f));
						spritenote6[0].setPosition(sf::Vector2f(direcy[0], direckeydown[0]));
						direckeydown[0]++;

					}
					if (direckeydown[0] == 800)
					{
						direckeydown[0] = 0;
						direcy[0] = randomkey2();
					}
				}
				if (openkey[1] == 1)
				{
					if (direckeydown[1] < 800)
					{
						spritenote6[1].setTexture(note6);
						note6.setSmooth(true);
						spritenote6[1].setScale(sf::Vector2f(0.75f, 0.60f));
						spritenote6[1].setPosition(sf::Vector2f(direcy[1], direckeydown[1]));
						direckeydown[1]++;

					}
					if (direckeydown[1] == 800)
					{
						direckeydown[1] = 0;
						direcy[1] = randomkey();
					}
				}

				window.clear();
				window.draw(spritebackground);
				window.draw(spritewallpaper);
				window.draw(playerSprite);
				window.draw(combo);

				window.draw(spritenote6[0]);
				window.draw(spritenote6[1]);

				window.draw(spritebutton[0]);
				window.draw(spritebutton[1]);
				window.draw(spritebutton[2]);
				window.draw(spritebutton[3]);
				window.draw(line1);
				window.draw(line2);
				window.draw(line3);
				window.draw(line4);
				window.draw(line5);

				window.display();

			}
			if (Song == 2)
			{
				if (soundplay == true)
				{
					sound.setBuffer(Sayonaramoon);
					sound.setVolume(70);
					sound.play();
					elapsed = clock.restart();
					soundplay = false;
				}
				if (elapsed.asSeconds() >= 91)
				{
					Song = 0;
					gamemode = 2;
					elapsed = clock.restart();
				}
				if (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
					{
						window.close();
					}
					if (event.key.code == 100)
					{
						if (direckeydown[1] > 750 && direckeydown[1] <= 800)
						{
							comboscore++;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());

						}
						else if (direckeydown[0] > 750 && direckeydown[0] <= 800)
						{
							comboscore++;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						else
						{
							comboscore = 0;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						spritebutton[0].setTexture(buttonpress);
						spritebutton[0].setScale(sf::Vector2f(0.35f, 0.40f));
						spritebutton[0].setPosition(sf::Vector2f(240.f, 750.f));
						sounddrum.play();
					}
					else if (event.key.code == 102)
					{
						if (direckeydown[1] > 750 && direckeydown[1] <= 800)
						{
							comboscore++;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						else if (direckeydown[0] > 750 && direckeydown[0] <= 800)
						{
							comboscore++;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						else
						{
							comboscore = 0;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						spritebutton[1].setTexture(buttonpress);
						spritebutton[1].setScale(sf::Vector2f(0.35f, 0.40f));
						spritebutton[1].setPosition(sf::Vector2f(545.f, 750.f));
						sounddrum.play();
					}
					else if (event.key.code == 104)
					{
						if (direckeydown[1] > 750 && direckeydown[1] <= 800)
						{
							comboscore++;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						else if (direckeydown[0] > 750 && direckeydown[0] <= 800)
						{
							comboscore++;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						else
						{
							comboscore = 0;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						spritebutton[2].setTexture(buttonpress);
						spritebutton[2].setScale(sf::Vector2f(0.35f, 0.40f));
						spritebutton[2].setPosition(sf::Vector2f(860.f, 750.f));
						sounddrum.play();
					}
					else if (event.key.code == 106)
					{
						if (direckeydown[1] > 750 && direckeydown[1] <= 800)
						{
							comboscore++;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						else if (direckeydown[0] > 750 && direckeydown[0] <= 800)
						{
							comboscore++;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						else
						{
							comboscore = 0;
							ssScore.str("");
							ssScore << "Combo: " << comboscore;
							combo.setString(ssScore.str());
						}
						spritebutton[3].setTexture(buttonpress);
						spritebutton[3].setScale(sf::Vector2f(0.35f, 0.40f));
						spritebutton[3].setPosition(sf::Vector2f(1165.f, 750.f));
						sounddrum.play();
					}
					else
					{
						spritebutton[0].setTexture(button);
						spritebutton[1].setTexture(button);
						spritebutton[2].setTexture(button);
						spritebutton[3].setTexture(button);
						spritebutton[0].setScale(sf::Vector2f(0.4f, 0.47f));
						spritebutton[1].setScale(sf::Vector2f(0.4f, 0.47f));
						spritebutton[2].setScale(sf::Vector2f(0.4f, 0.47f));
						spritebutton[3].setScale(sf::Vector2f(0.4f, 0.47f));
						spritebutton[0].setPosition(sf::Vector2f(223.f, 735.f));
						spritebutton[1].setPosition(sf::Vector2f(529.f, 735.f));
						spritebutton[2].setPosition(sf::Vector2f(842.f, 735.f));
						spritebutton[3].setPosition(sf::Vector2f(1148.f, 735.f));
					}
				}
				elapsed = clock.getElapsedTime();
				time = (int)elapsed.asSeconds();
				//printf("%d", time);

				playerSprite.setTextureRect(sf::IntRect(321 * frame, row, 321, 720));
				if (frameCounter == 200)
				{
					frame = (frame + 1) % 4;
					frameCounter = 0;
				}
				frameCounter++;
				playerSprite.setPosition(sf::Vector2f(1500.f, 165.f));
				playerSprite.setScale(sf::Vector2f(1.f, 1.f));

				if (time % 3 == 0)
				{
					openkey[0] = 1;
				}
				else if (time % 1 == 0)
				{
					openkey[1] = 1;
				}

				if (openkey[0] == 1)
				{
					if (direckeydown[0] < 800)
					{
						spritenote4[0].setTexture(note4);
						note6.setSmooth(true);
						spritenote4[0].setScale(sf::Vector2f(0.75f, 0.60f));
						spritenote4[0].setPosition(sf::Vector2f(direcy[0], direckeydown[0]));
						direckeydown[0]++;

					}
					if (direckeydown[0] == 800)
					{
						direckeydown[0] = 0;
						direcy[0] = randomkey2();
					}
					if (direckeydown[0] < 800)
					{
						spritenote4[0].setTexture(note4);
						note6.setSmooth(true);
						spritenote4[0].setScale(sf::Vector2f(0.75f, 0.60f));
						spritenote4[0].setPosition(sf::Vector2f(direcy[0], direckeydown[0]));
						direckeydown[0]++;

					}
					if (direckeydown[0] == 800)
					{
						direckeydown[0] = 0;
						direcy[0] = randomkey2();
					}
				}
				if (openkey[1] == 1)
				{
					if (direckeydown[1] < 800)
					{
						spritenote4[1].setTexture(note4);
						note6.setSmooth(true);
						spritenote4[1].setScale(sf::Vector2f(0.75f, 0.60f));
						spritenote4[1].setPosition(sf::Vector2f(direcy[1], direckeydown[1]));
						direckeydown[1]++;
					}
					if (direckeydown[1] == 800)
					{
						direckeydown[1] = 0;
						direcy[1] = randomkey();
					}
					if (direckeydown[1] < 800)
					{
						spritenote4[1].setTexture(note4);
						note6.setSmooth(true);
						spritenote4[1].setScale(sf::Vector2f(0.75f, 0.60f));
						spritenote4[1].setPosition(sf::Vector2f(direcy[1], direckeydown[1]));
						direckeydown[1]++;
					}
					if (direckeydown[1] == 800)
					{
						direckeydown[1] = 0;
						direcy[1] = randomkey();
					}
				}

				window.clear();
				window.draw(spritebackground);
				window.draw(spritewallpaperboruto);
				window.draw(playerSprite);
				window.draw(combo);

				window.draw(spritenote4[0]);
				window.draw(spritenote4[1]);

				window.draw(spritebutton[0]);
				window.draw(spritebutton[1]);
				window.draw(spritebutton[2]);
				window.draw(spritebutton[3]);
				window.draw(line1);
				window.draw(line2);
				window.draw(line3);
				window.draw(line4);
				window.draw(line5);

				window.display();
			}
		}

	}

	return 0;
}