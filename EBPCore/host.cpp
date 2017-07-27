#include "common.h"
#include "ebp_api.h"
#include "console.h"
#include "cmd.h"
#include "cvar.h"
#include "filesystem.h"
#include "inetwork.h"
#include "vkwork.h"
#include "userlogic.h"
#include "longpoll.h"
#include "mysql.h"

inputApi api_input = {
	console::log,
	cmd::exe
};
outputApi api_output;




/*
Function: Host_Main
Desc: Init EBPCore
*/
inputApi EXPORT Host_Main(const std::string botname, outputApi api)
{
	setlocale(LC_CTYPE, "Russian");
	system("chcp 65001");
	 // вызов функции настройки локали

	api_output = api;
	console::log("Initialization EBotPlatform "+common::getVersionName(), "Core:Host_Main");
	FS_Init(botname);
	cmd::init();
	Net_Init();
	VK_Init();
	UL_Init();
	mysql::utils::init();

	cmd::exec("config.cfg");

	mysql::utils::start();
	UL_Start();
	longpoll::start();


	atexit(Host_Shutdown);
	return api_input;
}

/*
=================
Host_Shutdown
=================
*/
void EXPORT Host_Shutdown(void)
{
}