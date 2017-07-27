#include "common.h"
#include "ebp_api.h"
#include "console.h"
#include "cmd.h"
#include "cvar.h"

/*
	Установить имя окна
*/
std::string common::c_title(std::vector<std::string> cmd_args)
{
	if (cmd_args.size() != 2) {
		return "Use \"title <window name>\"";
	}
	api_output.setwindowtitle(cmd_args[1]);
	return "New title is \"" + cmd_args[1] + "\"";
}

std::string common::getVersionName() {
	return "A_" + std::to_string((int)(VERSION * 10.0f));
}