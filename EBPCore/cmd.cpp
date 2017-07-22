#include "cmd.h"
#include "common.h"
#include "console.h"
#include "cvar.h"
#include <map>
#include <vector>
#include "filesystem.h"


struct cmd_s
{
	std::string name;
	xcommand_t	function;
	std::string desc;
};

// Local
std::map<std::string, cmd_s> commands;
std::string Exec_Command(std::vector<std::string> cmd_args);
std::string Loh_Command(std::vector<std::string> cmd_args);

/*
	Инициализация команд
*/
void Cmd_Init()
{
	Console_Log("Initialization Commands...", "Core:Cmd_Init");
	Cmd_AddCommand("title", Title_Command, "Set window title");
	Cmd_AddCommand("exec", Exec_Command, "Execute config file");
	Cmd_AddCommand("loh", Loh_Command, "Get loh");
}

/*
	Есть ли команда
*/
bool Cmd_Exists(const std::string cmd_name) {
	return commands.find(cmd_name) != commands.end();
}

/*
	Команда запуска конфига
*/
std::string Exec_Command(std::vector<std::string> cmd_args)
{
	if (cmd_args.size() != 2) {
		return "Use \"exec <config path>\"";
	}
	Cmd_ExeConfig(cmd_args[1]);
	return "executing " + cmd_args[1] + "...";
}

/*
	Добавить команду в консоль
*/
void Cmd_AddCommand(const std::string cmd_name, xcommand_t function, const std::string cmd_desc)
{
	cmd_s	cmd;
	if (Cvar_Exists(cmd_name))
	{
		Console_Log(cmd_name+" already defined as a сvar", "Core:Cmd_AddCommand");
		return;
	}
	if (Cmd_Exists(cmd_name))
	{
		Console_Log(cmd_name +" already defined\n", "Core:Cmd_AddCommand");
		return;
	}
	cmd = cmd_s();
	cmd.name = cmd_name;
	cmd.desc = cmd_desc;
	cmd.function = function;
	commands[cmd_name] = cmd;
}

/*
Запустить линию команды
*/
std::string Cmd_ExeCommand(std::string text)
{
	std::vector<std::string> cmd_args = Cmd_ParseArgs(text);
	// check functions
	if (Cmd_Exists(cmd_args[0]))
	{
		return commands.find(cmd_args[0])->second.function(cmd_args);
	} else if (Cvar_Exists(cmd_args[0]))
	{
		if (cmd_args.size() == 2) {
			Cvar_SetValue(cmd_args[0], cmd_args[1]);
			return cmd_args[0] + " = " + Cvar_GetValue(cmd_args[0]);
		}
		else {
			return cmd_args[0] + " = " + Cvar_GetValue(cmd_args[0]);
		}
	}
	else
		return "Unknown command \"" + cmd_args[0] + "\"\n";
}

std::vector<std::string> Cmd_ParseArgs(std::string text)
{
	bool nospace = 0;
	int index = 0;
	std::vector<std::string> cmd_args;

	if (text.empty()) return std::vector<std::string>();
	if (text.find(' ') == std::string::npos) {
		cmd_args.push_back(text);
		return cmd_args;
	}

	cmd_args.push_back("");
	for (int i = 0;i < strlen(text.c_str());i++) {
		char ch = text.c_str()[i];
		if (ch == ' ' && !nospace) {
			index++;
			cmd_args.push_back("");
		}
		else if (ch == '"')
			nospace = !nospace;
		else
			cmd_args[index] += ch;
	}
	return cmd_args;
}

void Cmd_ExeConfig(std::string cpath)
{
	std::string s;
	std::fstream file = FS_OpenFile(cpath);
	while (std::getline(file, s)) {
		if (s=="" || s.front() == '#')
			continue;
		Cmd_ExeCommand(s);
	}
	file.close();
}