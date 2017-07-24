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

using json = nlohmann::json;

std::map<int, VKRequest> vkrs;



void VK_Init() {
	console::log("Initialization VKWork...", "Core:VK_Init");
	cvar::add("vk_token", "-", "Token on VKontakte");
	cvar::add("vk_version", "5.63", "Api version on VKontakte");
}

int VK_RegisterVKR(VKRequest vkr) {
	int i = 0;
	while (vkrs.find(i) != vkrs.end())
		i++;
	vkrs[i] = vkr;
	return i;
}

int VK_CreateRequest(std::string method, bool sendtoken) {
	VKRequest vkr = VKRequest();
	vkr.method = method;
	vkr.sendtoken = sendtoken;
	return VK_RegisterVKR(vkr);
}
 
std::string cp1251_to_Utf8(const char *str);
void VK_SetParam(int vkrid, std::string p_name, std::string p_value) {
	vkrs[vkrid].params[p_name] = cp1251_to_Utf8(p_value.c_str());
}

std::string VK_Send(int vkrid) {
	VKRequest vkr = vkrs[vkrid];
	vkrs.erase(vkrid);
	return VK_Send(vkr.method, vkr.params, vkr.sendtoken);
}

std::string VK_GetToken() {
	return cvar::get("vk_token");
}

std::string Utf8_to_cp1251(const char *str);
std::string VK_Send(std::string method, std::map<std::string, std::string> params, bool sendtoken) {
	if (sendtoken)
		params["access_token"] = cvar::get("vk_token");
	params["v"] = cvar::get("vk_version");
	std::string str = Utf8_to_cp1251((char *)(Net_Post("https://api.vk.com/method/" + method, params)));
	return str;
}


void VK_SendOff(VKRequest vkr) {}


