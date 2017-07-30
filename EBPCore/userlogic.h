#pragma once
#include <string>
#include <vector>
#include <LuaBridge.h>

namespace userlogic
{
	void init();
	void start();
	void call(std::string method);
	void callEvent(std::string method, int evnum);
	void free();
	void logError(luabridge::LuaException error);
	std::string c_relua(std::vector<std::string> cmd_args);
	namespace api
	{
		void registerApi();
		// Api Funcs
		luabridge::LuaRef connect(std::string text);
		void connectModule(std::string text);

		luabridge::LuaRef cmd_ParseArgs(std::string text);
		std::string net_Get(std::string url);
	}
}