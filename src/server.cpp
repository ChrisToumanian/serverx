#include "server.h"

Server::Server(int _port, std::string (*_callback)(Client*, std::string))
{
	port = _port;
	running = false;
	callback = _callback;
	max_clients = 30;
}

void Server::start()
{
	running = true;
	
	// Begin server loop
	log("server", "Server initialized.");
	loop();
	log("server", "Server stopped.");
}

void Server::loop()
{
	char buffer[1024]; // set 1K data buffer
	int opt = TRUE;
	int master_socket, addrlen, new_socket, activity, i, valread, sd;
	int max_sd;
    int next_client_id = 1;
	struct sockaddr_in address;
	fd_set readfds;  // read socket descriptors

	// Create master socket
	if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Set master socket to allow multiple connections
	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	// Set type of socket created
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	// Bind socket to localhost port
	if (bind(master_socket, (struct sockaddr*)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	// Callback successful listen
	log("server", "Listening on port " + std::to_string(port));

	// Specify maximum of 3 pending connections for master socket
	if (listen(master_socket, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	// Accept incoming connections
	addrlen = sizeof(address);
	log("server", "Waiting for connections...");

    signal(SIGPIPE, signal_callback_handler);

	while (running)
	{
		// Clear socket set
		FD_ZERO(&readfds);

		// Add master socket to set
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		// Add child sockets to set
		for (auto& client : clients)
		{
			// Socket descriptor
			sd = client.socket;

			// If valid socket descriptor add to read list
			if (sd > 0)
				FD_SET(sd, &readfds);

			// Highest file descriptor number, needed for select function
			if (sd > max_sd)
				max_sd = sd;
		}

		// Wait for activity on one of the sockets, timeout is NULL
		activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }

        // If something happens on master socket there is an incoming connection
        if (FD_ISSET(master_socket, &readfds))
        {
            // Accept socket
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            // Create new client
            Client client;
            client.id = next_client_id;
            client.socket = new_socket;
            client.address = inet_ntoa(address.sin_addr);
            client.nickname = "client#" + std::to_string(client.id);
            next_client_id++;

            // Add client to list of clients
            clients.push_back(client);

            // Send new client greeting message
            if (send(client.socket, (welcome_message + "\r\n").c_str(), welcome_message.length() + 2, 0) != welcome_message.length() + 2)
            {
                perror("send");
            }

            // Callback connection
            callback(&client, "/connected");
        }

        // Else it's an IO operation on another socket
        for (auto& client : clients)
        {
            sd = client.socket;

            if (FD_ISSET(sd, &readfds))
            {
                // Check if closing and read incoming message
                if ((valread = read(sd, buffer, 1024)) == 0)
                {
                    // Close socket and remove client from list
                    getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                    disconnect(client.id);
                }
                else
                {
                    // Read buffer from socket
                    buffer[valread] = '\0';

                    if (strlen(buffer) > 0)
                    {
                        // Send client request
                        std::string response = callback(&client, buffer);

                        // Send response to client
                        if (response.length() > 2)
                        {
						    send(sd, (response + "\r\n").c_str(), response.length() + 2, 0);
                        }
                    }
                    
                }
            }
        }
	}
}

void Server::stop()
{
	running = false;
	callback(NULL, "Server stopped.");
}

void Server::broadcast(std::string message)
{
   	for (auto& client : clients)
    {
        send(client.socket, (message + "\r\n").c_str(), message.length() + 2, 0);
    }
}

void Server::send_to(int client_id, std::string message)
{
    for (auto& client : clients)
    {
        if (client.id == client_id)
        {
            send(client.socket, (message + "\r\n").c_str(), message.length() + 2, 0);
        }
    }
}

void Server::log(std::string name, std::string message)
{
	// Get current time
	time (&time_raw);
	timeinfo = localtime(&time_raw);
	strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", timeinfo);
	std::string time(time_buffer);
    
    // Output log message
    std::cout << "[" << time << "] [" << name << "] " << message << std::endl;
}

void Server::disconnect(int client_id)
{
    for (int i = 0; i < clients.size(); i++)
    {
        if (client_id == clients[i].id)
        {
            callback(&clients[i], "/disconnected");
            close(clients[i].socket);
            clients.erase(clients.begin() + i);
            break;
        }
    }
}

void Server::signal_callback_handler(int signum)
{
}
