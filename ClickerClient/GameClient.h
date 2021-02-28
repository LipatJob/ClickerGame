#pragma once
#pragma comment(lib, "Ws2_32.lib")

#include <sdkddkver.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <mutex>

#define SCK_VERSION2 0x0202

namespace GameClient
{
	void StartClient();
	SOCKET ConnectToServer();
	void SendClientMessage(SOCKET serverSocket);
	void ReadServerMessage(SOCKET serverSocket);
}