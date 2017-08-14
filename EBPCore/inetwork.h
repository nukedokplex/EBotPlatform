#pragma once
#include "common.h"
#include "console.h"

namespace net
{
	void init();
	string c_nettest(vector<string> cmd_args);
	string urlDecode(string str);
	size_t writeToBuffer(char *ptr, size_t size, size_t nmemb, void *data);
	size_t writeToFile(char *ptr, size_t size, size_t nmemb, void *data);
	string send(std::string url, const char *post = NULL, string file = "");
	string get(string url);
	string post(string url, const char *data);
	string post(string url, map<string, string> params);
	void init();

	struct buffer {
		string data;
		size_t size;
	};

	class request
	{
		string url;
		map<string, string> params;
		char *data;
	public:
		// Variables
		bool isEnd; // is request has sended
		int size; // data size
		// Voids
		request(string url)
		{
			this->url = url;
			data = NULL;
		}
		void set(string param, string value);
		void setData(string data);
		void setFile(string filepath);
		string send();
		void download(string path);
		string send_off();
		void download_off(string path);
	};
};