#pragma once
#include "common.h"
#include "console.h"

namespace net
{
	void init();
	string c_nettest(vector<string> cmd_args);
	string urlDecode(string str);
	void *dataRealloc(void *ptr, size_t size);
	size_t writeToBuffer(void *ptr, size_t size, size_t nmemb, void *data);
	void *send(std::string url, const char *post = NULL);
	void *get(string url);
	void *post(string url, const char *data);
	void *post(string url, map<string, string> params);
	void init();

	struct buffer {
		char *data;
		size_t size;
	};

	class request
	{
		string url;
		map<string, string> params;
		char *data;
	public:
		request(string url)
		{
			this->url = url;
			data = NULL;
		}
		void set(string param, string value);
		void setData(string data);
		void setFile(string filepath);
		string send();
	};
};