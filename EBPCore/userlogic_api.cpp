// API
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
// --
#include <LuaBridge.h>

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}


void UL_RegisterAPI(luabridge::lua_State* LuaScript)
{
	// CMD
	luabridge::getGlobalNamespace(LuaScript)
	.beginNamespace("cmd")
		.addFunction("exeConfig", Cmd_ExeConfig)
		//.addFunction("add", Cmd_AddCommand); -- TODO add Lua functions
		.addFunction("exists", Cmd_Exists)
		.addFunction("exec", Cmd_ExeCommand)
		.addFunction("parse", Cmd_ParseArgs)
	.endNamespace();

	// Consoló
	luabridge::getGlobalNamespace(LuaScript)
	.beginNamespace("console")
		.addFunction("log", Console_Log)
		.addFunction("error", Console_Error)
	.endNamespace();
	
	// Cvar
	luabridge::getGlobalNamespace(LuaScript)
	.beginNamespace("cvar")
		.addFunction("exists", Cvar_Exists)
		.addFunction("set", Cvar_SetValue)
		.addFunction("add", Cvar_AddCvar)
		.addFunction("get", Cvar_GetValue)
	.endNamespace();
	// Events
	luabridge::getGlobalNamespace(LuaScript)
	.beginNamespace("event")
		.addFunction("pushString", Event_PushString)
		.addFunction("pushInt", Event_PushString)
		.addFunction("pushFloat", Event_PushString)
		.addFunction("getString", Event_GetString)
		.addFunction("getInt", Event_GetInt)
		.addFunction("getFloat", Event_GetFloat)
		.addFunction("flush", Event_Flush)
		.addFunction("register", Event_Register)
		.addFunction("call", Event_Call)
	.endNamespace();
	// Filesystem
	/*n.
		beginNamespace("filesystem")
		.addFunction("exists", FS_Exists)
		.addFunction("root", FS_GetRootPath)
		.addFunction("full", FS_GetFullPath)
		.addFunction("open", FS_OpenFile)
		.endNamespace();*/
	// Network
	luabridge::getGlobalNamespace(LuaScript)
	.beginNamespace("network")
		.addFunction("get", Net_Get)
		.addFunction("post", Net_Post)
	.endNamespace();
	// VKWORK
	luabridge::getGlobalNamespace(LuaScript)
	.beginNamespace("vk")
		.addFunction("create", VK_CreateRequest)
		.addFunction("set", VK_SetParam)
		.addFunction("send", (std::string(*)(int))VK_Send)
		.addFunction("send_off", VK_SendOff)
	.endNamespace();
}

// API \\

// Console
void UL_API_Console_Log(std::string& text, std::string& type) {
	Console_Log(text, type);
}