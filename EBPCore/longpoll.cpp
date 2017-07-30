#include <map>
#include <thread>
#include "inetwork.h"
#include "events.h"
#include "longpoll.h"
#include "vkwork.h"

using namespace std;
using namespace longpoll;


map<int, update> longpollUpdates;

string server;
string key;
int ts;

void longpoll::start()
{
	// Set longpoll events
	longpollUpdates[1] = { "vk_flags_replace", false };
	longpollUpdates[2] = { "vk_flags_set", false };
	longpollUpdates[3] = { "vk_flags_remove", false };
	longpollUpdates[4] = { "vk_message", false };
	longpollUpdates[6] = { "vk_messages_read_in", false };
	longpollUpdates[7] = { "vk_messages_read_out", false };
	longpollUpdates[8] = { "vk_was_online", true };
	longpollUpdates[9] = { "vk_was_offline", true };
	longpollUpdates[10] = { "vk_dialog_flags_remove", false };
	longpollUpdates[11] = { "vk_dialog_flags_replace", false };
	longpollUpdates[12] = { "vk_dialog_flags_set", false };
	longpollUpdates[51] = { "vk_chat_edit", false };
	longpollUpdates[61] = { "vk_typing", false };
	longpollUpdates[62] = { "vk_typing_chat", false };
	longpollUpdates[80] = { "vk_unread_edit", false };
	longpollUpdates[114] = { "vk_notify_edit", true };
	// Start LongPoll
	getServer();
	std::thread loopthread(startLoop);
	loopthread.detach();
}

void longpoll::startLoop()
{
	while (true)
	{
		loop();
	}
}

void longpoll::getServer()
{
	console::log("Get LongPoll server...", "Core:LongPoll");
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
		net::request *longPollRequest = new net::request("https://" + server);
		longPollRequest->set("act","a_check");
		longPollRequest->set("key", key);
		longPollRequest->set("ts", to_string(ts));
		longPollRequest->set("wait", "25");
		longPollRequest->set("mode", "0");
		longPollRequest->set("version", "2");
		// Send LongPoll request
		string response = longPollRequest->send().c_str();

			json data = json::parse(response);
			// Check errors
			if (data.find("failed") != data.end())
			{
				getServer();
				throw to_string((int)data.at("failed"));
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
		console::error("Error ("+e+"), reconnect...", "Core:LongPoll");
	}
	catch (...) 
	{// Unknow errors
		console::error("Unknow error, reconnect...", "Core:LongPoll");
	}
}

void longpoll::getUpdate(json *update) {
	int code = update->at(0);
	longpoll::update *upd = &longpollUpdates[code];

	if (!Event_Exists(upd->name))
		return;
	int eid = Event_New();
	Event_PushInt(eid, update->at(1));
	Event_PushInt(eid, update->at(2));
	if(upd->dopValue)
		Event_PushInt(eid, update->at(3));
	std::thread loopthread(Event_Call, upd->name, eid);
	loopthread.detach();
}
