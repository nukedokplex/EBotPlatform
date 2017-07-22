#pragma once

extern void Event_Register(std::string name, void(*)(va_list));
extern void Event_Call(std::string name, ...);