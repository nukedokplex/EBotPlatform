#ifdef __linux__
	#include <iostream>
	#include <cstdlib>
	#include "ebp_api.h"
	#include <string>
	#include <clocale>
	#include <stdio.h>
	#include <locale>
	#include "utf8/utf8.h"
	#include <vector>
	#include "sleep.h"
#elif _WIN32
	#include <iostream>
	#include <cstdlib>
	#include "../EBPCore/ebp_api.h"
	#include <string>
	#include <clocale>
	#include <conio.h>
	#include <stdio.h>
	#include <locale>
	#include <windows.h> 
	#include "../EBPCore/utf8\utf8.h"
	#include <vector>
#endif

using namespace std;

extern void WriteConsoleLine(std::string text);
void SetWindowTitle(std::string text);
void SafeWriteConsoleLine(std::string text);

inputApi api_input;
outputApi api_output = {
	SafeWriteConsoleLine,
	SetWindowTitle
};
string Utf8_to_cp1251(const char *str);

vector<string> linesStack; // Fix console conflicts

bool addlock = false;
void SafeWriteConsoleLine(std::string text)
{
	if (!addlock) {
		addlock = true;
		WriteConsoleLine(text);
		addlock = false;
	}
	else
	{
		Sleep(5);
		SafeWriteConsoleLine(text);
	}
}

void SetColor(int text, int background)
{
	#ifdef _WIN32
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
	#endif
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

static void cp1251_to_utf8(char *out, const char *in) {
    static const int table[128] = {                    
        0x82D0,0x83D0,0x9A80E2,0x93D1,0x9E80E2,0xA680E2,0xA080E2,0xA180E2,
        0xAC82E2,0xB080E2,0x89D0,0xB980E2,0x8AD0,0x8CD0,0x8BD0,0x8FD0,    
        0x92D1,0x9880E2,0x9980E2,0x9C80E2,0x9D80E2,0xA280E2,0x9380E2,0x9480E2,
        0,0xA284E2,0x99D1,0xBA80E2,0x9AD1,0x9CD1,0x9BD1,0x9FD1,               
        0xA0C2,0x8ED0,0x9ED1,0x88D0,0xA4C2,0x90D2,0xA6C2,0xA7C2,              
        0x81D0,0xA9C2,0x84D0,0xABC2,0xACC2,0xADC2,0xAEC2,0x87D0,              
        0xB0C2,0xB1C2,0x86D0,0x96D1,0x91D2,0xB5C2,0xB6C2,0xB7C2,              
        0x91D1,0x9684E2,0x94D1,0xBBC2,0x98D1,0x85D0,0x95D1,0x97D1,            
        0x90D0,0x91D0,0x92D0,0x93D0,0x94D0,0x95D0,0x96D0,0x97D0,
        0x98D0,0x99D0,0x9AD0,0x9BD0,0x9CD0,0x9DD0,0x9ED0,0x9FD0,
        0xA0D0,0xA1D0,0xA2D0,0xA3D0,0xA4D0,0xA5D0,0xA6D0,0xA7D0,
        0xA8D0,0xA9D0,0xAAD0,0xABD0,0xACD0,0xADD0,0xAED0,0xAFD0,
        0xB0D0,0xB1D0,0xB2D0,0xB3D0,0xB4D0,0xB5D0,0xB6D0,0xB7D0,
        0xB8D0,0xB9D0,0xBAD0,0xBBD0,0xBCD0,0xBDD0,0xBED0,0xBFD0,
        0x80D1,0x81D1,0x82D1,0x83D1,0x84D1,0x85D1,0x86D1,0x87D1,
        0x88D1,0x89D1,0x8AD1,0x8BD1,0x8CD1,0x8DD1,0x8ED1,0x8FD1
    };
    while (*in)
        if (*in & 0x80) {
            int v = table[(int)(0x7f & *in++)];
            if (!v)
                continue;
            *out++ = (char)v;
            *out++ = (char)(v >> 8);
            if (v >>= 16)
                *out++ = (char)v;
        }
        else
            *out++ = *in++;
    *out = 0;
}

string Utf8_to_cp1251(const char *str)
{
	string res;
	cp1251_to_utf8((char*) &res, str);
	return res;
}