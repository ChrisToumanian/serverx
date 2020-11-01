#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include "server.h"
#include "channel.h"
#include "reader.h"
#include "channels/chat.h"
#include "user.h"

struct Message
{
	int client_id;
	std::string data;
};

bool running = false;
int port = 25566;
int max_clients = 100;
int queue_limit = 1000;
std::string server_name = "server";
std::string config_filename = "server.yml";
static std::vector<Channel*> channels;
static std::vector<Message*> queue;
static Server* server;

void read_config();
void load();
void start();
void loop();
void stop();
void manage_queue();
void send_to_channels(int client_id, std::string message);
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

	// Load chat channel
	Chat* chat = new Chat();
	chat->server = server;
	chat->load();
	channels.push_back(chat);
}

void start()
{
	// Begin queue loop
	running = true;
	std::thread loop_thread(loop);
	
	// Create server thread
	std::cout << "Initializing server..." << std::endl;
	server->start();

	// Clean up and exit
	loop_thread.join();
}

void loop()
{
	while (running)
	{
		manage_queue();
	}
}

void stop()
{
	server->stop();
}

std::string receive(Client* client, std::string message)
{
    Reader::rtrim(message);

	// Add to queue
	if (message.length() > 0)
    {
		Message* msg = new Message();
		msg->client_id = client->id;
		msg->data = message;
		queue.push_back(msg);
	}

	return "";
}

void manage_queue()
{
	// Enforce queue limit
	if (queue.size() > queue_limit)
	{
		for (int i = queue_limit - 1; i > -1; i--)
		{
			delete queue[i];
			queue[i] = NULL;
			queue.erase(queue.begin() + i);
		}
	}

	// Handle messages
	for (int i = 0; i < queue.size(); i++)
	{
		send_to_channels(queue[i]->client_id, queue[i]->data);
		queue.erase(queue.begin() + i);
	}
}

void send_to_channels(int client_id, std::string message)
{
	for (int i = 0; i < channels.size(); i++)
	{
		channels[i]->receive(client_id, message);
	}
}
