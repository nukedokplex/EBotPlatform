#include "common.h"
#include "ebp_api.h"
#include "console.h"
#include "cmd.h"
#include "cvar.h"
#include "filesystem.h"
#include "inetwork.h"
#include "vkwork.h"
#include "userlogic.h"
#include "longpoll.h"
#include "mysql.h"
#include "utils.h"
#include "other.h"
#include "host.h"
#include "eapi.h"

inputApi api_input = {
	console::log,
	cmd::exe,
	Host_Start
};
outputApi api_output;

inputApi EXPORT Host_Main(const std::string botname, outputApi api)
{
	api_output = api;
	console::init(botname);
	console::log("Initializing EBotPlatform "+other::getVersionName(), "Core");
	// Init systems
	fs::init(botname);
	cmd::init();
	cvar::init();
	net::init();
	eapi::init();
	vk::init();
	utils::init();
	userlogic::init();
	mysql::utils::init();
	atexit(Host_Shutdown);
	return api_input;
}

int Host_Start()
{
	cmd::exec("config.cfg");
	eapi::start();
	mysql::utils::start();
	userlogic::start();
	longpoll::start();
	return 0;
}

void Host_End()
{
	system("PAUSE");
	exit(0);
}

/*
=================
Host_Shutdown
=================
*/
void EXPORT Host_Shutdown(void)
{
}