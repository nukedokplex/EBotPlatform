#include "common.h"
#include "console.h"
#include "cmd.h"
#include "inetwork.h"
#include "cvar.h"
#include "vkwork.h"
#include "json.hpp"
#include "events.h"
#include <map>
#include <thread> 

using namespace vk;
using json = nlohmann::json;

std::map<int, VKRequest> vkrs;

void vk::init() {
	console::log("Initialization VKWork...", "Core:VK_Init");
	cvar::add("vk_token", "-", "Token on VKontakte");
	cvar::add("vk_version", "5.63", "Api version on VK");
}

int vk::registerVkr(VKRequest vkr) {
	int i = 0;
	while (vkrs.find(i) != vkrs.end())
		i++;
	vkrs[i] = vkr;
	return i;
}

int vk::create(std::string method, bool sendtoken) {
	VKRequest vkr = VKRequest();
	vkr.method = method;
	vkr.sendtoken = sendtoken;
	return vk::registerVkr(vkr);
}
 
void vk::set(int vkrid, std::string p_name, std::string p_value) {
	if (vkrs.find(vkrid) == vkrs.end())
	{
		console::error("VKRID not found", "vkwork");
		return;
	}
	vkrs[vkrid].params[p_name] = p_value.c_str();
}

std::string vk::send(int vkrid) {
	if (vkrs.find(vkrid) == vkrs.end())
	{
		console::error("VKRID not found", "vkwork");
		return "";
	}
	std::string r = send(vkrs[vkrid].method, vkrs[vkrid].params, vkrs[vkrid].sendtoken);
	vkrs.erase(vkrid);
	return r;
}

std::string vk::getToken() {
	return cvar::get("vk_token");
}

std::string vk::send(std::string method, std::map<std::string, std::string> params, bool sendtoken) {
	if (sendtoken)
		params["access_token"] = cvar::get("vk_token");
	params["v"] = cvar::get("vk_version");
	std::string str = (std::string)(char *)(Net_Post("https://api.vk.com/method/" + method, params));
	return str;
}

void vk::send_off(VKRequest vkr) {}


