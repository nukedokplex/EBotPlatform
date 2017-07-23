#include "common.h"
#include "ebp_api.h"
#include "console.h"

void Console_Log(std::string text, std::string type) {
	api_output.writeline("["+ type +"]: "+ text +"\n");
}

void Console_Error(std::string text, std::string type) {
	api_output.writeline("[" + type + "-ERROR]: " + text + "\n");
}