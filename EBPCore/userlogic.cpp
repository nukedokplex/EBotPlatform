#include "userlogic.h"
#include "common.h"
#include "console.h"
#include "cmd.h"
#include "inetwork.h"
#include "filesystem.h"
#include "cvar.h"
#include "vkwork.h"
#include "../common/b_api.h"
#include "events.h"
#include <string>
#include <locale>
#include <cstdlib> // для system
#include <windows.h>
#include <iostream>

std::string ReUL_Command(std::vector<std::string> cmd_args);
// DLL
typedef void(*pfnInit)(core_api api);
HINSTANCE ul_dll;
core_api core = {
	// CMD
	Cmd_ExeConfig,
	Cmd_AddCommand,
	Cmd_Exists,
	Cmd_ExeCommand,
	Cmd_ParseArgs,
	// Console
	Console_Log,
	Console_Error,
	// Cvar
	Cvar_Exists,
	Cvar_SetValue,
	Cvar_AddCvar,
	Cvar_GetValue,
	// Events
	Event_Register,
	Event_Call,
	// Filesystem
	FS_Exists,
	FS_GetRootPath,
	FS_GetFullPath,
	FS_OpenFile,
	// Network
	Net_Get,
	Net_Post,
	// VKWORK
	VK_CreateRequest,
	VK_SetParam,
	VK_Send,
	VK_SendOff,
	VK_Send
};

void UL_Init()
{
	Console_Log("Initialization UserLogic...", "Core:UL_Init");
	Cmd_AddCommand("re", ReUL_Command, "Reload UserLogic");
	Cvar_AddCvar("dll_path", "bin/bin.dll", "Path to DLL");
}

/*
Команда перезагрузки UserLogic
*/
void UL_Start() {
	Console_Log("Start UL in \""+ Cvar_GetValue("dll_path") +"\"", "Core:UL_Start");
	ul_dll = LoadLibrary(("bot/"+Cvar_GetValue("dll_path")).c_str());
	if (ul_dll == NULL) {
		Console_Error(Cvar_GetValue("dll_path") + " NOT LOADED", "Core:UL_Start");
		return;
	}
	pfnInit Host_Main = (pfnInit)GetProcAddress(ul_dll, "Host_Main");
	if (Host_Main == NULL) {
		Console_Error(Cvar_GetValue("dll_path") + " IS BAD", "Core:UL_Start");
		return;
	}
//	Host_Main(core);
}

void UL_Free() {
}

//void UL_LogError(_EXCEPTION_ error) {

//}

std::string ReUL_Command(std::vector<std::string> cmd_args)
{
	UL_Free();
	UL_Start();
	return "UserLogic reloaded";
}