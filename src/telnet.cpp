#include "telnet.h"

Telnet::Telnet()
{
	clear();

	print(0, "MUDHACK");
	print(0, 149, "X");
}

void Telnet::clear()
{
	for (int i = 0; i < height; i++)
	{
		buffer[i] = "";
		for (int j = 0; j < width; j++)
		{
			buffer[i] += " ";
		}
	}
}

std::string Telnet::get_buffer()
{
	// Create output string that clears terminal
	std::string output = "\u001B[2J";

	// Add buffer to output
	for (int i = 0; i < height; i++)
	{
		output += buffer[i] + "\r\n" ;
	}
	output += cursor;

	return output;
}

void Telnet::print(int line, std::string text)
{
	print(line, 0, text);
}

void Telnet::print(int line, int offset, std::string text)
{
	for (int i = offset; i < text.length() + offset; i++)
	{
		buffer[line][i] = text[i - offset];
	}
}