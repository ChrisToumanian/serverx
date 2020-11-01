#ifndef CHAT_H
#define CHAT_H

#include "channel.h"

class Chat : public Channel
{
public:
	Chat();

	void user_connected(User* user);
	void user_disconnected(User* user);
	void user_command(User* user, std::vector<std::string> commands);
};

#endif
