#ifndef TELNET_H
#define TELNET_H

#include <string>
#include "server.h"

class Telnet
{
public:
	Telnet();

	std::string cursor = "> ";

	void clear();
	std::string get_buffer();
	void print(int line, std::string text);
	void print(int line, int offset, std::string text);

private:
	static const int width = 150;
	static const int height = 40;
	std::string buffer[height];
};

#endif