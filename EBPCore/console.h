#pragma once
#include "common.h"
#include "ebp_api.h"

namespace console
{
	extern string filename;
	void init(string botdir);
	void log(std::string text, std::string type);
	void error(std::string text, std::string type);
	void debug(string text, string type, string cvar_name);
}