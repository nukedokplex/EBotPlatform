#include <winsock.h>
#include "cvar.h"
#include "console.h"
#include "mysql.h"
#include "cmd.h"
#pragma comment(lib, "vs14/mysqlclient.lib")

bool on;
MYSQL_ROW row;
MYSQL connection,*_mysql;
int state;

void mysql::utils::init()
{
	cvar::add("mysql", "0", "Use MySql");
	cvar::add("mysql_server", "tcp://127.0.0.1:3306", "MySql server");
	cvar::add("mysql_user", "root", "MySql user");
	cvar::add("mysql_password", "root", "MySql password");
	cvar::add("mysql_db", "", "MySql database");
	cmd::add("sql", mysql::utils::command, "Execute mysql command");
}

void mysql::utils::start()
{
	on = cvar::get("mysql") == "1";
	if (on)
	{
		mysql_init(&connection);
		console::log("Connecting to: " + cvar::get("mysql_server"), "mysql");
		_mysql = mysql_real_connect(&connection, cvar::get("mysql_server").c_str(),
			cvar::get("mysql_user").c_str(), cvar::get("mysql_password").c_str(),
			cvar::get("mysql_db").c_str(),0,NULL,0);
		if (_mysql == NULL)
		{
			console::error("Connection error: " + (std::string)mysql_error(&connection), "mysql");
			return;
		}
	}
}

std::string mysql::utils::command(std::vector<std::string> cmd_args)
{
	mysql::execute(cmd_args[1]);
	const char *info = mysql_info(_mysql);
	if (info == NULL)
		info = "Null";
	return "MYSQL> "+(std::string)info;
}

// OR
MYSQL_RES *mysql::execute(std::string sql)
{
	state = mysql_query(_mysql, sql.c_str());
	if (state != 0) {
		console::error("Execute error: " + (std::string)mysql_error(&connection), "mysql");
		return 0;
	}
	return mysql_store_result(&connection);
}