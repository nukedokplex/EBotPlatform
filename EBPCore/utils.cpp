#include <regex>
#include <random>
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
mt19937_64 mersenne;

void utils::random::init()
{
	mersenne.seed(time(0));
}

int utils::random::get(int min, int max) {
	if (min > max) {
		min *= -1;
		max *= -1;
	}
	if (min == max) return min;

	std::uniform_int_distribution<> uid(min, max);
	return uid(mersenne);
}

// REGULAR
std::string utils::regular::replace(std::string text, std::string pattern, std::string to)
{
	std::tr1::regex rx(pattern);
	return std::tr1::regex_replace(text, rx, to);
}