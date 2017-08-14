#pragma once
#include "common.h"
#include <fstream>

namespace fs
{
	class file
	{
	public:
		fstream _stream;
		// Voids
		void write(string data);
		void writeBinary(const char *data, size_t size);
		string read();
		void readBinary(char *data, size_t size);
		void close();
	};
	void init(std::string botdir);
	bool exists(std::string path, bool inBotFolder = true);
	file *open(std::string path, bool inBotFolder = true, int flags = 2);
	file *openOrCreate(std::string path, bool inBotFolder = true, int flags = 2);
	file *create(std::string path, bool inBotFolder = true, int flags = 2);
	std::string getRootPath();
	void writeData(string path, string data, bool inBotFolder = true);
	void safeWriteData(string path, string data, bool inBotFolder = true);
	string readData(string path, bool inBotFolder = true);
}