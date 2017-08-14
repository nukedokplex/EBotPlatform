CC=g++
CFLAGS=-c -std=c++11 -Wall
LDFLAGS=-llua5.3 -lcurl -lmysqlclient -lpthread
INCLUDES= -I $(PWD)/LuaBridge/Source/LuaBridge
SOURCES= EBPCore/cmd.cpp EBPCore/console.cpp EBPCore/cvar.cpp EBPCore/eapi.cpp EBPCore/events.cpp EBPCore/filesystem.cpp EBPCore/host.cpp EBPCore/inetwork.cpp EBPCore/longpoll.cpp EBPCore/mysql.cpp EBPCore/other.cpp EBPCore/string_utils.cpp EBPCore/userlogic.cpp EBPCore/userlogic_api.cpp EBPCore/utils.cpp EBPCore/vkwork.cpp EBPCore/sleep.cpp EBPCore/charsets.cpp
OBJECTS=$(SOURCES:.cpp=.o) 
EXECUTABLE=EPBPlatform

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@ 

.cpp.o: 	
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)