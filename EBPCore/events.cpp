#include <thread>
#include "events.h"
#include "userlogic.h"
#include "console.h"

map<string, string> events::events;

// Push

void events::caller::pushString(string value)
{
	this->stack.push_back(value);
}

void events::caller::pushInt(int value)
{
	this->stack.push_back(to_string(value));
}

// Get

string events::caller::getString()
{
	string value = this->stack[this->stack_pos];
	this->stack_pos++;
	return value;
}

int events::caller::getInt()
{
	string value = this->stack[this->stack_pos];
	this->stack_pos++;
	return stoi(value);
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