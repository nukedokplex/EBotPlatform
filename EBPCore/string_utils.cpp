#include <string> 
#include <codecvt>
#include <locale> 
#include "common.h"
#include "utils.h"

#ifdef __linux__ 
	std::locale const utf8("C");
#elif _WIN32
	std::locale const utf8("rus");
#endif

// Convert UTF-8 byte string to wstring
std::wstring to_wstring(std::string const& s) {
	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
	return conv.from_bytes(s);
}

// Convert wstring to UTF-8 byte string
std::string to_string(std::wstring const& s) {
	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
	return conv.to_bytes(s);
}


std::string utils::string::lower(std::string str)
{
	auto ss = to_wstring(str);
	for (auto& c : ss) {
		c = std::tolower(c, utf8);
	}
	return to_string(ss);
}

std::string utils::string::upper(std::string str)
{
	auto ss = to_wstring(str);
	for (auto& c : ss) {
		c = std::toupper(c, utf8);
	}
	return to_string(ss);
}