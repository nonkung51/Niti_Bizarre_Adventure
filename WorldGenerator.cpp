#pragma warning(disable : 4996)
#include "WorldGenerator.h"


void WorldGenerator::CreateMatrix()
{
	srand(static_cast<unsigned int>(time(NULL)));

	for (int y = 0; y < 88; y++)
	{
		for (int x = 0; x < 88; x++)
		{
			_Matrix[y][x] = 0;
		}
	}

	for (int y = 0; y < 88; y = y + 8)
	{
		for (int x = 0; x < 88; x = x + 8)
		{
			/* Fill Matrix with random values */
			_Matrix[y][x] = rand() % 100;
		}
	}
}

void WorldGenerator::Interpolation()
{
	/* 1. Interpolation */
	for (int y = 0; y < 88; y = y + 8)
	{
		for (int x = 0; x < 88; x = x + 8)
		{
			float Difference = static_cast<float>(_Matrix[y][x + 8] - _Matrix[y][x]);
			float InterpolationValue = Difference / 8.0f;

			int counter = 0;
			for (int c = x; c < x + 8; c++)
			{
				_Matrix[y][c] = static_cast<int>(_Matrix[y][x] + (InterpolationValue * counter));
				counter++;
			}
		}
	}

	/* 2. Interpolation */
	for (int x = 0; x < 88; x++)
	{
		for (int y = 0; y < 88; y = y + 8)
		{
			float Difference = static_cast<float>(_Matrix[y + 8][x] - _Matrix[y][x]);
			float InterpolationValue = Difference / 8.0f;

			int counter = 0;
			for (int c = y; c < y + 8; c++)
			{
				_Matrix[c][x] = static_cast<int>(_Matrix[y][x] + (InterpolationValue * counter));
				counter++;
			}
		}
	}
}

void WorldGenerator::CreateGraphics()
{
	std::cout << "Creating graphics... " << std::endl;
	for (int y = 0; y < 88; y++)
	{
		for (int x = 0; x < 88; x++)
		{
			_Graphics.push_back(new sf::RectangleShape);
			_Graphics[_Graphics.size() - 1]->setSize(sf::Vector2f(64, 64));
			_Graphics[_Graphics.size() - 1]->setPosition(x * 64.0f, y * 64.0f);

			/* Water */
			if (_Matrix[y][x] < 35)
			{
				_Graphics[_Graphics.size() - 1]->setFillColor(sf::Color(0, 157, 255));
			}

			/* Sand - Beaches */
			if (_Matrix[y][x] > 34 && _Matrix[y][x] < 50)
			{
				_Graphics[_Graphics.size() - 1]->setFillColor(sf::Color(255, 230, 0));
			}

			/* Grass */
			if (_Matrix[y][x] > 49 && _Matrix[y][x] < 85)
			{
				_Graphics[_Graphics.size() - 1]->setFillColor(sf::Color(0, 145, 0));
			}

			/* Rocks - Stones */
			if (_Matrix[y][x] > 84)
			{
				_Graphics[_Graphics.size() - 1]->setFillColor(sf::Color(80, 80, 80));
			}
		}
	}

	std::cout << "Set " << _Graphics.size() << " sprites" << std::endl;
}

void WorldGenerator::Render(sf::RenderWindow* window)
{
	for (unsigned int c = 0; c < _Graphics.size(); c++)
	{
		window->draw(*_Graphics[c]);
	}
}
