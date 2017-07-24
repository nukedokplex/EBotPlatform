#include <regex>
#include "utils.h"

std::string utils::regular::replace(std::string text, std::string pattern, std::string to)
{
	std::tr1::regex rx(pattern);
	return std::tr1::regex_replace(text, rx, to);
}
