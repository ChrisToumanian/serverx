#ifndef CHANNEL_H
#define CHANNEL_H

#include <iostream>
#include <string>
#include "server.h"
#include "user.h"
#include "reader.h"

class Channel
{
public:
    Channel();

	std::string name = "channel";
	std::vector<User*> users;
	int current_id = 0;
	Server* server;
    
	virtual void load();
	virtual void update();
	void receive(int user_id, std::string message);

	virtual void user_connected(User* user);
	virtual void user_disconnected(User* user);
	virtual void user_message(User* user, std::string message, std::vector<std::string> commands);
	virtual void send_to_all(std::string message);
	virtual void send_to_user(User* user, std::string message);

	User* get_user(int user_id);
	User* get_user(std::string username);
	int get_users_online();
	User* add_user(int user_id);
	void remove_user(User* user);

private:
	int get_new_id();
};

#endif
