#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "server.h"
#include "channel.h"
#include "reader.h"
#include "chat.h"

int port = 25566;
int max_clients = 100;
std::string server_name = "server";
std::string config_filename = "server.yml";
std::vector<Channel*> channels;
static Server* server;

void read_config();
void start();
void load();
void stop();
std::string receive(Client* client, std::string message);
std::string command(Client* client, std::string message);

int main(int argc, char *argv[])
{
    read_config();
	load();
	start();
}

void read_config()
{
    std::vector<std::string> settings = Reader::get_file_lines(config_filename);
    for (int i = 0; i < settings.size(); i++)
    {
        std::vector<std::string> setting = Reader::split(settings[i], ": ");
        if (setting[0] == "server-name")
            server_name = setting[1];
        else if (setting[0] == "port")
            port = std::stoi(setting[1]);
        else if (setting[0] == "max-clients")
			max_clients = std::stoi(setting[1]);
    }
}

void load()
{
	server = new Server(port, receive);

	// Load channels
	Chat* channel = new Chat();
	channel->server = server;
	channel->load();
	channels.push_back(channel);
	//Channel* channel = new Channel();
	//channel->server = server;
	//channel->load();
	//channels.push_back(channel);
}

void start()
{
	std::cout << "Initializing server..." << std::endl;
	server->start();
}

void stop()
{
	server->stop();
}

std::string receive(Client* client, std::string message)
{
    Reader::rtrim(message);

	if (message.length() > 0) // client message
    {
		for (int i = 0; i < channels.size(); i++)
		{
			channels[i]->receive(client->id, message);
		}
	}

	return "";
}
