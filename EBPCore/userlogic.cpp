#include "userlogic.h"
#include "common.h"
#include "console.h"
#include "cmd.h"
#include "inetwork.h"
#include "filesystem.h"
#include "cvar.h"
#include "vkwork.h"
#include "../common/b_api.h"
#include "events.h"
#include <string>
#include <locale>
#include <cstdlib> // для system
#include <windows.h>
#include <iostream>
#include <LuaBridge.h>

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

using namespace luabridge;


std::string ReUL_Command(std::vector<std::string> cmd_args);
// DLL
typedef void(*pfnInit)(core_api api);
lua_State* LuaScript;

void UL_Init()
{
	Console_Log("Initialization UserLogic...", "Core:UL_Init");
	Cmd_AddCommand("relua", ReUL_Command, "Reload UserLogic");
	Cvar_AddCvar("dll_path", "scripts/main.lua", "Path to DLL");
}

/*
Команда перезагрузки UserLogic
*/
void UL_RegisterAPI(lua_State* LuaScript);
void UL_LogError(luabridge::LuaException error);
void UL_Start() {
	try {
		Console_Log("Start UL in \"" + Cvar_GetValue("dll_path") + "\"", "Core:UL_Start");
		// Load
		LuaScript = luabridge::luaL_newstate();
		luaL_openlibs(LuaScript);
		UL_RegisterAPI(LuaScript);
		luaL_dofile(LuaScript, ("bot/" + Cvar_GetValue("dll_path")).c_str());
		lua_pcall(LuaScript, 0, 0, 0);
		UL_Call("Main");
	}
	catch (luabridge::LuaException const& e) {
		UL_LogError(e);
	}
}

void UL_Call(std::string method)
{
	try {
		luabridge::getGlobal(LuaScript, method.c_str())();
	}
	catch (luabridge::LuaException const& e) {
		UL_LogError(e);
	}
}

void UL_Free() {
}

void UL_LogError(luabridge::LuaException error) {
	Console_Error(error.what(), "Lua");
}

std::string ReUL_Command(std::vector<std::string> cmd_args)
{
	UL_Free();
	UL_Start();
	return "UserLogic reloaded";
}