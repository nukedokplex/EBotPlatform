#pragma once
#include <string>

int Random_Get(int min, int max);

bool Flags_IsFlag(int flags, int flag);
int Flags_AddFlag(int flags, int flag);
int Flags_RemoveFlag(int flags, int flag);

std::string Charset_Change(std::string str, int from, int to);

namespace utils
{
	void init();
	namespace regular
	{
		std::string replace(std::string text, std::string pattern, std::string to);
	}
	namespace random
	{
		void init();
		int get(int min, int max);
	}
	namespace string
	{
		std::string lower(std::string str);
		std::string upper(std::string str);
	}
}