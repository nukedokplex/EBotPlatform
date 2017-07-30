#include <regex>
#include <ctime>

#include "utils.h"

using namespace std;

void utils::init()
{
	utils::random::init();
}

// FLAGS
bool Flags_IsFlag(int flags, int flag) {
	return flags & flag;
}

int Flags_AddFlag(int flags, int flag) {
	return flags + flag;
}

int Flags_RemoveFlag(int flags, int flag) {
	return flags - flag;
}

// RANDOM
void utils::random::init()
{
	srand(time(0));
}

int utils::random::get(int min, int max) {
	if (min > max) {
		min *= -1;
		max *= -1;
	}
	srand(time(0));
	return min + rand() % (max - min);
}

// REGULAR
std::string utils::regular::replace(std::string text, std::string pattern, std::string to)
{
	std::tr1::regex rx(pattern);
	return std::tr1::regex_replace(text, rx, to);
}