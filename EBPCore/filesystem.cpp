#include "common.h"
#include "console.h"
#include "cvar.h"
#include <map>
#include <vector>
#include "filesystem.h"

void fs::init(std::string botdir)
{
	cvar::add("bot_dir", botdir, "Bot directory");
}

std::string fs::getRootPath()
{
	return cvar::get("bot_dir") + "/";
}

bool fs::exists(std::string path, bool inBotFolder)
{
	std::ifstream fin;
	if (inBotFolder)
		fin.open(cvar::get("bot_dir") + "/" + path);
	else
		fin.open(path);
	if (fin.is_open()) {
		fin.close();
		return true;
	} else
	return false;
}

/* --------- FILE CLASS ---------*/

/*
	Open new file

	@flags:
	2 - Read
	4 - Write
	8 - Binary
*/
fs::file *fs::open(std::string path, bool inBotFolder, int flags)
{
	if (!fs::exists(path, inBotFolder)) {
		console::error(path + " not found", "Filesystem");
		return NULL;
	}
	if (inBotFolder)
		path = cvar::get("bot_dir") + "/" + path;

	file *_file = new file();
	int _flags = 
		(flags & 2 ? ios::in : 0) | // 2 - Read
		(flags & 4 ? ios::out : 0) | // 4 - Write
		(flags & 8 ? ios::binary : 0);  // 8 - Binary
	_file->_stream.open(path, _flags);
	return _file;
}

/*
Open or create new file

@flags:
2 - Read
4 - Write
8 - Binary
*/
fs::file *fs::openOrCreate(std::string path, bool inBotFolder, int flags)
{
	if (!fs::exists(path, inBotFolder)) {
		fs::writeData(path, "", inBotFolder);
	}
	return fs::open(path, inBotFolder, flags);
}

/*
Create new file

@flags:
2 - Read
4 - Write
8 - Binary
*/
fs::file *fs::create(std::string path, bool inBotFolder, int flags)
{
	string newfname = inBotFolder ? cvar::get("bot_dir") + "/" + path : path;
	ofstream fp(newfname);
	fp.close();
	return fs::open(path, inBotFolder, flags);
}

/*
	Close and delete file
*/
void fs::file::close()
{
	this->_stream.close();
	delete this;
}

/*
	Read all file
*/
string fs::file::read()
{
	string data;
	string line;
	while (std::getline(this->_stream, line)) {
		data += line;
	}
	return data;
}

/*
	Read all binary data
*/
void fs::file::readBinary(char *data, size_t size)
{
	this->_stream.read(data, size);
}

/*
	Write text to file
*/
void fs::file::write(string data)
{
	this->_stream << data;
}

/*
Write binary data to file
*/
void fs::file::writeBinary(const char *data, size_t size)
{
	//this->_stream << data;
	this->_stream.write(data, size);
}

/* --------- /FILE CLASS/ ---------*/

void fs::writeData(string path, string data, bool inBotFolder)
{
	if (fs::file *file = fs::create(path, inBotFolder, 4)) {
		file->write(data);
		file->close();
	}
}

void fs::safeWriteData(string path, string data, bool inBotFolder)
{
	// TODO
}

string fs::readData(string path, bool inBotFolder)
{
	string data;
	fs::file *in = fs::open(path, inBotFolder);
	data = in->read();
	in->close();
	return data;
}