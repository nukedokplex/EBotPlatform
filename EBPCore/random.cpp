#include <iostream>
#include <ctime>
using namespace std;

int Random_Get(int min, int max) {
	if (min > max) {
		min *= -1;
		max *= -1;
	}
	srand(time(0));
	return min + rand() % (max-min);
}