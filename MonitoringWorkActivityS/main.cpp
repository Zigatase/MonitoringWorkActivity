#include "server.h"

u_int panelId;

int main()
{
	// Init socket
	WSADATA wsa_data;

	int ws_ok = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (ws_ok != 0)
	{
		return -1;
	}

	// Creat socket
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_socket == INVALID_SOCKET)
	{
		return -1;
	}

	// Bind a socket to an address and port
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(77777);

	bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));

	// Start listening for connections
	listen(server_socket, 5);

	// Create the master file descriptor set and zero it
	fd_set master;
	FD_ZERO(&master);

	// Add our first socket that we're interested in interacting with; the listening socket!
	// It's important that this socket is added for our server or else we won't 'hear' incoming
	// Connections
	FD_SET(server_socket, &master);

	bool running = true;

	while (running)
	{
		fd_set copy = master;

		// See who's talking to us
		int socket_count = select(0, &copy, nullptr, nullptr, nullptr);

		// Loop through all the current connections / potential connect
		for (int i = 0; i < socket_count; i++)
		{
			// Makes things easy for us doing this assignment
			SOCKET sock = copy.fd_array[i];

			// Is it an inbound communication?
			if (sock == server_socket)
			{
				char buf[4096];
				ZeroMemory(buf, 4096);

				// Accept a new connection
				SOCKET client = accept(server_socket, nullptr, nullptr);

				// Add the new connection to the list of connected clients
				FD_SET(client, &master);

				// Send a welcome message to the connected client
				std::string connectionMessage = "-Connect";
				send(client, connectionMessage.c_str(), connectionMessage.size() + 1, 0);
			}
			else // It's an inbound messag
			{
				char buf[4096];
				ZeroMemory(buf, 4096);

				// Receive message
				int bytesIn = recv(sock, buf, 4096, 0);
				if (bytesIn <= 0)
				{
					// Drop Client
					closesocket(sock);
					FD_CLR(sock, &master);
				}

				// --- COMMAND ---
				if (buf[0] == 'T')
				{
					std::cout << "[Command from the client]: " << buf << std::endl;
				}
			}
		}
	}

	// Remove the listening socket from the master file descriptor set and close it
	// to prevent anyone else trying to connect.
	FD_CLR(server_socket, &master);
	closesocket(server_socket);

	// Cleanup winsock
	WSACleanup();

	system("pause");
	return 0;
}