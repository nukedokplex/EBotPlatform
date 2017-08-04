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

void vk::init() 
{
	cvar::add("vk_token", "-", "Token on VKontakte");
	cvar::add("vk_version", "5.63", "Api version on VK");
}

void vk::request::set(std::string param, std::string value) 
{
	this->params[param] = value.c_str();
}

std::string vk::request::send()
{
	return vk::send(this->method, this->params, this->sendtoken);
}

std::string vk::getToken() {
	return cvar::get("vk_token");
}

std::string vk::send(std::string method, std::map<std::string, std::string> params, bool sendtoken) 
{
	if (sendtoken)
		params["access_token"] = cvar::get("vk_token");
	params["v"] = cvar::get("vk_version");
	return net::post("https://api.vk.com/method/" + method, params);
}