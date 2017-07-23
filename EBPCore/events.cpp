#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <vector>
#include "events.h"
#include "userlogic.h"

std::map<std::string, std::string> events;
std::map<std::string, bool> lock;

std::vector<void*> stack;
int it = 0;

// Push

void Event_PushString(std::string value)
{
	stack.push_back((void*)value.c_str());
}

void Event_PushInt(int value)
{
	stack.push_back((void*)value);
}

void Event_PushFloat(float value)
{
	stack.push_back((void*)&value);
}

// Get

std::string Event_GetString()
{
	void *value = stack[it];
	it++;
	return (std::string)(char*)value;
}

int Event_GetInt()
{
	void *value = stack[it];
	it++;
	return (int)value;
}

float Event_GetFloat()
{
	void *value = stack[it];
	it++;
	return *(float *)&value;// WTF??
}

// Work

void Event_Flush()
{
	stack = std::vector<void*>();
	it = 0;
}

void Event_Register (std::string name, std::string func)
{
	events[name] = func;
	lock[name] = false;
}

void Event_Call (std::string name)
{
	if (events.find(name) == events.end())
		return;
	while (lock[name]) {}
	lock[name] = true;
	UL_Call(events.find(name)->second);
	lock[name] = false;
}