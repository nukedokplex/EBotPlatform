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
#include <locale>
//#include "glib-2.0\glib\gthread.h"

using json = nlohmann::json;

std::map<int, VKRequest> vkrs;

//Longpoll events
void VKW_Tr_ReplaceMessageFlags(json update);
void VKW_Tr_SetMessageFlags(json update);
void VKW_Tr_FlushMessageFlags(json update);
void VKW_Tr_NewMessage(json update);
void VKW_Tr_ReadInMessage(json update);
void VKW_Tr_ReadOutMessage(json update);
void VKW_Tr_WasOnline(json update);
void VKW_Tr_WasOffline(json update);
void VKW_Tr_FlushDialogFlags(json update);
void VKW_Tr_ReplaceDialogFlags(json update);
void VKW_Tr_SetDialogFlags(json update);
void VKW_Tr_ChatEdited(json update);
void VKW_Tr_TypingDialog(json update);
void VKW_Tr_TypingChat(json update);
void VKW_Tr_EditUnreadCount(json update);
void VKW_Tr_EditNotifyChat(json update);
std::map<int, void(*)(json update)> longpollUpdates;

void VK_Init() {
	Console_Log("Initialization VKWork...", "Core:VK_Init");
	Cvar_AddCvar("vk_token", "-", "Token on VKontakte");
	Cvar_AddCvar("vk_version", "5.63", "Api version on VKontakte");
	longpollUpdates[1] = VKW_Tr_ReplaceMessageFlags;
	longpollUpdates[2] = VKW_Tr_SetMessageFlags;
	longpollUpdates[3] = VKW_Tr_FlushMessageFlags;
	longpollUpdates[4] = VKW_Tr_NewMessage;
	longpollUpdates[6] = VKW_Tr_ReadInMessage;
	longpollUpdates[7] = VKW_Tr_ReadOutMessage;
	longpollUpdates[8] = VKW_Tr_WasOnline;
	longpollUpdates[9] = VKW_Tr_WasOffline;
	longpollUpdates[10] = VKW_Tr_FlushDialogFlags;
	longpollUpdates[11] = VKW_Tr_ReplaceDialogFlags;
	longpollUpdates[12] = VKW_Tr_SetDialogFlags;
	longpollUpdates[51] = VKW_Tr_ChatEdited;
	longpollUpdates[61] = VKW_Tr_TypingDialog;
	longpollUpdates[62] = VKW_Tr_TypingChat;
	longpollUpdates[80] = VKW_Tr_EditUnreadCount;
	longpollUpdates[114] = VKW_Tr_EditNotifyChat;
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
 
void VK_SetParam(int vkrid, std::string p_name, std::string p_value) {
	vkrs[vkrid].params[p_name] = p_value;
}

std::string VK_Send(int vkrid) {
	VKRequest vkr = vkrs[vkrid];
	vkrs.erase(vkrid);
	return VK_Send(vkr.method, vkr.params, vkr.sendtoken);
}

std::string VK_Send(std::string method, std::map<std::string, std::string> params, bool sendtoken) {
	if (sendtoken)
		params["access_token"] = Cvar_GetValue("vk_token");
	params["v"] = Cvar_GetValue("vk_version");
	std::string str = (char *)(Net_Post("https://api.vk.com/method/" + method, params));
	return str;
}


void VK_SendOff(VKRequest vkr) {}


/*========== VKWork loop ==========*/
std::map<std::string, std::string> params;
json longpollinfo;

void VKW_Loop();
void VKW_Start() {
	Console_Log("Start VKWork Longpoll loop", "Core:VKW_Start");
	int vkr = VK_CreateRequest("messages.getLongPollServer");
	longpollinfo = json::parse(VK_Send(vkr).c_str());
	std::string key = longpollinfo.at("response").at("key");
	int ts = longpollinfo.at("response").at("ts");
	params["act"] = "a_check";
	params["key"] = key;
	params["ts"] = std::to_string(ts);
	params["wait"] = "25";
	params["mode"] = "0";
	params["version"] = "1";
	std::thread loopthread(VKW_Loop);
	loopthread.detach();
}
void VKW_TrUpdate(json *update);
void VKW_Loop() {
	std::string server = longpollinfo.at("response").at("server");
	char *resp = (char *)Net_Post("https://" + server, params);
	if (resp!=NULL)
	{
		json respJ = json::parse(resp);
		if (respJ.find("failed") != respJ.end())
			return VKW_Start();

		int cc = respJ.at("updates").size();
		if (cc != 0)
			for (json::iterator it = respJ["updates"].begin(); it != respJ["updates"].end(); ++it) {
				json update = *it;
				VKW_TrUpdate(&update);
			}

		int ts = respJ.at("ts");
		params.at("ts") = std::to_string(ts);
	}
	else
		Console_Error("LongPoll unknow error, reconnect.", "VKW_Loop");
	VKW_Loop();
}

void VKW_TrUpdate(json *update) {
	int code = update->at(0);
	std::thread loopthread(longpollUpdates[code], *update);
	loopthread.detach();
}

//Code: 1
void VKW_Tr_ReplaceMessageFlags(json update) {
	Event_Flush();
	Event_PushInt(update.at(1));
	Event_PushInt(update.at(2));
	Event_Call("vk_flags_replace");
}
//Code: 2
void VKW_Tr_SetMessageFlags(json update) {
	Event_Flush();
	Event_PushInt(update.at(1));
	Event_PushInt(update.at(2));
	Event_Call("vk_message_set");
}
//Code: 3
void VKW_Tr_FlushMessageFlags(json update) {
	Event_Flush();
	Event_PushInt(update.at(1));
	Event_PushInt(update.at(2));
	Event_Call("vk_flags_remove");
}
//Code: 4
void VKW_Tr_NewMessage(json update) {
	Event_Flush();
	Event_PushInt(update.at(1));
	Event_PushInt(update.at(2));
	Event_Call("vk_message");
}
//Code: 6
void VKW_Tr_ReadInMessage(json update) {
	Event_Flush();
	Event_PushInt(update.at(1));
	Event_PushInt(update.at(2));
	Event_Call("vk_messages_read_in");
}
//Code: 7
void VKW_Tr_ReadOutMessage(json update) {
	Event_Flush();
	Event_PushInt(update.at(1));
	Event_PushInt(update.at(2));
	Event_Call("vk_messages_read_out");
}
//Code: 8
void VKW_Tr_WasOnline(json update) {
	Event_Flush();
	Event_PushInt(update.at(1));
	Event_PushInt(update.at(2));
	Event_PushInt(update.at(3));
	Event_Call("vk_was_online");
}
//Code: 9
void VKW_Tr_WasOffline(json update) {
	Event_Flush();
	Event_PushInt(update.at(1));
	Event_PushInt(update.at(2));
	Event_PushInt(update.at(3));
	Event_Call("vk_was_offline");
}
//Code: 10
void VKW_Tr_FlushDialogFlags(json update) {
	Event_Flush();
	Event_PushInt(update.at(1));
	Event_PushInt(update.at(2));
	Event_Call("vk_dialog_flags_remove");
}
//Code: 11
void VKW_Tr_ReplaceDialogFlags(json update) {
	Event_Flush();
	Event_PushInt(update.at(1));
	Event_PushInt(update.at(2));
	Event_Call("vk_dialog_flags_replace");
}
//Code: 12
void VKW_Tr_SetDialogFlags(json update) {
	Event_Flush();
	Event_PushInt(update.at(1));
	Event_PushInt(update.at(2));
	Event_Call("vk_dialog_flags_set");
}
//Code: 51
void VKW_Tr_ChatEdited(json update) {
	Event_Flush();
	Event_PushInt(update.at(1));
	Event_PushInt(update.at(2));
	Event_Call("vk_chat_edit");
}
//Code: 61
void VKW_Tr_TypingDialog(json update) {
	Event_Flush();
	Event_PushInt(update.at(1));
	Event_PushInt(update.at(2));
	Event_Call("vk_typing");
}
//Code: 62
void VKW_Tr_TypingChat(json update) {
	Event_Flush();
	Event_PushInt(update.at(1));
	Event_PushInt(update.at(2));
	Event_Call("vk_typing_chat");
}
//Code: 80
void VKW_Tr_EditUnreadCount(json update) {
	Event_Flush();
	Event_PushInt(update.at(1));
	Event_PushInt(update.at(2));
	Event_Call("vk_unread_edit");
}
//Code: 114
void VKW_Tr_EditNotifyChat(json update) {
	Event_Flush();
	Event_PushInt(update.at(1));
	Event_PushInt(update.at(2));
	Event_PushInt(update.at(3));
	Event_Call("vk_notify_edit");
}