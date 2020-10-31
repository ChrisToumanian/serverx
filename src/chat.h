#ifndef CHAT_H
#define CHAT_H

#include "channel.h"

class Chat : public Channel
{
public:
	Chat();

	void user_connected(User* user);
	void user_disconnected(User* user);
	void user_message(User* user, std::string message, std::vector<std::string> commands);
	void send_to_all(std::string message);
	void send_to_user(User* user, std::string message);
};

#endif
