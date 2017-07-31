#pragma once
#include "common.h"
#include <fstream>

namespace fs
{
	void init(std::string botdir);
	bool exists(std::string path, bool inBotFolder = true);
	std::fstream openFile(std::string path, bool inBotFolder = true);
	std::string getRootPath();
	void writeData(string path, string data, bool inBotFolder = true);
	void safeWriteData(string path, string data, bool inBotFolder = true);
}