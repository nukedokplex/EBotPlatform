#pragma once
#include <string>
#include <vector>
typedef std::string(*xcommand_t)(std::vector<std::string> cmd_args);

void Cmd_AddCommand(const std::string cmd_name, xcommand_t function, const std::string cmd_desc);
std::string Cmd_ExeCommand(std::string text);
std::vector<std::string> Cmd_ParseArgs(std::string line);
bool Cmd_Exists(const std::string cmd_name);
void Cmd_Init();
void Cmd_ExeConfig(std::string cpath);