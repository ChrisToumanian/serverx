make:
	g++ -o bin/server src/main.cpp src/server.cpp src/reader.cpp src/channel.cpp src/user.cpp src/channels/chat.cpp src/telnet.cpp -pthread
