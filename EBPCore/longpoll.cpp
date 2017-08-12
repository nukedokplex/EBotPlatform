#include "common.h"
#include <thread> // std::thread
#include "inetwork.h" // 
#include "events.h"
#include "vkwork.h"
#include "longpoll.h"
#include <windows.h> // Sleep

using namespace longpoll;

map<int, update> longpoll::longpollUpdates;
string longpoll::server;
string longpoll::key;
int longpoll::ts;

void longpoll::start()
{
	// Set longpoll events
	longpollUpdates[1] = { "vk_flags_replace", {1, 1} };
	longpollUpdates[2] = { "vk_flags_set", {1, 1} };
	longpollUpdates[3] = { "vk_flags_remove", { 1, 1 } };
	longpollUpdates[4] = { "vk_message", { 1, 1, 1, 1, 2 } }; // mid, flags, peer_id, time, text
	longpollUpdates[6] = { "vk_messages_read_in", { 1, 1 } };
	longpollUpdates[7] = { "vk_messages_read_out", { 1, 1 } };
	longpollUpdates[8] = { "vk_was_online",  {1, 1,1} };
	longpollUpdates[9] = { "vk_was_offline",{ 1, 1,1 } };
	longpollUpdates[10] = { "vk_dialog_flags_remove",{ 1, 1 } };
	longpollUpdates[11] = { "vk_dialog_flags_replace",{ 1, 1 } };
	longpollUpdates[12] = { "vk_dialog_flags_set",{ 1, 1 } };
	longpollUpdates[51] = { "vk_chat_edit",{ 1, 1 } };
	longpollUpdates[61] = { "vk_typing",{ 1, 1 } };
	longpollUpdates[62] = { "vk_typing_chat",{ 1, 1 } };
	longpollUpdates[80] = { "vk_unread_edit",{ 1, 1 } };
	longpollUpdates[114] = { "vk_notify_edit",{ 1, 1,1 } };
	// Start LongPoll
	getServer();
	startLoop();
}

void longpoll::startLoop()
{
	while (true)
	{
		Sleep(50);
		loop();
	}
}

void longpoll::getServer()
{
	console::log("Getting LongPoll server...", "LongPoll");
	// Send response
	vk::request getLongPollRequest = vk::request("messages.getLongPollServer");
	json longpollinfo = json::parse(getLongPollRequest.send().c_str());
	// Get values
	string _server = longpollinfo.at("response").at("server");
	string _key = longpollinfo.at("response").at("key");
	int _ts = longpollinfo.at("response").at("ts");
	// Save values (FUCKING JSON)
	server = _server;
	key    = _key;
	ts     = _ts;
}

void longpoll::loop() {
	try {
		// Set data
		net::request longPollRequest = net::request("https://" + server);
		longPollRequest.set("act","a_check");
		longPollRequest.set("key", key);
		longPollRequest.set("ts", to_string(ts));
		longPollRequest.set("wait", "25");
		longPollRequest.set("mode", "0");
		longPollRequest.set("version", "2");
		// Send LongPoll request
		string response = longPollRequest.send().c_str();
		json data = json::parse(response);
		// Check errors
		if (data.find("failed") != data.end())
		{
			console::log("Reconnect... (" + to_string((int)data.at("failed")) + ")", "LongPoll");
			getServer();
			return;
		}
		// Get updates
		json updates = data.at("updates");
		int updateCount = updates.size();
		if (updateCount != 0)
		{
			for (json::iterator it = updates.begin(); it != updates.end(); ++it) 
			{
				json update = *it;
				getUpdate(&update);
			}
		}
		ts = data.at("ts");// Update TS
		data = 0;
	}
	
	// Catch errors
	catch (string e)
	{// String errors
		console::error("Error ("+e+"), reconnect...", "LongPoll");
		Sleep(100);
	}
	catch (...) 
	{// Unknown errors
		console::error("Unknown error, reconnect...", "LongPoll");
		Sleep(1000);
	}
}

void longpoll::getUpdate(json *update) 
{
	int code = update->at(0);
	longpoll::update upd = longpollUpdates[code];

	if (events::caller *caller = events::create(upd.name))
	{
		for (int i = 0; i < upd.values.size(); i++)
		{
			switch (upd.values[i])
			{
			case 1: caller->pushInt(update->at(i + 1)); break;
			case 2: caller->pushString(update->at(i + 1)); break;
			default:
				break;
			}
			
		}
		caller->call();
	}
}