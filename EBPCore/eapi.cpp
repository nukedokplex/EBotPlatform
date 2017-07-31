#include "common.h"
#include "eapi.h"
#include "inetwork.h"
#include "cvar.h"
#include "cmd.h"
#include "json.hpp"
#include "filesystem.h" // fs::writeData

using json = nlohmann::json;

string eapi::base_url = "http://ebp.elektro-volk.ru/api/";
vector<eapi::element> eapi::elements;

void eapi::init()
{
	cvar::add("eapi_key", "-", "Key on EAPI");
	cmd::add("updates", eapi::c_getUpdates, "Get updates list");
	cmd::add("update_all", eapi::c_updateAll, "Update all elements");
	cmd::add("update", eapi::c_update, "Update element");
	cmd::add("install", eapi::c_install, "Install element");
}

void Host_End();
void eapi::start()
{
	console::log("Authorization in EAPI...", "EAPI");
	net::request auth = net::request(eapi::base_url+"check.php");
	auth.set("key", cvar::get("eapi_key"));
	json response = json::parse(auth.send());
	if (response.at("status") == "ok")
	{
		console::log("Successfully authorized", "EAPI");
	}
	else {
		console::error("Authorization Error", "EAPI");
		console::error("Please install the eapi_key in your key", "EAPI");
		console::error("The key is available on your bot page", "EAPI");
		console::error("Read more: ebp.elektro-volk.ru/mybots", "EAPI");
		Host_End();
	}
}

string eapi::c_getUpdates(args cmd_args)
{
	return "";
}

string eapi::c_updateAll(args cmd_args)
{
	return "";
}

string eapi::c_update(args cmd_args)
{
	return "";
}

string eapi::c_install(args cmd_args)
{
	string el_name = cmd_args[1];

	net::request get = net::request(eapi::base_url + "get.php");
	get.set("name", el_name);
	json response = json::parse(get.send());
	if (response.at("status") == "ok")
	{
		string filename = response.at("element").at("filename");
		string url      = response.at("element").at("url");
		string version  = response.at("element").at("version");

		element el = element();
		el.file = filename;
		el.server = url;
		el.name = el_name;
		el.version = version;
		elements.push_back(el);

		eapi::install(&el);
		return "Element has been installed\nVersion: " + el.version;
	}
	else {
		console::error("Element not found", "EAPI");
		return "Element not found";
	}
}

void eapi::install(eapi::element *el)
{
	console::log("Download "+el->name+" from "+el->server, "EAPI");
	string file = (char *)net::get(el->server);
	fs::writeData(el->file, file);
	console::log("Downloaded " + el->name + "; Size: " + to_string(file.size()*2/*WTF?*/) + " bytes.", "EAPI");
}