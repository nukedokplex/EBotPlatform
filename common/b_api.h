#pragma once
#include <string>
#include <vector>
#include <map>

// CMD
typedef std::string(*xcommand_t)(std::vector<std::string> cmd_args);
// VKWORK
struct VKRequest {
	std::string method;
	bool sendtoken;
	std::map<std::string, std::string> params;
};

struct core_api
{
	//CMD
	void(*Cmd_ExeConfig) (std::string cpath);
	void(*Cmd_AddCommand)(const std::string cmd_name, xcommand_t function, const std::string cmd_desc);
	bool(*Cmd_Exists)    (const std::string cmd_name);
	std::string(*Cmd_ExeCommand)            (std::string text);
	std::vector<std::string>(*Cmd_ParseArgs)(std::string line);
	// Console
	void(*Console_Log)  (std::string text, std::string type);
	void(*Console_Error)(std::string text, std::string type);
	// Cvar
	bool(*Cvar_Exists)  (const std::string cvar_name);
	bool(*Cvar_SetValue)(const std::string cvar_name, std::string cvar_value);
	void(*Cvar_AddCvar) (const std::string cvar_name, std::string value, const std::string cvar_desc);
	std::string(*Cvar_GetValue)(const std::string cvar_name);
	// Events
	void(*Event_Register)(std::string name, void(*)(va_list));
	void(*Event_Call)(std::string name, ...);
	// Filesystem
	bool(*FS_Exists)(std::string path, bool inBotFolder);
	std::string(*FS_GetRootPath)();
	std::string(*FS_GetFullPath)(std::string path, bool inBotFolder);
	std::fstream(*FS_OpenFile)(std::string path, bool inBotFolder);
	// Network
	void*(*Net_Get)(std::string url);
	void*(*Net_Post)(std::string url, std::map<std::string, std::string> params);
	// VKWORK
	int(*VK_CreateRequest)(std::string method, bool sendtoken);
	void(*VK_SetParam)(int vkridr, std::string p_name, std::string p_value);
	std::string(*VK_Send)(int vkrid);
	void(*VK_SendOff)(VKRequest vkr);
	std::string(*VK_Send_)(std::string method, std::map<std::string, std::string> params, bool sendtoken);
}; //UserLogic -> EBP

extern core_api core;