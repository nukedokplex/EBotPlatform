#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "events.h"

std::map<std::string, void(*)(va_list)> events;

void Event_Register (std::string name, void(*func)(va_list))
{
	events[name] = func;
}

void Event_Call (std::string name, ...)
{
	if (events.find(name) == events.end())
		return;
	va_list ap;
	va_start(ap, name);
	events.find(name)->second(ap);
	va_end(ap);
}