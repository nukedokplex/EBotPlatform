#pragma once

// Push
void Event_PushString(int id, std::string value);
void Event_PushInt(int id, int value);
void Event_PushFloat(int id, float value);
// Get
std::string Event_GetString(int id);
int Event_GetInt(int id);
float Event_GetFloat(int id);
// Work
void Event_Flush(int id);
int Event_New();
void Event_Register(std::string name, std::string func);
void Event_Call(std::string name, int stack_id);
bool Event_Exists(std::string name);