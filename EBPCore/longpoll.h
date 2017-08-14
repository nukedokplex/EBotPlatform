#pragma once
#include "json.hpp"

using json = nlohmann::json;

namespace longpoll
{
	struct update
	{
		std::string name;
		vector<char> values;
	};
	extern map<int, update> longpollUpdates;
	extern string server;
	extern string key;
	extern int ts;

	void start();
	void startLoop();
	void getServer();
	void loop();
	void getUpdate(json *update);
}
