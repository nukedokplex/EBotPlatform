#include "common.h"
#include "other.h"
#include "filesystem.h"
#include "ebp_api.h"
#include "cvar.h"
#include "console.h"

using namespace console;

string console::filename;

void console::init(string botdir)
{
	filename = botdir + "/logs/ebp_0.log";
	for (int i = 0; fs::exists(filename, false); i++) {
		filename = botdir + "/logs/ebp_"+to_string(i)+".log";
	}
	FILE *file = fopen(filename.c_str(), "w");
	fprintf(file, "==================================================\n");
	fprintf(file, ("     EBotPlatform "+ other::getVersionName() +"\n").c_str());
	fprintf(file, "     Copyright (C) 2017 Elektro-volk\n");
	fprintf(file, "==================================================\n");
	fclose(file);
}

void console::log(std::string text, std::string type) 
{
	if (FILE *file = fopen(filename.c_str(), "a")) {
		fprintf(file, ("{1}[" + other::getCurrentTime() + "]{9}[{2}" + type + "{9}]:{} " + text + "\n").c_str());
		fclose(file);
	}
	api_output.writeline("{9}[{2}"+ type +"{9}]:{} "+ text +"\n");
}

void console::error(std::string text, std::string type) 
{
	console::log(text, "{4}"+type+":ERROR");
}

void console::debug(string text, string type, string cvar_name)
{
	if (cvar::get(cvar_name) == "1")
		console::log(text, type);
}