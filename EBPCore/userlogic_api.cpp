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
#include <unordered_map>
// --
#include <LuaBridge.h>

#ifdef __linux__ 
	extern "C" {
	# include "lua5.3/lua.h"
	# include "lua5.3/lauxlib.h"
	# include "lua5.3/lualib.h"
	}
#elif _WIN32
	extern "C" {
	# include "lua.h"
	# include "lauxlib.h"
	# include "lualib.h"
	}
#endif

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
		.addFunction("data", userlogic::api::cmd_data)
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
		.beginClass <events::caller>("event")
		.addStaticFunction("register", events::registerEvent)
		.addFunction("getString", &events::caller::getString)
		.addFunction("getInt", &events::caller::getInt)
		.endClass();
	// Filesystem
	luabridge::getGlobalNamespace(LuaScript)
	.beginNamespace("filesystem")
		.addFunction("exists", fs::exists)
		.addFunction("root", fs::getRootPath)
	.endNamespace();
	// Network
	luabridge::getGlobalNamespace(LuaScript)
	.beginNamespace("net")
		.addFunction("get", net::get)
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
		.addFunction("send", userlogic::api::simplevksend)
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
		.addFunction("connectModule", userlogic::api::connectModule)
		.addFunction("print", userlogic::api::print);

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
	userlogic::loadedModules++;
}

// UTF8 print
void userlogic::api::print(std::string text)
{
	api_output.writeline(text+"\n");
}

luabridge::LuaRef userlogic::api::argsToTable(args c_args) 
{
	luabridge::LuaRef r = luabridge::newTable(LuaScript);
	for (int i = 0;i < c_args.size();i++)
		r[i + 1] = (std::string) c_args[i];
	return r;
}

args userlogic::api::tableToArgs(luabridge::LuaRef table)
{
	args r;
	for (int i = 0;i < table.length();i++)
		r.push_back(table[i+1].cast<string>());
	return r;
}

std::unordered_map<std::string, luabridge::LuaRef> getKeyValueMap(const luabridge::LuaRef& table)
{
	using namespace luabridge;
	std::unordered_map<std::string, LuaRef> result;
	if (table.isNil()) { return result; }

	auto L = table.state();
	push(L, table); // push table

	lua_pushnil(L);  // push nil, so lua_next removes it from stack and puts (k, v) on stack
	while (lua_next(L, -2) != 0) { // -2, because we have table at -1
		if (lua_isstring(L, -2)) { // only store stuff with string keys
			result.emplace(lua_tostring(L, -2), LuaRef::fromStack(L, -1));
		}
		lua_pop(L, 1); // remove value, keep key for lua_next
	}

	lua_pop(L, 1); // pop table
	return result;
}

map<string, string> userlogic::api::tableToMap(luabridge::LuaRef table)
{
	map<string, string> r;
	for (auto& pair : getKeyValueMap(table)) {
		auto& key = pair.first;
		auto& value = pair.second;
		r[key] = value.cast<string>();
	}
	return r;
}

luabridge::LuaRef userlogic::api::cmd_ParseArgs(std::string text) 
{
	return userlogic::api::argsToTable(cmd::parse(text));
}

// Lua style
string userlogic::api::cmd_data(luabridge::LuaRef cmd_args, int sub)
{
	return cmd::data(userlogic::api::tableToArgs(cmd_args), sub);
}

string userlogic::api::simplevksend(string method, luabridge::LuaRef params, bool sendtoken)
{
	return vk::send(method, userlogic::api::tableToMap(params), sendtoken);
}