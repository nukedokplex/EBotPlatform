#include <thread>
#include "events.h"
#include "userlogic.h"

map<string, string> events::events;

// Push

void events::caller::pushString(string value)
{
	this->stack.push_back((void*)value.c_str());
}

void events::caller::pushInt(int value)
{
	this->stack.push_back((void*)value);
}

void events::caller::pushFloat(float value)
{
	this->stack.push_back((void*)&value);
}

// Get

std::string events::caller::getString()
{
	void *value = this->stack[this->stack_pos];
	this->stack_pos++;
	return (std::string)(char*)value;
}

int events::caller::getInt()
{
	void *value = this->stack[this->stack_pos];
	this->stack_pos++;
	return (int)value;
}

float events::caller::getFloat()
{
	void *value = this->stack[this->stack_pos];
	this->stack_pos++;
	return *(float *)&value;// WTF??
}

// Work

void events::caller::call ()
{
	std::thread loopthread(userlogic::callEvent, events::events[this->name], this);
	loopthread.detach();
}

void events::registerEvent(string name, string func)
{
	events::events[name] = func;
}

events::caller *events::create(string ev_name)
{
	if (!events::exists(ev_name))
		return NULL;
	return new events::caller(ev_name);
}

bool events::exists(string name)
{
	return events.find(name) != events.end();
}