#pragma once
#include <_mysql.h>

namespace mysql
{
	MYSQL_RES *execute(std::string sql);
	namespace utils
	{
		void init();
		void start();
		std::string command(std::vector<std::string> cmd_args);
	}
}