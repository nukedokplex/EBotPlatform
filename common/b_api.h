#pragma once
#include <string>
#include <vector>
#include <map>

// CMD
typedef std::string(*xcommand_t)(std::vector<std::string> cmd_args);
// VKWORK
struct VKRequest {
	std::string method;
	bool sendtoken;
	std::map<std::string, std::string> params;
};