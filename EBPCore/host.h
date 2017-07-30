#pragma once
#include <string> // std::string
#include "ebp_api.h" // inputApi

#define EXPORT	 __declspec( dllexport ) 

extern "C" {
	inputApi EXPORT Host_Main(const std::string botdir, outputApi api);
	void EXPORT Host_Shutdown(void);
}