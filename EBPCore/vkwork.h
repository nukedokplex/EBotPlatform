#pragma once
#include <string>
#include <map>
#include "common.h"
#include "console.h"
#include "../common/b_api.h"

void VK_Init();
int VK_CreateRequest(std::string method, bool sendtoken = true);
void VK_SetParam(int vkridr, std::string p_name, std::string p_value);
std::string VK_Send(int vkrid);
void VK_SendOff(VKRequest vkr);
std::string VK_Send(std::string method, std::map<std::string, std::string> params, bool sendtoken=true);
std::string VK_GetToken();