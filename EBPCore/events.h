#pragma once
#include <LuaBridge.h>

// Push
void Event_PushString(std::string value);
void Event_PushInt(int value);
void Event_PushFloat(float value);
// Get
std::string Event_GetString();
int Event_GetInt();
float Event_GetFloat();
// Work
void Event_Flush();
void Event_Register(std::string name, std::string func);
void Event_Call(std::string name);