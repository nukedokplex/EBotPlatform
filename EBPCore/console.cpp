#include "common.h"
#include "ebp_api.h"
#include "console.h"

void console::log(std::string text, std::string type) {
	api_output.writeline("["+ type +"]: "+ text +"\n");
}

void console::error(std::string text, std::string type) {
	api_output.writeline("[" + type + "-ERROR]: " + text + "\n");
}