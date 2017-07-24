#include <map>
#include <thread>
#include "inetwork.h"
#include "events.h"
#include "longpoll.h"
#include "vkwork.h"

using namespace std;
using namespace longpoll;

map<int, void(*)(json update)> longpollUpdates;

string server;
string key;
int ts;

void longpoll::start()
{
	// Set longpoll events
	longpollUpdates[1] = update::replaceMessageFlags;
	longpollUpdates[2] = update::setMessageFlags;
	longpollUpdates[3] = update::flushMessageFlags;
	longpollUpdates[4] = update::newMessage;
	longpollUpdates[6] = update::readInMessage;
	longpollUpdates[7] = update::readOutMessage;
	longpollUpdates[8] = update::wasOnline;
	longpollUpdates[9] = update::wasOffline;
	longpollUpdates[10] = update::flushDialogFlags;
	longpollUpdates[11] = update::replaceDialogFlags;
	longpollUpdates[12] = update::setDialogFlags;
	longpollUpdates[51] = update::chatEdited;
	longpollUpdates[61] = update::typingDialog;
	longpollUpdates[62] = update::typingChat;
	longpollUpdates[80] = update::editUnreadCount;
	longpollUpdates[114] = update::editNotifyChat;
	// Start LongPoll
	getServer();
	std::thread loopthread(loop);
	loopthread.detach();
}

void longpoll::getServer()
{
	Console_Log("Get LongPoll server...", "Core:LongPoll");
	// Send response
	int getLongPollRequest = VK_CreateRequest("messages.getLongPollServer");
	json longpollinfo = json::parse(VK_Send(getLongPollRequest).c_str());
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
		map<string, string> params;
		params["act"] = "a_check";
		params["key"] = key;
		params["ts"] = to_string(ts);
		params["wait"] = "25";
		params["mode"] = "0";
		params["version"] = "2";
		// Send LongPoll request
		char *response = (char *)Net_Post("https://" + server, params);
		if (response != NULL)
		{
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
			data = NULL;
		}
	}
	// Catch errors
	catch (string e)
	{// String errors
		Console_Error("Error ("+e+"), reconnect...", "Core:LongPoll");
	}
	catch (...) 
	{// Unknow errors
		Console_Error("Unknow error, reconnect...", "Core:LongPoll");
	}
	// Start again
	loop();
}

void longpoll::getUpdate(json *update) {
	int code = update->at(0);
	std::thread loopthread(longpollUpdates[code], *update);
	loopthread.detach();
}

//Code: 1
void update::replaceMessageFlags(json update) {
	if (!Event_Exists("vk_flags_replace"))
		return;
	int eid = Event_New();
	Event_PushInt(eid, update.at(1));
	Event_PushInt(eid, update.at(2));
	Event_Call("vk_flags_replace", eid);
}
//Code: 2
void update::setMessageFlags(json update) {
	if (!Event_Exists("vk_message_set"))
		return;
	int eid = Event_New();
	Event_PushInt(eid, update.at(1));
	Event_PushInt(eid, update.at(2));
	Event_Call("vk_message_set", eid);
}
//Code: 3
void update::flushMessageFlags(json update) {
	if (!Event_Exists("vk_flags_remove"))
		return;
	int eid = Event_New();
	Event_PushInt(eid, update.at(1));
	Event_PushInt(eid, update.at(2));
	Event_Call("vk_flags_remove", eid);
}
//Code: 4
void update::newMessage(json update) {
	if (!Event_Exists("vk_message"))
		return;
	int eid = Event_New();
	Event_PushInt(eid, update.at(1));
	Event_PushInt(eid, update.at(2));
	Event_Call("vk_message", eid);
}
//Code: 6
void update::readInMessage(json update) {
	if (!Event_Exists("vk_messages_read_in"))
		return;
	int eid = Event_New();
	Event_PushInt(eid, update.at(1));
	Event_PushInt(eid, update.at(2));
	Event_Call("vk_messages_read_in", eid);
}
//Code: 7
void update::readOutMessage(json update) {
	if (!Event_Exists("vk_messages_read_out"))
		return;
	int eid = Event_New();
	Event_PushInt(eid, update.at(1));
	Event_PushInt(eid, update.at(2));
	Event_Call("vk_messages_read_out", eid);
}
//Code: 8
void update::wasOnline(json update) {
	if (!Event_Exists("vk_was_online"))
		return;
	int eid = Event_New();
	Event_PushInt(eid, update.at(1));
	Event_PushInt(eid, update.at(2));
	Event_PushInt(eid, update.at(3));
	Event_Call("vk_was_online", eid);
}
//Code: 9
void update::wasOffline(json update) {
	if (!Event_Exists("vk_was_offline"))
		return;
	int eid = Event_New();
	Event_PushInt(eid, update.at(1));
	Event_PushInt(eid, update.at(2));
	Event_PushInt(eid, update.at(3));
	Event_Call("vk_was_offline", eid);
}
//Code: 10
void update::flushDialogFlags(json update) {
	if (!Event_Exists("vk_dialog_flags_remove"))
		return;
	int eid = Event_New();
	Event_PushInt(eid, update.at(1));
	Event_PushInt(eid, update.at(2));
	Event_Call("vk_dialog_flags_remove", eid);
}
//Code: 11
void update::replaceDialogFlags(json update) {
	if (!Event_Exists("vk_dialog_flags_replace"))
		return;
	int eid = Event_New();
	Event_PushInt(eid, update.at(1));
	Event_PushInt(eid, update.at(2));
	Event_Call("vk_dialog_flags_replace", eid);
}
//Code: 12
void update::setDialogFlags(json update) {
	if (!Event_Exists("vk_dialog_flags_set"))
		return;
	int eid = Event_New();
	Event_PushInt(eid, update.at(1));
	Event_PushInt(eid, update.at(2));
	Event_Call("vk_dialog_flags_set", eid);
}
//Code: 51
void update::chatEdited(json update) {
	if (!Event_Exists("vk_chat_edit"))
		return;
	int eid = Event_New();
	Event_PushInt(eid, update.at(1));
	Event_PushInt(eid, update.at(2));
	Event_Call("vk_chat_edit", eid);
}
//Code: 61
void update::typingDialog(json update) {
	if (!Event_Exists("vk_typing"))
		return;
	int eid = Event_New();
	Event_PushInt(eid, update.at(1));
	Event_PushInt(eid, update.at(2));
	Event_Call("vk_typing", eid);
}
//Code: 62
void update::typingChat(json update) {
	if (!Event_Exists("vk_typing_chat"))
		return;
	int eid = Event_New();
	Event_PushInt(eid, update.at(1));
	Event_PushInt(eid, update.at(2));
	Event_Call("vk_typing_chat", eid);
}
//Code: 80
void update::editUnreadCount(json update) {
	if (!Event_Exists("vk_unread_edit"))
		return;
	int eid = Event_New();
	Event_PushInt(eid, update.at(1));
	Event_PushInt(eid, update.at(2));
	Event_Call("vk_unread_edit", eid);
}
//Code: 114
void update::editNotifyChat(json update) {
	if (!Event_Exists("vk_notify_edit"))
		return;
	int eid = Event_New();
	Event_PushInt(eid, update.at(1));
	Event_PushInt(eid, update.at(2));
	Event_PushInt(eid, update.at(3));
	Event_Call("vk_notify_edit", eid);
}