#pragma once
#include "common.h"
#include "ebp_api.h"

namespace console
{
	void log(std::string text, std::string type);
	void error(std::string text, std::string type);
}