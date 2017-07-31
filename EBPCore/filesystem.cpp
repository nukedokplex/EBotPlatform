#include "common.h"
#include "console.h"
#include "cvar.h"
#include <map>
#include <vector>
#include "filesystem.h"

void fs::init(std::string botdir)
{
	cvar::add("bot_dir", botdir, "Bot directory");
}

std::string fs::getRootPath()
{
	return cvar::get("bot_dir") + "/";
}

bool fs::exists(std::string path, bool inBotFolder)
{
	std::ifstream fin;
	if (inBotFolder)
		fin.open(cvar::get("bot_dir") + "/" + path);
	else
		fin.open(path);
	if (fin.is_open()) {
		fin.close();
		return true;
	} else
	return false;
}

std::fstream fs::openFile(std::string path, bool inBotFolder)
{
	if (!fs::exists(path, inBotFolder)) {
		console::error(path + " not found", "Filesystem");
		return std::fstream();
	}
	std::fstream fin;
	if (inBotFolder)
		fin.open(cvar::get("bot_dir") + "/" + path);
	else
		fin.open(path);
	return fin;
}

void fs::writeData(string path, string data, bool inBotFolder)
{
	if (inBotFolder)
		path = cvar::get("bot_dir") + "/" + path;
	if (FILE *file = fopen(path.c_str(), "a")) {
		fprintf(file, data.c_str());
		fclose(file);
	}
}

void fs::safeWriteData(string path, string data, bool inBotFolder)
{
	// TODO
}