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
void UL_Free();

lua_State* LuaScript;
bool LuaRun;

void UL_Init()
{
	Console_Log("Initialization UserLogic...", "Core:UL_Init");
	Cmd_AddCommand("relua", ReUL_Command, "Reload UserLogic");
	Cvar_AddCvar("dll_path", "scripts/main.lua", "Path to DLL");
}

/*
Команда перезагрузки UserLogic
*/
void UL_RegisterAPI();
void UL_LogError(luabridge::LuaException error);
void UL_Start() {
	try {
		Console_Log("Start UL in \"" + Cvar_GetValue("dll_path") + "\"", "Core:UL_Start");
		// Load
		LuaScript = luabridge::luaL_newstate();
		luaL_openlibs(LuaScript);
		UL_RegisterAPI();
		lua_checkstack(LuaScript, 2048);
		luaL_dofile(LuaScript, ("bot/" + Cvar_GetValue("dll_path")).c_str());
		lua_pcall(LuaScript, 0, 0, 0);
		LuaRun = true;
		UL_Call("Main");
	}
	catch (luabridge::LuaException const& e) {
		UL_LogError(e);
	}
}

void UL_Call(std::string method)
{
	while (!LuaRun)
	{
		Sleep(100);
	}
	try {
		luabridge::LuaRef func = luabridge::getGlobal(LuaScript, method.c_str());
		if (func.isFunction())
			func();
	}
	catch (luabridge::LuaException const& e) {
		UL_LogError(e);
	}
}

void UL_CallEvent(std::string method, int sid)
{
	while (!LuaRun)
	{
		Sleep(100);
	}
	lua_State* state = lua_newthread(LuaScript);
	try {
		luabridge::LuaRef func = luabridge::getGlobal(state, method.c_str());
		if (func.isFunction())
			func(sid);
	}
	catch (luabridge::LuaException const& e) {
		UL_LogError(e);
	}
	lua_gc(LuaScript, LUA_GCCOLLECT, 0);
}

void UL_Free() {
	LuaRun = false;
	lua_close(LuaScript);
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