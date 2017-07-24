#pragma once
#include "json.hpp"

using json = nlohmann::json;

namespace longpoll
{
	namespace update
	{
		void replaceMessageFlags(json update);
		void setMessageFlags(json update);
		void flushMessageFlags(json update);
		void newMessage(json update);
		void readInMessage(json update);
		void readOutMessage(json update);
		void wasOnline(json update);
		void wasOffline(json update);
		void flushDialogFlags(json update);
		void replaceDialogFlags(json update);
		void setDialogFlags(json update);
		void chatEdited(json update);
		void typingDialog(json update);
		void typingChat(json update);
		void editUnreadCount(json update);
		void editNotifyChat(json update);
	}
	void start();
	void getServer();
	void loop();
	void getUpdate(json *update);
}
