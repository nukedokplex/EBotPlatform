CC=g++
CFLAGS=-c -std=c++11 -Wall
LDFLAGS=
INCLUDES= -I $(PWD)/LuaBridge/Source/LuaBridge -I $(PWD)/iconv-lite
SOURCES= EBPCore/cmd.cpp EBPCore/console.cpp EBPCore/cvar.cpp EBPCore/eapi.cpp EBPCore/events.cpp EBPCore/filesystem.cpp EBPCore/host.cpp EBPCore/inetwork.cpp EBPCore/longpoll.cpp EBPCore/mysql.cpp EBPCore/other.cpp EBPCore/string_utils.cpp EBPCore/userlogic.cpp EBPCore/userlogic_api.cpp EBPCore/utils.cpp EBPCore/vkwork.cpp EBPCore/sleep.cpp EBPCore/charsets.cpp iconv-lite/iconvlite.cpp
OBJECTS=$(SOURCES:.cpp=.o) 
EXECUTABLE=EPBPlatform

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ 

.cpp.o: 	
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
