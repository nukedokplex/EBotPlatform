#pragma once
#include "common.h"

namespace events
{
	class caller
	{
	private:
		string name;
		vector<void*> stack;
		int stack_pos;
	public:
		caller(string name)
		{
			this->name = name;
			this->stack = vector<void*>();
			this->stack_pos = 0;
		}
		// Push
		void pushString(string value);
		void pushInt(int value);
		// Get
		string getString();
		int getInt();
		// Work
		void call();
	};
	// Variables
	extern map<string, string> events;
	// Voids
	void registerEvent(string name, string func);
	events::caller *create(string name);
	bool exists(string name);
};