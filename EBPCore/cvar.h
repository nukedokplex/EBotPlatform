#pragma once

#include <string>
#include <vector>

bool Cvar_Exists(const std::string cvar_name);
void Cvar_AddCvar(const std::string cvar_name, std::string value, const std::string cvar_desc);
std::string Cvar_GetValue(const std::string cvar_name);
bool Cvar_SetValue(const std::string cvar_name, std::string cvar_value);