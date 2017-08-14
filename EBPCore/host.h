#pragma once
#include <string> // std::string
#include "ebp_api.h" // inputApi

#ifdef __linux__ 
	#define EXPORT
#elif _WIN32
	#define EXPORT	 __declspec( dllexport ) 
#endif

extern "C" {
	inputApi EXPORT Host_Main(const std::string botdir, outputApi api);
	int Host_Start();
	void EXPORT Host_Shutdown(void);
}