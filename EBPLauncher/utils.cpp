#include <iostream>
#include <cstdlib> // для system
#include "../EBPCore/ebp_api.h"
#include <string>
#include <clocale>
#include <conio.h> //Необходимые библиотеки
#include <stdio.h>
#include <locale>
#include <windows.h> 
#include "../EBPCore/utf8\utf8.h"

using namespace std;

extern void WriteConsoleLine(std::string text);
void SetWindowTitle(std::string text);

inputApi api_input;
outputApi api_output = {
	WriteConsoleLine,
	SetWindowTitle
};
string Utf8_to_cp1251(const char *str);

void SetColor(int text, int background)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

bool is_number(const char s[])
{
	char *e;
	return s != NULL && !isspace(*s) && (strtol(s, &e, 10), *e == '\0');
}

void WriteConsoleLine(std::string text) 
{
	text = Utf8_to_cp1251(text.data());
	SetColor(7,0);
	// MY SUPER COLOR PARSING!!!
	bool write_color = false;
	std::string color = "";
	for (int i = 0; i < text.size(); i++) {
		if (write_color) {
			if (text[i] == '}')
			{
				if (is_number(color.c_str())) {
					if (color == "")
						SetColor(7, 0);
					else
						SetColor(std::stoi(color), 0);
				} else std::cout << "{" << color << "}";
				write_color = false;
			}
			else
				color += text[i];
		}
		else {
			if (text[i] == '{')
			{
				color = "";
				write_color = true;
			}
			else
				std::cout << text[i];
		}
	}
}

void SetWindowTitle(std::string text) {
	system(("title "+ Utf8_to_cp1251(text.c_str())).c_str());

}

void instructionLoop() {
	std::string buf;
	std::getline(std::cin, buf);
	api_input.Console_Log(api_input.Console_ExeCommand(buf), "CMD");
	instructionLoop();
}

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