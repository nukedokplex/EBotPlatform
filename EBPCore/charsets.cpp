#ifdef __linux__ 
	#include "utf8/utf8.h" // utf8::find_invalid
	#include "iconvlite.h"
#elif _WIN32
	#include "utf8\utf8.h" // utf8::find_invalid
	#include <windows.h> // MultiByteToWideChar
#endif

using namespace std;

bool Charset_IsUTF8(const char *str)
{
	string line = str;
	string::iterator end_it = utf8::find_invalid(line.begin(), line.end());
	if (end_it != line.end()) {
		return false;
	}
	return true;
}

string Charset_Change(string str, int from, int to)
{
	string res;
	#ifdef _WIN32 
	int result_u, result_c;

	result_u = MultiByteToWideChar(from, 0, str.c_str(), -1, 0, 0);
	if (!result_u)
		return 0;
	wchar_t *ures = new wchar_t[result_u];
	if (!MultiByteToWideChar(from,0,str.c_str(),-1,ures,result_u))
	{
		delete[] ures;
		return 0;
	}

	result_c = WideCharToMultiByte(to,0,ures,-1,0,0,0,0);
	if (!result_c)
	{
		delete[] ures;
		return 0;
	}
	char *cres = new char[result_c];
	if (!WideCharToMultiByte(to,0,ures,-1,cres,result_c,0, 0))
	{
		delete[] cres;
		return 0;
	}
	delete[] ures;
	res.append(cres);
	delete[] cres;
	return res;
	#elif __linux__ 
	convert_utf8_to_windows1251(str, res);
	return res;
	#endif
}