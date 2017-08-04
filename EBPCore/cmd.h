#pragma once
#include <string>
#include <vector>

using namespace std;

namespace cmd
{
	typedef string(*func)(vector<string> cmd_args);
	struct command
	{
		string name;
		cmd::func function;
		string desc;
	};

	void add(const string cmd_name, func function, const string cmd_desc);
	string exe(string text);
	vector<string> parse(string line);
	bool exists(const string cmd_name);
	void init();
	void exec(string cpath);
	string c_exec(vector<string> cmd_args);
	string c_help(vector<string> cmd_args);
	string data(args cmd_args, int sub);
}