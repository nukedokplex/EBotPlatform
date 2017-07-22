#include "common.h"
#include "ebp_api.h"
#include "console.h"
#include "cmd.h"
#include "cvar.h"
#include "filesystem.h"
#include "inetwork.h"
#include "vkwork.h"
#include "userlogic.h"


inputApi api_input = {
	Console_Log,
	Cmd_ExeCommand
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
	Console_Log("Initialization EBotPlatform "+GetVersionName(), "Core:Host_Main");
	FS_Init(botname);
	Cmd_Init();
	Net_Init();
	VK_Init();
	UL_Init();

	Cmd_ExeConfig("config.cfg");

	UL_Start();
	VKW_Start();


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