#include "common.h"
#include "console.h"
#include "cmd.h"
#include "inetwork.h"
#include "curl/curl.h"
#include "cvar.h"

#pragma comment(lib,"libcurl_imp.lib") 

struct MemoryStruct {
	char *memory;
	size_t size;
};


void *myrealloc(void *ptr, size_t size)
{
	/* There might be a realloc() out there that doesn't like reallocing
	NULL pointers, so we take care of it here */
	if (ptr)
		return realloc(ptr, size);
	else
		return malloc(size);
}
size_t
WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)data;
	mem->memory = (char *)myrealloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory) {
		memcpy(&(mem->memory[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
	}
	return realsize;
}


/*
	Получить ответ сервера
*/
std::string Nettest_Command(std::vector<std::string> cmd_args)
{
	if (cmd_args.size() != 2) {
		return "Use \"nettest <url>\"";
	}
	void * ret = Net_Get(cmd_args[1]);
	if (ret == NULL) {
		return "NETTEST UNKNOW ERROR";
	}
	return (char *)ret;
}

void Net_Init() {
	Console_Log("Initialization Network...", "Core:Net_Init");
	Cmd_AddCommand("nettest", Nettest_Command, "Get server response");
	Cvar_AddCvar("net_agent", "ebotplatrorm-agent/5.0", "User Agent on Net");
}

void *Net_Get(std::string url) {
	CURL *curl;
	CURLcode res;
	struct MemoryStruct chunk;
	chunk.memory = NULL;
	chunk.size = 0;
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERAGENT, Cvar_GetValue("net_agent").c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			Console_Log("curl_easy_perform() failed: " + (std::string)curl_easy_strerror(res), "Core:Net_Get");
		curl_easy_cleanup(curl);
		return chunk.memory;
	}
	return NULL;
}

void *Net_Post(std::string url, std::map<std::string, std::string> params) {
	CURL *curl;
	CURLcode res;
	struct MemoryStruct chunk;
	chunk.memory = NULL;
	chunk.size = 0;
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERAGENT, Cvar_GetValue("net_agent").c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

		std::string paramline = "";
		std::map<std::string, std::string>::const_iterator iter;
		for (iter = params.begin(); iter != params.end(); iter++) {
			paramline+=iter->first+"="+curl_easy_escape(curl, iter->second.c_str(), iter->second.size())+"&";
		}
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, paramline.c_str());

		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			Console_Log("curl_easy_perform() failed: " + (std::string)curl_easy_strerror(res), "Core:Net_Post");
		curl_easy_cleanup(curl);
		return chunk.memory;
	}
	return NULL;
}