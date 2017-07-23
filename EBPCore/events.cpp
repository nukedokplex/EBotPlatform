#include <map>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <vector>
#include "events.h"
#include "userlogic.h"

std::map<std::string, std::string> events;
std::map<std::string, bool> lock;

std::map<int, std::vector<void*>> stacks;
std::map<int, int> stack_it;

// Push

void Event_PushString(int id, std::string value)
{
	stacks[id].push_back((void*)value.c_str());
}

void Event_PushInt(int id,int value)
{
	stacks[id].push_back((void*)value);
}

void Event_PushFloat(int id,float value)
{
	stacks[id].push_back((void*)&value);
}

// Get

std::string Event_GetString(int id)
{
	void *value = stacks[id][stack_it[id]];
	stack_it[id]++;
	return (std::string)(char*)value;
}

int Event_GetInt(int id)
{
	void *value = stacks[id][stack_it[id]];
	stack_it[id]++;
	return (int)value;
}

float Event_GetFloat(int id)
{
	void *value = stacks[id][stack_it[id]];
	stack_it[id]++;
	return *(float *)&value;// WTF??
}

// Work

void Event_Flush(int id)
{
	if (stacks.size()==0 || stacks.find(id) == stacks.end())
		return;// WTF?
	stacks.erase(id);
	stack_it.erase(id);
}

int Event_New()
{
	int i = 0;
	while (stacks.find(i) != stacks.end())
		i++;
	stacks[i] = std::vector<void*>();
	stack_it[i] = 0;
	return i;
}

void Event_Register (std::string name, std::string func)
{
	events[name] = func;
	lock[name] = false;
}

void UL_CallEvent(std::string method, int sid);
void Event_Call (std::string name, int stack_id)
{
	if (events.find(name) != events.end())
	{
		//while (lock[name]) { Sleep(100); }
		//lock[name] = true;
		UL_CallEvent(events.find(name)->second, stack_id);
		//lock[name] = false;
	}
	Event_Flush(stack_id);
}

bool Event_Exists(std::string name)
{
	return events.find(name) != events.end();
}