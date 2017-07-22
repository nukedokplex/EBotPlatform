#include <iostream>
#include <cstdlib> // для system
#include "../EBPCore/ebp_api.h"
#include <string>

extern void WriteConsoleLine(std::string text);
void SetWindowTitle(std::string text);

inputApi api_input;
outputApi api_output = {
	WriteConsoleLine,
	SetWindowTitle
};

void WriteConsoleLine(std::string text) {
	std::cout << text.data() << std::endl;
}

void SetWindowTitle(std::string text) {
	system(("title "+ text).c_str());

}

void instructionLoop() {
	std::string buf;
	std::getline(std::cin, buf);
	api_input.Console_Log(api_input.Console_ExeCommand(buf), "CMD");
	instructionLoop();
}