#include "curl/curl.h" // Use curl
#include "common.h"
#include "console.h" // console::error
#include "cmd.h" // cmd::add
#include "cvar.h" // cvar::add
#include "filesystem.h"
#include "inetwork.h" // this
#include "other.h" // other::getVersionName()

#pragma comment(lib,"libcurl_imp.lib") 

void net::init()
{
	cmd::add("nettest", net::c_nettest, "Get server response");
	cvar::add("net_agent", "ebotplatrorm-agent/" + other::getVersionName(), "User Agent on Net");
}

string net::c_nettest(std::vector<std::string> cmd_args)
{
	if (cmd_args.size() != 2)
		return "Use \"nettest <url>\"";
	string ret = net::get(cmd_args[1]);
	return ret;
}

string net::urlDecode(string str)
{
	std::string result;
	char* esc_text = curl_easy_escape(NULL, str.c_str(), str.length());
	if (!esc_text) throw std::runtime_error("Can not convert string to URL");

	result = esc_text;
	curl_free(esc_text);

	return result;
}

size_t net::writeToBuffer(char *ptr, size_t size, size_t nmemb, void *data)
{
	size_t realsize = size * nmemb;
	net::buffer *buff = (net::buffer *)data;
	buff->data.append(ptr);
	return realsize;
}

size_t net::writeToFile(char *ptr, size_t size, size_t nmemb, void *data)
{
	size_t realsize = size * nmemb;
	fs::file *file = (fs::file *)data;
	file->writeBinary(ptr, nmemb);
	return realsize;
}

string net::send(std::string url, const char *post, string file_path)
{
	CURL *curl;
	fs::file *file = NULL;
	net::buffer *buff = NULL;

	bool toFile = file_path != "";
	if (toFile)
		file = fs::openOrCreate(file_path, true, 12);
	else
		buff = new net::buffer();

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERAGENT, cvar::get("net_agent").c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		if (toFile) {
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)file);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, net::writeToFile);
		}
		else {
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)buff);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, net::writeToBuffer);
		}
	
		if (post != NULL) {
			curl_easy_setopt(curl, CURLOPT_POST, 1);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
		}
		CURLcode code = curl_easy_perform(curl);
		if (code != CURLE_OK) {
			console::error("Can't connect to " + url, "Curl");
			console::error(curl_easy_strerror(code), "Curl");
		}
		curl_easy_cleanup(curl);
		if (toFile)
		{
			file->close();
			return "";
		}
		return buff->data;
	}
	return "";
}

string net::get(std::string url)
{
	return net::send(url, NULL);
}

string net::post(std::string url, const char *data)
{
	return net::send(url, data);
}

string net::post(string url, map<string, string> params)
{
	// Build paramline
	std::string paramline = "";
	std::map<std::string, std::string>::const_iterator iter;
	for (iter = params.begin(); iter != params.end(); iter++) {
		paramline += iter->first + "=" + urlDecode(iter->second) + "&";
	}
	// Send
	return net::send(url, paramline.c_str());
}

void net::request::set(std::string p_name, std::string p_value) 
{
	this->params[p_name] = p_value.c_str();
}

void net::request::setData(string data)
{
	this->data = (char *)data.c_str();
}

void net::request::setFile(string filepath)
{
	string data;
	fs::file *in = fs::open(filepath);
	data = in->read();
	in->close();
	this->data = (char *)data.c_str();
}

string net::request::send() 
{
	string data;
	if (this->data == NULL)
		data = net::post(this->url, this->params);
	else
		data = net::post(this->url, this->data);
	return data;
}

void net::request::download(string path)
{
	net::send(this->url, char(), path);
}