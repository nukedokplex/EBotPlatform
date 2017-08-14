#pragma once


typedef struct
{
	void(*Console_Log)(std::string text, std::string type);
	std::string(*Console_ExeCommand)(std::string commandline);
	int (*Host_Start)();
} inputApi; //Launcher -> EBP

typedef struct
{
	void(*writeline)(std::string text);
	void(*setwindowtitle)(std::string text);
} outputApi; //EBP -> Launcher

extern inputApi api_input;
extern outputApi api_output;