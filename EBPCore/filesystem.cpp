#include "common.h"
#include "console.h"
#include "cvar.h"
#include <map>
#include <vector>
#include "filesystem.h"

void FS_Init(std::string botdir)
{
	Console_Log("Initialization Filesystem...", "Core:FS_Init");
	Cvar_AddCvar("bot_dir", botdir, "Bot directory");
}

std::string FS_GetFullPath(std::string path, bool inBotFolder)
{
	return "";
}

std::string FS_GetRootPath()
{
	return Cvar_GetValue("bot_dir") + "/";
}

bool FS_Exists(std::string path, bool inBotFolder)
{
	std::ifstream fin;
	if (inBotFolder)
		fin.open(Cvar_GetValue("bot_dir") + "/" + path);
	else
		fin.open(path);
	if (fin.is_open()) {
		fin.close();
		return true;
	} else
	return false;
}

std::fstream FS_OpenFile(std::string path, bool inBotFolder)
{
	if (!FS_Exists(path, inBotFolder)) {
		Console_Log("Error: " + path + " not found", "Core:FS_OpenFile");
		return std::fstream();
	}
	std::fstream fin;
	if (inBotFolder)
		fin.open(Cvar_GetValue("bot_dir") + "/" + path);
	else
		fin.open(path);
	return fin;
}