#ifdef __linux__
	#include <string>
	#include <iostream>
	#include <cstdlib> // для system
	#include "ebp_api.h"
	#include "host.h"
	#include <thread>
#elif _WIN32
	#include <windows.h>
	#include <string>
	#include <iostream>
	#include <cstdlib> // для system
	#include "../EBPCore/ebp_api.h"
	#include <thread>
	
	typedef inputApi(*pfnInit)(const std::string botname, outputApi api);
	typedef void(*pfnShutdown)(void);

	pfnInit Host_Main;
	pfnShutdown Host_Shutdown;
	HINSTANCE	hEngine;
#endif

#define BOT_PATH	"bot"	// default dir to start from

void Sys_LoadEBP(void)
{
	#ifdef _WIN32
	hEngine = LoadLibrary("EBPCore.dll");
	Host_Main = (pfnInit)GetProcAddress(hEngine, "Host_Main");
	Host_Shutdown = (pfnShutdown)GetProcAddress(hEngine, "Host_Shutdown");
	api_output.writeline("{8}EBPCore.dll has been loaded\n");
	#endif
}

void Sys_UnloadEngine(void)
{
	if (Host_Shutdown) Host_Shutdown();
	#ifdef _WIN32
	if (hEngine) FreeLibrary(hEngine);
	#endif
}

void instructionLoop();
int main()
{
	std::locale::global(std::locale(""));

	Sys_LoadEBP();
	api_input = Host_Main(BOT_PATH, api_output);
	api_input.Console_Log("Starting instructionLoop()...", "Launcher");
	std::thread iloop(instructionLoop);
	iloop.detach();
	return api_input.Host_Start();
}