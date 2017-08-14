#pragma once
#include "common.h"

namespace vk
{
	class request {
	private:
		map<string, string> params;
	public:
		request(string method, bool send_token = true)
		{
			this->method = method;
			this->sendtoken = send_token;
		}
		string method;
		bool sendtoken;
		void set(string, string);
		string send();
	};

	void init();
	string send(string, map<string, string>, bool);
	string getToken();
}