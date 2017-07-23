#pragma once
#include <string>

int Random_Get(int min, int max);

bool Flags_IsFlag(int flags, int flag);
int Flags_AddFlag(int flags, int flag);
int Flags_RemoveFlag(int flags, int flag);

std::string Charset_Change(std::string str, int from, int to);