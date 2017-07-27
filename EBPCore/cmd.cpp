#include "cmd.h"
#include "common.h"
#include "console.h"
#include "cvar.h"
#include <map>
#include <vector>
#include "filesystem.h"

using namespace cmd;



// Local
std::map<std::string, command> commands;

/*
	Инициализация команд
*/
void cmd::init()
{
	console::log("Initialization Commands...", "Core:Cmd_Init");
	cmd::add("title", common::c_title, "Set window title");
	cmd::add("exec", c_exec, "Execute config file");
}

/*
	Есть ли команда
*/
bool cmd::exists(const std::string cmd_name) {
	return commands.find(cmd_name) != commands.end();
}

/*
	Команда запуска конфига
*/
std::string cmd::c_exec(std::vector<std::string> cmd_args)
{
	if (cmd_args.size() != 2) {
		return "Use \"exec <config path>\"";
	}
	exec(cmd_args[1]);
	return "executing " + cmd_args[1] + "...";
}

/*
	Добавить команду в консоль
*/
void cmd::add(const std::string cmd_name, func function, const std::string cmd_desc)
{
	command	cmd;
	if (cvar::exists(cmd_name))
	{
		console::log(cmd_name+" already defined as a сvar", "Core:cmd::add");
		return;
	}
	if (exists(cmd_name))
	{
		console::log(cmd_name +" already defined\n", "Core:cmd::add");
		return;
	}
	cmd = command();
	cmd.name = cmd_name;
	cmd.desc = cmd_desc;
	cmd.function = function;
	commands[cmd_name] = cmd;
}

/*
Запустить линию команды
*/
std::string cmd::exe(std::string text)
{
	if (text == "")
		return "";
	std::vector<std::string> cmd_args = parse(text);
	// check functions
	if (exists(cmd_args[0]))
	{
		return commands.find(cmd_args[0])->second.function(cmd_args);
	} else if (cvar::exists(cmd_args[0]))
	{
		if (cmd_args.size() == 2) {
			cvar::set(cmd_args[0], cmd_args[1]);
			return cmd_args[0] + " = " + cvar::get(cmd_args[0]);
		}
		else {
			return cmd_args[0] + " = " + cvar::get(cmd_args[0]);
		}
	}
	else
		return "Unknown command \"" + cmd_args[0] + "\"\n";
}

std::vector<std::string> cmd::parse(std::string text)
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

void cmd::exec(std::string cpath)
{
	std::string s;
	std::fstream file = FS_OpenFile(cpath);
	while (std::getline(file, s)) {
		if (s=="" || s.front() == '#')
			continue;
		exe(s);
	}
	file.close();
}