#include <codecvt>
#include <locale>
#include <string>
#include <clocale>
#include <locale>
#include "console.h"
#include "utf8\utf8.h"
#include <windows.h> 

/*
std::string VoidToString(void *voids) {
	std::setlocale(LC_ALL, "");
	std::u32string utf32str ((char *)voids, strlen((char *)voids));
	std::string str(utf32str.begin(), utf32str.end());
	Console_Log(str, "123");
	return str;
}*/

void fix_utf8_string(std::string& str)
{
}