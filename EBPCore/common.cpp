#include "common.h"
#include "ebp_api.h"
#include "console.h"
#include "cmd.h"
#include "cvar.h"

/*
	Установить имя окна
*/
std::string Title_Command(std::vector<std::string> cmd_args)
{
	if (cmd_args.size() != 2) {
		return "Use \"title <window name>\"";
	}
	api_output.setwindowtitle(cmd_args[1]);
	return "New title is \"" + cmd_args[1] + "\"";
}

/*
	Пасхалка
*/
std::string Loh_Command(std::vector<std::string> cmd_args)
{
	for (int i = 0; i<100; i++)
		Console_Log("SAM LOH!", "AZAZA");
	return "Loh";
}

std::string GetVersionName() {
	return "A_" + std::to_string((int)(VERSION * 10.0f));
}