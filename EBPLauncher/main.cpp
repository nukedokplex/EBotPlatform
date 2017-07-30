#include <windows.h>
#include <string>
#include <iostream>
#include <cstdlib> // для system
#include "../EBPCore/ebp_api.h"

#define BOT_PATH	"bot"	// default dir to start from

typedef inputApi(*pfnInit)(const std::string botname, outputApi api);
typedef void(*pfnShutdown)(void);

pfnInit Host_Main;
pfnShutdown Host_Shutdown;
HINSTANCE	hEngine;

void Sys_LoadEBP(void)
{
	hEngine = LoadLibrary("EBPCore.dll");
	Host_Main = (pfnInit)GetProcAddress(hEngine, "Host_Main");
	Host_Shutdown = (pfnShutdown)GetProcAddress(hEngine, "Host_Shutdown");
	api_output.writeline("{8}EBPCore.dll has been loaded\n");
}

void Sys_UnloadEngine(void)
{
	if (Host_Shutdown) Host_Shutdown();
	if (hEngine) FreeLibrary(hEngine);
}
void instructionLoop();
int main()
{
	std::locale::global(std::locale(""));

	Sys_LoadEBP();
	api_input = Host_Main(BOT_PATH, api_output);
	api_input.Console_Log("Starting instructionLoop()...", "Launcher");
	instructionLoop();
	return 0;
}