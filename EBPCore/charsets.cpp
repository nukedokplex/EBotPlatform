#include <codecvt>
#include <locale>
#include <string>
#include <clocale>
#include <locale>
#include "console.h"
#include "utf8\utf8.h"
#include <windows.h> 

using namespace std;

/*
std::string VoidToString(void *voids) {
	std::setlocale(LC_ALL, "");
	std::u32string utf32str ((char *)voids, strlen((char *)voids));
	std::string str(utf32str.begin(), utf32str.end());
	Console_Log(str, "123");
	return str;
}*/

bool Charset_IsUTF8(const char *str) {
	string line = str;
	string::iterator end_it = utf8::find_invalid(line.begin(), line.end());
	if (end_it != line.end()) {
		return false;
	}
	return true;
}

string Utf8_to_cp1251(const char *str)
{
	string res;
	int result_u, result_c;

	if (!Charset_IsUTF8(str))
		return str;

	result_u = MultiByteToWideChar(CP_UTF8,
		0,
		str,
		-1,
		0,
		0);

	if (!result_u)
		return 0;

	wchar_t *ures = new wchar_t[result_u];

	if (!MultiByteToWideChar(CP_UTF8,
		0,
		str,
		-1,
		ures,
		result_u))
	{
		delete[] ures;
		return 0;
	}


	result_c = WideCharToMultiByte(
		1251,
		0,
		ures,
		-1,
		0,
		0,
		0, 0);

	if (!result_c)
	{
		delete[] ures;
		return 0;
	}

	char *cres = new char[result_c];

	if (!WideCharToMultiByte(
		1251,
		0,
		ures,
		-1,
		cres,
		result_c,
		0, 0))
	{
		delete[] cres;
		return 0;
	}
	delete[] ures;
	res.append(cres);
	delete[] cres;
	return res;
}