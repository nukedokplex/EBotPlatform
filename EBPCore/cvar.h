#pragma once

#include <string>
#include <vector>

namespace cvar
{
	struct cvar_s
	{
		std::string name;
		std::string value;
		std::string desc;
	};
	// Functions
	bool exists(const std::string cvar_name);
	void add(const std::string cvar_name, std::string value, const std::string cvar_desc);
	std::string get(const std::string cvar_name);
	bool set(const std::string cvar_name, std::string cvar_value);
}