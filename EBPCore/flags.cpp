bool Flags_IsFlag(int flags, int flag) {
	return flags & flag;
}

int Flags_AddFlag(int flags, int flag) {
	return flags + flag;
}

int Flags_RemoveFlag(int flags, int flag) {
	return flags - flag;
}