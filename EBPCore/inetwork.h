#pragma once
#include <string>
#include <map>
#include "common.h"
#include "console.h"


void *Net_Get(std::string url);
void *Net_Post(std::string url, std::map<std::string, std::string> params);
void Net_Init();