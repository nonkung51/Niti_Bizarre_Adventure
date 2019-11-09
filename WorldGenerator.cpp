#pragma warning(disable : 4996)
#include "WorldGenerator.h"


void WorldGenerator::CreateMatrix()
{
	srand(static_cast<unsigned int>(time(NULL)));

	for (int y = 0; y < 64; y++)
	{
		for (int x = 0; x < 64; x++)
		{
			_Matrix[y][x] = 0;
		}
	}
	//start from 16 end at 72 to create a border
	for (int y = 16; y < 48; y = y + 8)
	{
		for (int x = 16; x < 48; x = x + 8)
		{
			/* Fill Matrix with random values */
			_Matrix[y][x] = rand() % 100;
		}
	}
}

void WorldGenerator::Interpolation()
{
	/* 1. Interpolation */
	for (int y = 0; y < 64; y = y + 8)
	{
		for (int x = 0; x < 64; x = x + 8)
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
	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 64; y = y + 8)
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

	//create border
	/*for (int x = 0; x < 88; x++) {
		for (int y = 0; y < 88; y++ ) {
			if (x + y >= 140) {
				_Matrix[y][x] = 0;
			}
		}
	}*/
}