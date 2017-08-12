#include "common.h"
#include "userlogic.h"
#include "console.h" // console::log; console::error
#include "cmd.h" // cmd::add
#include "cvar.h" // cvar::add
#include "events.h" // events::caller
#include <windows.h> // Sleep
#include <LuaBridge.h>
#include "utils.h"

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

using namespace userlogic;

luabridge::lua_State* userlogic::LuaScript;
bool userlogic::isWorking;
bool userlogic::isCrash;
int userlogic::loadedModules = 0;
int userlogic::workedThreads = 0;

void userlogic::init()
{
	cmd::add("relua", c_relua, "Reload UserLogic");
	cvar::add("dll_path", "scripts/main.lua", "Path to DLL");
	cvar::add("ul_debug_calls", "0", "Debug event calls");
}

void userlogic::start() 
{
	try {
		console::log("Starting at \"" + cvar::get("dll_path") + "\"", "UserLogic");
		// Load
		LuaScript = luabridge::luaL_newstate();
		luaL_openlibs(LuaScript);
		api::registerApi();// Init api
		lua_checkstack(LuaScript, 2048);
		
		int code = luaL_dofile(LuaScript, ("bot/" + cvar::get("dll_path")).c_str());

		if (code != LUABRIDGE_LUA_OK)
			throw luabridge::LuaException(LuaScript, code);


		luabridge::LuaRef funcModules = luabridge::getGlobal(LuaScript, "LoadModules");
		if (!funcModules.isNil() && funcModules.isFunction())
			funcModules();// Loading modules
		else
			throw string("Function \"LoadModules()\" not found");
		console::log("Loaded " + to_string(loadedModules) + " modules.", "UserLogic");
		luabridge::LuaRef funcMain = luabridge::getGlobal(LuaScript, "Main");
		if (!funcMain.isNil() && funcMain.isFunction())
			funcMain();// Init script
		else
			throw string("Function \"Main()\" not found");

		userlogic::isWorking = true;
		userlogic::isCrash = false;
	}
	catch (luabridge::LuaException const& e) {
		isCrash = true;
		logError(e);
	}
	catch (string &e) {
		isCrash = true;
		console::error(e, "UserLogic");
	}
}

void userlogic::waitForWork()
{
	if (isWorking)
		return;

	while (!isWorking)
	{
		Sleep(500);
	}
}

void userlogic::callEvent(std::string method, events::caller *ev)
{
	waitForWork();
	workedThreads++;
	console::debug("Start "+method+"(event) method. LT: "+to_string(workedThreads), "CallEventLua", "ul_debug_calls");
	luabridge::lua_State *state = lua_newthread(LuaScript);
	try {
		luabridge::LuaRef func = luabridge::getGlobal(state, method.c_str());
		if (func.isFunction())
		{
			func(ev);
		}
		else
			console::error(method + " is not function", "CallEvent");
	}
	catch (luabridge::LuaException const& e) {
		logError(e);
	}
	delete ev;
	workedThreads--;
	console::debug("End " + method + "(event) method. LT: " + to_string(workedThreads), "CallEventLua", "ul_debug_calls");
}

void userlogic::free() 
{
	loadedModules = 0;
	isCrash = false;
	isWorking = false;

	lua_close(LuaScript);
}

void userlogic::logError(luabridge::LuaException error)
{
	console::error(error.what(), "Lua");
}

std::string userlogic::c_relua(std::vector<std::string> cmd_args)
{
	if (isWorking == false && isCrash ==false)
	{
		console::error("Please, wait. Your scripts will start", "relua");
		return "Please, wait. Your scripts will start";
	}
	isWorking = false;
	if (workedThreads != 0)
	{
		
		console::log("Wait threads...", "relua");
		while (workedThreads != 0)
		{
			Sleep(1000);
		}
	}
	free();
	start();
	return "UserLogic reloaded";
}