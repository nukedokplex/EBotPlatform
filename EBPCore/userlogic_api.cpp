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
#include "other.h"
// --
#include <LuaBridge.h>

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

extern luabridge::lua_State* LuaScript;


void userlogic::api::registerApi()
{
	// CMD
	luabridge::getGlobalNamespace(LuaScript)
	.beginNamespace("cmd")
		.addFunction("exeConfig", cmd::exec)
		//.addFunction("add", cmd::add); -- TODO add Lua functions
		.addFunction("exists", cmd::exists)
		.addFunction("exe", cmd::exe)
		.addFunction("parse", userlogic::api::cmd_ParseArgs)
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
	luabridge::getGlobalNamespace(LuaScript)
	.beginNamespace("filesystem")
		.addFunction("exists", fs::exists)
		.addFunction("root", fs::getRootPath)
	.endNamespace();
	// Network
	luabridge::getGlobalNamespace(LuaScript)
	.beginNamespace("net")
		.addFunction("get", userlogic::api::net_Get)
		.beginClass <net::request>("request")
			.addConstructor <void(*) (string)>()
			.addFunction("set", &net::request::set)
			.addFunction("setFile", &net::request::setFile)
			.addFunction("setData", &net::request::setData)
			.addFunction("send", &net::request::send)
		.endClass()
	.endNamespace();
	// VKWORK
	luabridge::getGlobalNamespace(LuaScript)
		.beginNamespace("vk")
		.addFunction("getToken", vk::getToken)
		.beginClass <vk::request>("request")
			.addConstructor <void(*) (string, bool)>()
			.addConstructor <void(*) (string)>()
			.addData("method", &vk::request::method)
			.addData("sendtoken", &vk::request::sendtoken)
			.addFunction("set", &vk::request::set)
			.addFunction("send", &vk::request::send)
		.endClass()
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
	// String UTF8
	luabridge::getGlobalNamespace(LuaScript)
		.beginNamespace("string")
		.addFunction("lower", utils::string::lower)
		.addFunction("upper", utils::string::upper)
	.endNamespace();
	// Random
	luabridge::getGlobalNamespace(LuaScript)
		.beginNamespace("random")
		.addFunction("get", utils::random::get)
		.endNamespace();
	// Other
	luabridge::getGlobalNamespace(LuaScript)
		.addFunction("changeCharset", Charset_Change)
		.addFunction("getVersionName", other::getVersionName)
		.addFunction("connect", userlogic::api::connect)
		.addFunction("connectModule", userlogic::api::connectModule);

	// Connect default modules
	luabridge::setGlobal(LuaScript, luabridge::getGlobal(LuaScript, "require")("data/lua/json"), "JSON");
	luabridge::getGlobal(LuaScript, "require")("data/lua/LuaTools");
	luabridge::setGlobal(LuaScript, luabridge::getGlobal(LuaScript, "require")("data/lua/ErrorSystem"), "ErrorSystem");
	luabridge::setGlobal(LuaScript, luabridge::getGlobal(LuaScript, "require")("data/lua/EbpTools"), "EbpTools");
}

// API \\
// Connect script
luabridge::LuaRef userlogic::api::connect(std::string text) 
{
	return luabridge::getGlobal(LuaScript, "require")(fs::getRootPath()+text);
}

// Connect module
void userlogic::api::connectModule(std::string text)
{
	luabridge::LuaRef r = userlogic::api::connect("scripts/modules/"+text);
	luabridge::setGlobal(LuaScript, r, text.c_str());
}

luabridge::LuaRef userlogic::api::cmd_ParseArgs(std::string text) {
	std::vector<std::string> or = cmd::parse(text);
	luabridge::LuaRef r = luabridge::newTable(LuaScript);
	for (int i = 0;i < or.size();i++)
		r[i+1] = (std::string)or[i];
	return r;
}

std::string userlogic::api::net_Get(std::string url)
{
	return (std::string)(char*)net::get(url);
}