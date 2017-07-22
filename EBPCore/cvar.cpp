#include "cvar.h"
#include "common.h"
#include "console.h"
#include "cmd.h"
#include <map>
#include <vector>

struct cvar_s
{
	std::string name;
	std::string value;
	std::string desc;
};

std::map<std::string, cvar_s> cvars;

bool Cvar_Exists(const std::string cvar_name)
{
	return cvars.find(cvar_name) != cvars.end();
}


/*
============
Cmd_AddCommand
============
*/
void Cvar_AddCvar(const std::string cvar_name, std::string value, const std::string cvar_desc)
{
	cvar_s	cvar;

	if (Cmd_Exists(cvar_name))
	{
		Console_Log(cvar_name + " already defined as a command", "Core:Cmd_AddCommand");
		return;
	}

	// fail if the command already exists
	if (Cvar_Exists(cvar_name))
	{
		Console_Log(cvar_name + " already defined", "Core:Cmd_AddCommand");
		return;
	}

	cvar = cvar_s();
	cvar.name = cvar_name;
	cvar.desc = cvar_desc;
	cvar.value = value;
	cvars[cvar_name] = cvar;
}

/*
Function: Cvar_GetValue
Desc: Get value of cvar name
*/
std::string Cvar_GetValue(const std::string cvar_name)
{
	if (!Cvar_Exists(cvar_name)){
		Console_Log("Error: cvar \"" + cvar_name + "\" not found", "Core:Cvar_GetValue");
		return NULL;
	}
	return cvars.find(cvar_name)->second.value;
}

/*
	Установить значение
*/
bool Cvar_SetValue(const std::string cvar_name, std::string cvar_value)
{
	if (!Cvar_Exists(cvar_name)) {
		Console_Log("Error: cvar \"" + cvar_name + "\" not found", "Core:Cvar_SetValue");
		return false;
	}
	cvars[cvar_name].value = cvar_value;
	return true;
}