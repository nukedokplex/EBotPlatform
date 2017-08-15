#include <iostream>
#include <cstring>
#include <string>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <cstring>
#include "../EBPCore/ebp_api.h"

bool input;

#ifdef __linux__ 
	#include <dlfcn.h> 
	#include <errno.h> 
	#define CORELIB "libEBPCore.so" 
	#define dlmount(x) dlopen(x, RTLD_NOW) 
	#define HINSTANCE void* 
#elif _WIN32 
	#include <windows.h>
	#define dlmount(x) LoadLibraryA(x) 
	#define dlclose(x) FreeLibrary(x) 
	#define dlsym(x,y) GetProcAddress(x,y) 	
	#define CORELIB "EBPCore.dll" 
#endif

	typedef inputApi(*pfnInit)(const std::string botname, outputApi api);
	typedef void(*pfnShutdown)(void);

	pfnInit Host_Main;
	pfnShutdown Host_Shutdown;
	HINSTANCE	hEngine;

#define BOT_PATH	"bot"	// default dir to start from

void Sys_LoadEBP(void)
{
	hEngine = dlmount(CORELIB);
	Host_Main = (pfnInit)dlsym(hEngine, "Host_Main");
	Host_Shutdown = (pfnShutdown)dlsym(hEngine, "Host_Shutdown");
	api_output.writeline((std::string)"{8}"+(std::string)CORELIB+(std::string)" has been loaded\n");
}

void Sys_UnloadEngine(void)
{
	if (Host_Shutdown) Host_Shutdown();
	if (hEngine) dlclose(hEngine);
}

void instructionLoop();
int main(int argc, char* argv[])
{
	if(argc > 1)input = false;
	else input = true;
	std::locale::global(std::locale(""));
	Sys_LoadEBP();
	api_input = Host_Main(BOT_PATH, api_output);
	api_input.Console_Log("Starting instructionLoop()...", "Launcher");
	std::thread iloop(instructionLoop);
	iloop.detach();
	return api_input.Host_Start();
}