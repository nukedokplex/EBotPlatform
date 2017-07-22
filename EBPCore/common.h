#pragma once

#include <cstdio>
#include <iostream>
#include "ebp_api.h"
#include <vector>

#define EXPORT	 __declspec( dllexport ) 
#define VERSION 0.5f

extern "C" {
	inputApi EXPORT Host_Main(const std::string botdir, outputApi api);
	void EXPORT Host_Shutdown(void);
}

extern std::string Title_Command(std::vector<std::string> cmd_args);
extern std::string GetVersionName();

