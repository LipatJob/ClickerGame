#include "GameClient.h"

using namespace std;

namespace GameClient
{
	void StartClient()
	{
		// Prompt user to connect to server
		string connectSelection;
		cout << "Do you want to connect to the server? (Y/N): ";
		getline(cin, connectSelection);

		if (connectSelection == "Y")
		{
			SOCKET serverSocket = ConnectToServer();

			// Start Threads
			thread clientMessageThread(SendClientMessage, serverSocket);
			thread serverThread(ReadServerMessage, serverSocket);

			clientMessageThread.join();
			serverThread.join();
		}
		else if (connectSelection == "N")
		{
			cout << " App not connected. Closing application..." << endl;

		}
		else {
			cout << "Invalid selecion. Closing application..." << endl;
		}
	}

	SOCKET ConnectToServer()
	{
		long SUCCESSFUL;
		WSADATA WinSockData;
		WORD DLLVERSION;

		DLLVERSION = MAKEWORD(2, 1);

		SUCCESSFUL = WSAStartup(DLLVERSION, &WinSockData);

		SOCKADDR_IN ADDRESS;

		SOCKET sock;
		sock = socket(AF_INET, SOCK_STREAM, NULL);

		ADDRESS.sin_addr.s_addr = inet_addr("127.0.0.1");
		ADDRESS.sin_family = AF_INET;
		ADDRESS.sin_port = htons(444);

		connect(sock, (SOCKADDR*)&ADDRESS, sizeof(ADDRESS));

		return sock;
	}

	void SendClientMessage(SOCKET serverSocket)
	{
		char CLIENT_MESSAGE[100];

		for (;;)
		{
			// Get message
			string message;
			getline(cin, message);

			// Convert string message to char array
			memset(&CLIENT_MESSAGE, 0, sizeof(CLIENT_MESSAGE));
			strcpy_s(CLIENT_MESSAGE, message.c_str());

			// Send message to server
			send(serverSocket, CLIENT_MESSAGE, strlen(CLIENT_MESSAGE) + 1, NULL);

		}
	}

	void ReadServerMessage(SOCKET serverSocket)
	{
		char SERVER_MESSAGE[100];
		for (;;)
		{
			// clear message buffer
			memset(SERVER_MESSAGE, 0, sizeof(SERVER_MESSAGE));

			// try to receive messge
			int recvstatus = recv(serverSocket, SERVER_MESSAGE, sizeof(SERVER_MESSAGE), 0);

			// if socket is disconnected, then exit
			if (recvstatus == SOCKET_ERROR)
			{
				cout << "Server disconnected" << endl;
				break;
			}

			// display message
			string converted(SERVER_MESSAGE);
			cout << converted;
		}
	}
}

