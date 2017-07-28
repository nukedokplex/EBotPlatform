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
#include "utils.h"
// --
#include <LuaBridge.h>

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

extern luabridge::lua_State* LuaScript;

luabridge::LuaRef API_Cmd_ParseArgs(std::string text);
std::string API_Net_Get(std::string url);

void UL_RegisterAPI()
{
	// CMD
	luabridge::getGlobalNamespace(LuaScript)
	.beginNamespace("cmd")
		.addFunction("exeConfig", cmd::exec)
		//.addFunction("add", cmd::add); -- TODO add Lua functions
		.addFunction("exists", cmd::exists)
		.addFunction("exe", cmd::exe)
		.addFunction("parse", API_Cmd_ParseArgs)
	.endNamespace();

	// Consoló
	luabridge::getGlobalNamespace(LuaScript)
	.beginNamespace("console")
		.addFunction("log", console::log)
		.addFunction("error", console::error)
	.endNamespace();
	
	// Cvar
	luabridge::getGlobalNamespace(LuaScript)
	.beginNamespace("cvar")
		.addFunction("exists", cvar::exists)
		.addFunction("set", cvar::set)
		.addFunction("add", cvar::add)
		.addFunction("get", cvar::get)
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
	.beginNamespace("net")
		.addFunction("get", API_Net_Get)
		.addFunction("create", Net_CreatePost)
		.addFunction("set", Net_SetParam)
		.addFunction("send", Net_Send)
	.endNamespace();
	// VKWORK
	luabridge::getGlobalNamespace(LuaScript)
		.beginNamespace("vk")
		.addFunction("create", VK_CreateRequest)
		.addFunction("set", VK_SetParam)
		.addFunction("send", (std::string(*)(int))VK_Send)
		.addFunction("send_off", VK_SendOff)
		.addFunction("getToken", VK_GetToken)
	.endNamespace();
	// Flags
	luabridge::getGlobalNamespace(LuaScript)
		.beginNamespace("flag")
		.addFunction("is", Flags_IsFlag)
		.addFunction("add", Flags_AddFlag)
		.addFunction("remove", Flags_RemoveFlag)
	.endNamespace();
	// Regular
	luabridge::getGlobalNamespace(LuaScript)
		.beginNamespace("reg")
		.addFunction("replace", utils::regular::replace)
	.endNamespace();
	// Other
	luabridge::getGlobalNamespace(LuaScript)
		.addFunction("changeCharset", Charset_Change);
	// String UTF8
	luabridge::getGlobalNamespace(LuaScript)
		.beginNamespace("string")
		.addFunction("lower", utils::string::lower)
		.addFunction("upper", utils::string::upper)
		.endNamespace();
}

// API \\

// Console
luabridge::LuaRef API_Cmd_ParseArgs(std::string text) {
	std::vector<std::string> or = cmd::parse(text);
	luabridge::LuaRef r = luabridge::newTable(LuaScript);
	for (int i = 0;i < or.size();i++)
		r[i+1] = (std::string)or[i];
	return r;
}

std::string API_Net_Get(std::string url)
{
	return (std::string)(char*)Net_Get(url);
}