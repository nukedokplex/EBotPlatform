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

struct PostRequest {
	std::string url;
	std::map<std::string, std::string> params;
};

std::map<int, PostRequest> prs;

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
std::string Utf8_to_cp1251(const char *str);
std::string Nettest_Command(std::vector<std::string> cmd_args)
{
	if (cmd_args.size() != 2) {
		return "Use \"nettest <url>\"";
	}
	void * ret = Net_Get(cmd_args[1]);
	if (ret == NULL) {
		return "NETTEST UNKNOW ERROR";
	}
	return Utf8_to_cp1251((char *)ret);
}

void Net_Init() {
	console::log("Initialization Network...", "Core:Net_Init");
	cmd::add("nettest", Nettest_Command, "Get server response");
	cvar::add("net_agent", "ebotplatrorm-agent/5.0", "User Agent on Net");
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
		curl_easy_setopt(curl, CURLOPT_USERAGENT, cvar::get("net_agent").c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			console::log("curl_easy_perform() failed: " + (std::string)curl_easy_strerror(res), "Core:Net_Get");
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
		curl_easy_setopt(curl, CURLOPT_USERAGENT, cvar::get("net_agent").c_str());
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
			console::log("curl_easy_perform() failed: " + (std::string)curl_easy_strerror(res), "Core:Net_Post");
		curl_easy_cleanup(curl);
		return chunk.memory;
	}
	return NULL;
}


// Helper Lua
int Net_RegisterPR(PostRequest pr) {
	int i = 0;
	while (prs.find(i) != prs.end())
		i++;
	prs[i] = pr;
	return i;
}

int Net_CreatePost(std::string url)
{
	PostRequest pr = PostRequest();
	pr.url = url;
	return Net_RegisterPR(pr);
}

std::string cp1251_to_Utf8(const char *str);
void Net_SetParam(int prid, std::string p_name, std::string p_value) {
	prs[prid].params[p_name] = cp1251_to_Utf8(p_value.c_str());
}

std::string Net_Send(int prid) {
	PostRequest pr = prs[prid];
	prs.erase(prid);
	return Utf8_to_cp1251((char *)Net_Post(pr.url, pr.params));
}