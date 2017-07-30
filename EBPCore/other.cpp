#include "common.h"
#include <time.h> // time
#include "ebp_api.h"
#include "console.h"
#include "cmd.h"
#include "cvar.h"
#include "other.h"

/*
	Установить имя окна
*/
std::string other::c_title(std::vector<std::string> cmd_args)
{
	if (cmd_args.size() != 2) {
		return "Use \"title <window name>\"";
	}
	api_output.setwindowtitle(cmd_args[1]);
	return "New title is \"" + cmd_args[1] + "\"";
}

std::string other::getVersionName() {
	return "A_" + std::to_string((int)(VERSION * 10.0f));
}

string other::getCurrentTime()
{
	time_t rawTime;
	tm *timeInfo;

	time(&rawTime);
	timeInfo = localtime(&rawTime);

	return to_string(timeInfo->tm_mday)+"/"+ to_string(timeInfo->tm_mon+1) + " "+to_string(timeInfo->tm_hour) + ":" + to_string(timeInfo->tm_min) + ":" + to_string(timeInfo->tm_sec);
}