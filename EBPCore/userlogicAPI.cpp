#include "objectscript.h"

#include "userlogic.h"
#include "common.h"
#include "console.h"
#include "cmd.h"
#include "inetwork.h"
#include "filesystem.h"
#include "cvar.h"
#include "vkwork.h"
#include "os-binder.h"
#include <string>
#include "utils.h"

using namespace ObjectScript;

int ULAPI_VK_CreateRequest(OS::String method, bool sendtoken);
void ULAPI_VK_SetParam(int vkrid, OS::String p_name, OS::String p_value);
std::string ULAPI_VK_SendVKR(int vkr);



void ULAPI_Init() {
	//VKWork
	OS::FuncDef funcs[] = {
		def("create", ULAPI_VK_CreateRequest),
		def("setparam", ULAPI_VK_SetParam),
		def("send", ULAPI_VK_SendVKR),
		{}
	};
	ul_script->getModule("vk");
	ul_script->setFuncs(funcs);
	ul_script->pop();

	//Flags
	OS::FuncDef funcs2[] = {
		def("is", Flags_IsFlag),
		def("add", Flags_AddFlag),
		def("remove", Flags_RemoveFlag),
		{}
	};
	ul_script->getModule("flags");
	ul_script->setFuncs(funcs2);
	ul_script->pop();

	//Random
	OS::FuncDef funcs3[] = {
		def("get", Random_Get),
		{}
	};
	ul_script->getModule("random");
	ul_script->setFuncs(funcs3);
	ul_script->pop();

	//Filesystem
	OS::FuncDef funcs4[] = {
		def("root", FS_GetRootPath),
		{}
	};
	ul_script->getModule("fs");
	ul_script->setFuncs(funcs4);
	ul_script->pop();
}

//Ееее, костыли!!
int ULAPI_VK_CreateRequest(OS::String method, bool sendtoken) {
	return VK_CreateRequest(method.str, sendtoken);
}

void ULAPI_VK_SetParam(int vkrid, OS::String p_name, OS::String p_value) {
	VK_SetParam(vkrid, p_name.str, p_value.str);
}

std::string ULAPI_VK_SendVKR(int vkr) {
	return VK_Send(vkr);
}
