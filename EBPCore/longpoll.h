#pragma once
#include "json.hpp"

using json = nlohmann::json;

namespace longpoll
{
	struct update
	{
		std::string name;
		bool dopValue;
	};

	void start();
	void startLoop();
	void getServer();
	void loop();
	void getUpdate(json *update);
}
