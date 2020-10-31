#include "channel.h"

Channel::Channel()
{
}

void Channel::load()
{
}

void Channel::update()
{
}

void Channel::receive(int user_id, std::string message)
{
    User* user = get_user(user_id);
    std::vector<std::string> commands = Reader::split(message, " ");

	if (user != NULL && commands[0] == "/quit")
	{
		user_disconnected(user);
		server->disconnect(user->id);
		remove_user(user);
	}
	else if (user != NULL)
	{
		user_message(user, message, commands);
	}
	else if (commands[0] == "/connected")
	{
		User* user = add_user(user_id);
		user_connected(user);
	}
	else if (commands[0] == "/disconnected")
	{
		user_disconnected(user);
		server->disconnect(user->id);
		remove_user(user);
	}
}

void Channel::user_connected(User* user)
{
	server->log(user->username, "connected.");
	server->broadcast(user->username + " joined.");
}

void Channel::user_disconnected(User* user)
{
	server->log(user->username, "disconnected.");
	server->broadcast(user->username + " left.");
}

void Channel::user_message(User* user, std::string message, std::vector<std::string> commands)
{
	if (commands[0][0] != '/')
	{
		user->receive(message);
		server->broadcast("[" + user->username + "] " + message);
		server->log(user->username, message);
	}
}

void Channel::send_to_all(std::string message)
{
}

void Channel::send_to_user(User* user, std::string message)
{
}

User* Channel::add_user(int user_id)
{
	User* user = new User();
	user->id = user_id;
	user->username = "User#" + std::to_string(user_id);
	users.push_back(user);

	return user;
}

void Channel::remove_user(User* user)
{
    int i = 0;
    for (auto& u : users)
    {
        if (u->id == user->id)
        {
            users.erase(users.begin() + i);
        }
        i++;
    }
}

int Channel::get_new_id()
{
	int id = current_id;
	current_id++;
	return id;
}

User* Channel::get_user(int user_id)
{
    for (auto& user : users)
    {
        if (user->id == user_id)
            return user;
    }
    return NULL;
}

User* Channel::get_user(std::string username)
{
    for (auto& user : users)
    {
        if (user->username == username)
            return user;
    }
    return NULL;
}

int Channel::get_users_online()
{
    if (users.size() > 0)
        return users.size();
    return 0;
}
