#include "GameServer.h"

namespace GameServer
{
	vector<Client> clients;

	void ListenToClientMessageLoop(SOCKET socket);

	void StartServerLoop()
	{
		// Setup Server
		long SUCCESSFUL;
		WSADATA WinSockData;
		WORD DLLVERSION;
		DLLVERSION = MAKEWORD(2, 1);
		SUCCESSFUL = WSAStartup(DLLVERSION, &WinSockData);

		SOCKADDR_IN ADDRESS;
		int AddressSize = sizeof(ADDRESS);
		ADDRESS.sin_addr.s_addr = inet_addr("127.0.0.1");
		ADDRESS.sin_family = AF_INET;
		ADDRESS.sin_port = htons(444);

		SOCKET sock_LISTEN;	
		sock_LISTEN = socket(AF_INET, SOCK_STREAM, NULL);
		bind(sock_LISTEN, (SOCKADDR*)&ADDRESS, sizeof(ADDRESS));
		listen(sock_LISTEN, SOMAXCONN);

		// EVENT: Server Started
		ServerStartEvent();

		SOCKET AcceptSocket;
		vector<thread> clientThreads;
		for (;;)
		{
			// Wait for clients to connect
			AcceptSocket = SOCKET_ERROR;
			while (AcceptSocket == SOCKET_ERROR)
			{
				AcceptSocket = accept(sock_LISTEN, (SOCKADDR*)&ADDRESS, &AddressSize);
			}


			thread clientThread(ListenToClientMessageLoop, AcceptSocket);

			// Add thread and socket to list
			clientThreads.push_back(move(clientThread));
		}

		// Closing server
		for (auto& thread : clientThreads)
		{
			thread.join();
		}
		ServerEndEvent();
	}

	void ListenToClientMessageLoop(SOCKET socket)
	{
		// Create client
		Client client(socket);
		ClientPreConnectEvent(client);
		clients.push_back(client);

		// Listen for client messages
		char CLIENT_MESSAGE[100];
		for (;;)
		{
			// Receive message
			memset(CLIENT_MESSAGE, 0, sizeof(CLIENT_MESSAGE));
			int recvstatus = recv(client.socket, CLIENT_MESSAGE, sizeof(CLIENT_MESSAGE), 0);

			// Event: Client has disconnected
			if (recvstatus == SOCKET_ERROR)
			{
				ClientDisconnectedEvent(client);
				break;
			}

			// Event: Client Sent a message
			string converted(CLIENT_MESSAGE);
			ClientSentMessageEvent(client, converted);
		}
	}


	void BroadcastMessage(string message)
	{
		// Format message to char array
		char MESSAGE_BUFFER[100];
		memset(&MESSAGE_BUFFER, 0, sizeof(MESSAGE_BUFFER));
		strcpy_s(MESSAGE_BUFFER, message.c_str());

		for (auto& client : clients)
		{
			// Skip broadcasting to the sender and sockets that are invalid
			if (client.socket == SOCKET_ERROR) { continue; }

			// Send message to sockets
			send(client.socket, MESSAGE_BUFFER, strlen(MESSAGE_BUFFER) + 1, NULL);
		}
	}

	void SendClientMessage(Client& client, string message)
	{
		// Format message to char array
		char MESSAGE_BUFFER[100];
		memset(&MESSAGE_BUFFER, 0, sizeof(MESSAGE_BUFFER));
		strcpy_s(MESSAGE_BUFFER, message.c_str());

		if (client.socket == SOCKET_ERROR) { return; }

		// Send message to sockets
		send(client.socket, MESSAGE_BUFFER, strlen(MESSAGE_BUFFER) + 1, NULL);
	}

	string ListenToClientMessage(Client& client)
	{
		// Create a message buffer
		char CLIENT_MESSAGE[100];
		memset(CLIENT_MESSAGE, 0, sizeof(CLIENT_MESSAGE));

		// Get message
		int recvstatus = recv(client.socket, CLIENT_MESSAGE, sizeof(CLIENT_MESSAGE), 0);

		// Handle if there is error
		if (recvstatus == SOCKET_ERROR) { return "Error"; }

		// Convert char array string to string object
		string converted(CLIENT_MESSAGE);
		return converted;
	}

}
