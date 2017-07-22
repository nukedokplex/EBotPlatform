#pragma once
#include "filesystem.h"
#include "cmd.h"
#include <vector>
#include <fstream>

extern std::string FS_GetFullPath(std::string path, bool inBotFolder = true);
extern void FS_Init(std::string botdir);
extern bool FS_Exists(std::string path, bool inBotFolder = true);
extern std::fstream FS_OpenFile(std::string path, bool inBotFolder = true);
extern std::string FS_GetRootPath();