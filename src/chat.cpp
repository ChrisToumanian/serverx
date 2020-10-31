#include "chat.h"

Chat::Chat()
{
}

void Chat::user_connected(User* user)
{
	server->log(user->username, "connected.");
	server->broadcast(user->username + "joined.");
}

void Chat::user_disconnected(User* user)
{
	server->log(user->username, "disconnected.");
	server->broadcast(user->username + " left.");
}

void Chat::user_message(User* user, std::string message, std::vector<std::string> commands)
{
	if (commands[0][0] != '/')
	{
		user->receive(message);
		server->broadcast("[" + user->username + "] " + message);
		server->log(user->username, message);
	}
}

void Chat::send_to_all(std::string message)
{
}

void Chat::send_to_user(User* user, std::string message)
{
}
