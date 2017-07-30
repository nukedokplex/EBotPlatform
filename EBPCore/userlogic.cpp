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

lua_State* LuaScript;
bool LuaRun;

void userlogic::init()
{
	cmd::add("relua", userlogic::c_relua, "Reload UserLogic");
	cvar::add("dll_path", "scripts/main.lua", "Path to DLL");
}

void userlogic::start() 
{
	try {
		console::log("Starting at \"" + cvar::get("dll_path") + "\"", "UserLogic");
		// Load
		LuaScript = luabridge::luaL_newstate();
		luaL_openlibs(LuaScript);
		userlogic::api::registerApi();
		lua_checkstack(LuaScript, 2048);
		luaL_dofile(LuaScript, ("bot/" + cvar::get("dll_path")).c_str());
		lua_pcall(LuaScript, 0, 0, 0);
		LuaRun = true;
		userlogic::call("Main");
	}
	catch (luabridge::LuaException const& e) {
		userlogic::logError(e);
	}
}

void userlogic::call(std::string method)
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
		userlogic::logError(e);
	}
}

void userlogic::callEvent(std::string method, int sid)
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
		userlogic::logError(e);
	}
}

void userlogic::free() {
	LuaRun = false;
	lua_close(LuaScript);
}

void userlogic::logError(luabridge::LuaException error) {
	console::error(error.what(), "Lua");
}

std::string userlogic::c_relua(std::vector<std::string> cmd_args)
{
	userlogic::free();
	userlogic::start();
	return "UserLogic reloaded";
}