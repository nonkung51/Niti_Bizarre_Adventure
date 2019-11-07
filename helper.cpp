#include "helper.h"
#include <stdlib.h>
#include <cmath>
#include <time.h>

int generateRandom(int max) {
	int randomNumber = rand();
	return (randomNumber % max) + 1;
}

int generateRandom0(int max) {
	int randomNumber = rand();
	return (randomNumber % max);
}

bool generateRandomBool() {
	int randomNumber = rand();
	return (randomNumber % 2) == 1;
}

float distance(float xa, float ya, float xb, float yb) {
	float dX = xa - xb;
	float dY = ya - yb;

	return sqrt((dX*dX) + (dY*dY));
}
