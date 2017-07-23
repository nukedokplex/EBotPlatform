#pragma once
#include <string>
#include <map>
#include "common.h"
#include "console.h"


void *Net_Get(std::string url);
void *Net_Post(std::string url, std::map<std::string, std::string> params);
void Net_Init();
// Lua
int Net_CreatePost(std::string url);
void Net_SetParam(int prid, std::string p_name, std::string p_value);
std::string Net_Send(int prid);