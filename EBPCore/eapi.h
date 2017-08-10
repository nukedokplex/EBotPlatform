#pragma once
#include "common.h"

namespace eapi
{
	// Structs
	struct element
	{
		string name;
		string server;
		string file;
		string version;
		int flags;
	};
	// Variables
	extern string base_url;
	extern vector<element> elements;
	// Voids
	void init();
	void start();
	string c_elements(args cmd_args);
	string c_getUpdates(args cmd_args);
	string c_updateAll(args cmd_args);
	string c_update(args cmd_args);
	string c_reinstall(args cmd_args);
	string c_install(args cmd_args);
	string c_apply(args cmd_args);
	void install(eapi::element *el);
	element *getUpdate(eapi::element *el);
	int getElement(string name);
	void load();
	void save();
}