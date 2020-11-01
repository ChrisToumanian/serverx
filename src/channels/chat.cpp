#include "chat.h"

Chat::Chat()
{
}

void Chat::user_connected(User* user)
{
	server->log(user->username, "connected.");
	server->broadcast(user->username + " joined.");
}

void Chat::user_disconnected(User* user)
{
	server->log(user->username, "disconnected.");
	server->broadcast(user->username + " left.");
}

void Chat::user_command(User* user, std::vector<std::string> commands)
{
	if (commands[0][0] != '/') // Say
	{
		server->broadcast("[" + user->username + "] " + Reader::join(commands));
		server->log(user->username, Reader::join(commands));
	}
	else if (commands[0] == "/uid" && commands.size() == 2) // Sign in player by UID
	{
		// Check players.csv
		std::vector<std::string> lines = Reader::get_file_lines("players.csv");
		for (std::string line : lines)
		{
			std::vector<std::string> data = Reader::split(line, ",");
			if (data[0] == commands[1]) // UID matches player
			{
				user->uid = data[0];
				user->username = data[1];
			}
		}
	}
	else if (commands[0] == "/name" && commands.size() == 2) // Username change
	{
		server->broadcast("[" + user->username + "] changed name to " + commands[1]);
		user->username = commands[1];
	}
	else if (commands[0] == "/me" && commands.size() > 1) // Emote
	{
		server->broadcast(user->username + " " + Reader::join(commands, 1));
		server->log(user->username, Reader::join(commands, 1));
	}
	else if (commands[0] == "/yell") // Yell
	{
		server->broadcast(user->username + " yells " + Reader::join(commands, 1));
		server->log(user->username, "yells " + Reader::join(commands, 1));
	}
	else if (commands[0] == "/whisper" && commands.size() > 2) // Whisper
	{
		send_to_user(get_user(commands[1]), user->username + " whispers " + Reader::join(commands, 2));
	}
}
