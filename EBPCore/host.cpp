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

inputApi api_input = {
	console::log,
	cmd::exe
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
	vk::init();
	utils::init();
	userlogic::init();
	mysql::utils::init();

	cmd::exec("config.cfg");

	mysql::utils::start();
	userlogic::start();
	longpoll::start();


	atexit(Host_Shutdown);
	return api_input;
}

/*
=================
Host_Shutdown
=================
*/
void EXPORT Host_Shutdown(void)
{
}