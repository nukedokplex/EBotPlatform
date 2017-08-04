#include "common.h"
#include "eapi.h"
#include "inetwork.h"
#include "cvar.h"
#include "cmd.h"
#include "json.hpp"
#include "filesystem.h" // fs::writeData
#include <LuaBridge.h>

using json = nlohmann::json;

string eapi::base_url = "http://ebp.elektro-volk.ru/api/";
vector<eapi::element> eapi::elements;
vector<string> updateReloads;

void eapi::init()
{
	cvar::add("eapi_key", "-", "Key on EAPI");
	cmd::add("elements", eapi::c_elements, "Get elements list");
	cmd::add("updates", eapi::c_getUpdates, "Get updates list");
	cmd::add("update_all", eapi::c_updateAll, "Update all elements");
	cmd::add("update", eapi::c_update, "Update element");
	cmd::add("install", eapi::c_install, "Install element");
	cmd::add("apply", eapi::c_apply, "Apply core updates");
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
	eapi::load();
	console::log(c_getUpdates(args()), "EAPI");
}


/*========== COMMANDS ==========*/

//	Get updates list
string eapi::c_getUpdates(args cmd_args)
{
	string updates = "";
	for (int i = 0; i < elements.size(); i++)
	{
		if (element *el = eapi::getUpdate(&elements[i]))
		{
			updates += "********************\n";
			updates += "Name: "+el->name+"\n";
			updates += "Version: " + elements[i].version + "\n";
			updates += "New version: " + el->version + "\n";
			updates += "Command: update " + el->name + "\n";
		}
	}
	if (updates == "")
		return "Updates not found";
	return updates;
}

//	Get elements list
string eapi::c_elements(args cmd_args)
{
	string updates = "";
	for (int i = 0; i < elements.size(); i++)
	{
			updates += "********************\n";
			updates += "Name: " + elements[i].name + "\n";
			updates += "Version: " + elements[i].version + "\n";
			updates += "File: " + elements[i].file + "\n";
	}
	if (updates == "")
		return "Elements not found";
	return "Your elements: \n"+updates;
}

//	Get update all elements list
string eapi::c_updateAll(args cmd_args)
{
	return "";
}

//	Update element
string eapi::c_update(args cmd_args)
{
	string el_name = cmd_args[1];
	int i = eapi::getElement(el_name);
	if (i!=-1)
	{
		if (element *el = eapi::getUpdate(&elements[i]))
		{
			elements[i] = *el;
			eapi::save();
			eapi::install(el);
			return "Element has been updated\nNew version: " + el->version;
		}
		else {
			return "You already have the latest version";
		}
	}
	else {
		console::error("Element not found", "EAPI");
		return "Element not found";
	}
}

//	Install element
string eapi::c_install(args cmd_args)
{
	string el_name = cmd_args[1];

	net::request get = net::request(eapi::base_url + "get.php");
	get.set("name", el_name);
	json response = json::parse(get.send());
	if (int i = eapi::getElement(el_name) == -1)
	{
		if (response.at("status") == "ok")
		{
			string filename = response.at("element").at("filename");
			string url = response.at("element").at("url");
			string version = response.at("element").at("version");
			int flags = response.at("element").at("flags");

			element el = element();
			el.file = filename;
			el.server = url;
			el.name = el_name;
			el.version = version;
			el.flags = flags;
			elements.push_back(el);
			eapi::save();

			eapi::install(&el);
			return "Element has been installed\nVersion: " + el.version;
		}
		else {
			console::error("Element not found", "EAPI");
			return "Element not found";
		}
	}
	else {
		console::error("The item is already installed. You can delete it with a command", "EAPI");
		console::error("uninstall " + el_name, "EAPI");
		return "The item is already installed. You can delete it with a command\nuninstall "+el_name;
	}
}

//	Apply core updates
string eapi::c_apply(args cmd_args)
{
	if (updateReloads.size()!=0)
	{
		fs::file *updatesFile = fs::openOrCreate("update.bat", false, 4);
		updatesFile->write("@echo off\n");
		
		for (int i = 0; i < updateReloads.size(); i++)
		{
			updatesFile->write("move " + updateReloads[i] + ".update " + updateReloads[i] + "\n");
		}

		updatesFile->write("start EBPLauncher.exe\n");
		updatesFile->write("del /q update.bat");
		updatesFile->close();
		system("start update.bat");
		exit(0);
	}
	else
		return "No waiting updates";
}

/*-----------COMMANDS-----------*/

eapi::element *eapi::getUpdate(eapi::element *_el)
{
	net::request get = net::request(eapi::base_url + "get.php");
	get.set("name", _el->name);
	json response = json::parse(get.send());

	string filename = response.at("element").at("filename");
	string url = response.at("element").at("url");
	string version = response.at("element").at("version");
	int flags = response.at("element").at("flags");
	if (version == _el->version)
		return NULL;
	else{
		element *el = new element();
		el->file = filename;
		el->server = url;
		el->name = _el->name;
		el->version = version;
		el->flags = flags;
		return el;
	}
}

void eapi::install(eapi::element *el)
{
	console::log("Downloading "+el->name+" from "+el->server, "EAPI");
	net::request file_data = net::request(el->server);

	string path = el->flags & 2 ? "../" + el->file : el->file;

	if (el->flags & 4) {// Binary
		file_data.download(path + ".update");
		updateReloads.push_back(el->file);
		console::log("Enter apply for the changes to take effect. (It restart program!)", "EAPI");
	}
	else {// USER
		fs::writeData(path, file_data.send());
	}
	console::log("Downloaded: " + el->name, "EAPI");
}

int eapi::getElement(string name)
{
	for (int i = 0; i < elements.size(); i++)
	{
		if (name == elements[i].name)
			return i;
	}
	return -1;
}

void eapi::load()
{
	if (!fs::exists("datafiles/eapi_elements.json"))
	{
		console::log("Install default elements...", "EAPI");
		eapi::c_install({"", "core_ebp"});
		eapi::c_install({ "", "core_launcher" });
		// Lua
		eapi::c_install({ "", "data_lua_json" });
		eapi::c_install({ "", "data_lua_tools" });
		eapi::c_install({ "", "data_lua_errors" });
		eapi::c_install({ "", "data_lua_ebptool" });
		eapi::c_apply({});
		return;
	}
	
	json loaddata = json::parse(fs::readData("datafiles/eapi_elements.json"));
	for (json::iterator elementdata = loaddata.begin(); elementdata != loaddata.end(); ++elementdata) {
		string _file = elementdata->at("file");
		string _server = elementdata->at("server");
		string _name = elementdata->at("name");
		string _version = elementdata->at("version");
		int _flags = elementdata->at("flags");

		element el = element();
		el.file = _file;
		el.server = _server;
		el.name = _name;
		el.version = _version;
		el.flags = _flags;
		elements.push_back(el);
	}
	console::log("Loaded " + to_string(elements.size()) + " elements.", "EAPI");
}

void eapi::save()
{
	json savedata;
	for (int i = 0; i < elements.size(); i++)
	{
		json elementdata;
		elementdata["name"] = elements[i].name;
		elementdata["server"] = elements[i].server;
		elementdata["file"] = elements[i].file;
		elementdata["version"] = elements[i].version;
		elementdata["flags"] = elements[i].flags;
		savedata.push_back(elementdata);
	}
	fs::writeData("datafiles/eapi_elements.json", savedata.dump());
}