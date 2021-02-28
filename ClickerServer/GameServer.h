//imports
#pragma comment(lib, "Ws2_32.lib")

#include <sdkddkver.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <conio.h>
#include <stdio.h>

#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#define SCK_VERSION2 0x0202

#include <WS2tcpip.h>

using namespace std;

namespace GameServer
{
	// Classes
	class Client {
	public:
		SOCKET socket;
		string username;
		Client(SOCKET& socket)
		{
			this->socket = socket;
			this->username = "";
		}
	};

	// Variables
	extern std::vector<Client> clients;

	// Events
	void ServerStartEvent();
	void ClientPreConnectEvent(Client& client);
	void ClientSentMessageEvent(Client& client, string message);
	void ClientDisconnectedEvent(Client& client);
	void ServerEndEvent();

	// Behaviors
	void StartServerLoop();
	void BroadcastMessage(string message);
	void SendClientMessage(Client& client, string message);
	string ListenToClientMessage(Client& client);
}
