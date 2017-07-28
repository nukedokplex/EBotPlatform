#include "cvar.h"
#include "common.h"
#include "console.h"
#include "cmd.h"
#include <map>
#include <vector>
using namespace cvar;

map<std::string, cvar_s> cvars;

void cvar::init()
{
	cmd::add("cvars", c_cvars, "Get cvar list");
}

std::string cvar::c_cvars(std::vector<std::string> cmd_args)
{
	std::string ret = "EBP " + common::getVersionName() + " cvars:";
	for (std::map <std::string, cvar_s> ::iterator it = cvars.begin(); it != cvars.end(); ++it)
	{
		ret += "\n" + it->first + " - " + it->second.desc;
	}
	return ret;
}

bool cvar::exists(const std::string cvar_name)
{
	return cvars.find(cvar_name) != cvars.end();
}


/*
============
cmd::add
============
*/
void cvar::add(const std::string cvar_name, std::string value, const std::string cvar_desc)
{
	cvar_s	cvar;

	if (cmd::exists(cvar_name))
	{
		console::log(cvar_name + " already defined as a command", "Core:cmd::add");
		return;
	}

	// fail if the command already exists
	if (exists(cvar_name))
	{
		console::log(cvar_name + " already defined", "Core:cmd::add");
		return;
	}

	cvar = cvar_s();
	cvar.name = cvar_name;
	cvar.desc = cvar_desc;
	cvar.value = value;
	cvars[cvar_name] = cvar;
}

/*
Function: cvar::get
Desc: Get value of cvar name
*/
std::string cvar::get(const std::string cvar_name)
{
	if (!exists(cvar_name)){
		console::log("Error: cvar \"" + cvar_name + "\" not found", "Core:cvar::get");
		return NULL;
	}
	return cvars.find(cvar_name)->second.value;
}

/*
	Установить значение
*/
bool cvar::set(const std::string cvar_name, std::string cvar_value)
{
	if (!exists(cvar_name)) {
		console::log("Error: cvar \"" + cvar_name + "\" not found", "Core:cvar::set");
		return false;
	}
	cvars[cvar_name].value = cvar_value;
	return true;
}