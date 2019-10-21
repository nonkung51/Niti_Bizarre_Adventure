#include "helper.h"
#include <stdlib.h>
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
