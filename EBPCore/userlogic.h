#pragma once
#include "common.h"
#include "events.h"
#include <LuaBridge.h>

namespace userlogic
{
	// Variables
	extern luabridge::lua_State* LuaScript;
	extern bool isWorking;
	extern bool isCrash;
	extern int workedThreads;
	extern int loadedModules;
	
	// Voids
	void init();
	void start();
	void waitForWork();
	void callEvent(std::string method, events::caller *ev);
	void free();
	void logError(luabridge::LuaException error);
	std::string c_relua(std::vector<std::string> cmd_args);
	// Lua API
	namespace api
	{
		void registerApi();
		// Api Funcs
		luabridge::LuaRef connect(std::string text);
		void connectModule(std::string text);

		luabridge::LuaRef argsToTable(args c_args);
		args tableToArgs(luabridge::LuaRef table);

		luabridge::LuaRef cmd_ParseArgs(std::string text);
		std::string net_Get(std::string url);
		string cmd_data(luabridge::LuaRef cmd_args, int sub);
	}
}