#pragma once
#include "filesystem.h"
#include "cmd.h"
#include <vector>
#include <fstream>

namespace fs
{
	void init(std::string botdir);
	bool exists(std::string path, bool inBotFolder = true);
	std::fstream openFile(std::string path, bool inBotFolder = true);
	std::string getRootPath();
}