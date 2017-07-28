#pragma once
#include <string>
#include <map>
#include "common.h"
#include "console.h"
#include "../common/b_api.h"

namespace vk
{
	void init();
	int registerVkr(VKRequest vkr);
	int create(std::string method, bool sendtoken = true);
	void set(int vkridr, std::string p_name, std::string p_value);
	std::string send(int vkrid);
	void send_off(VKRequest vkr);
	std::string send(std::string method, std::map<std::string, std::string> params, bool sendtoken = true);
	std::string getToken();
}