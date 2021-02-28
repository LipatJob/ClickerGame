#include "GameServer.h"
#include "ClickerGame.h"
#include <mutex>

void ClickerGame::GameLoopTriggered()
{
	string message = "Power: " + to_string(ClickerGame::GetPower()) + "\n";
	GameServer::BroadcastMessage(message);
}

namespace GameServer
{
	thread gameloop;
	mutex mx;

	void ServerStartEvent()
	{
		cout << "SERVER: Listening for connections" << endl;
		gameloop = thread(ClickerGame::StartGameLoop);
		cout << "SERVER: Game loop started" << endl;
	}

	void ServerEndEvent()
	{
		gameloop.join();
		cout << "Server Closing" << endl;
	}

	void ClientPreConnectEvent(Client& client)
	{
		do {
			SendClientMessage(client, "Please enter a username: ");
			client.username = ListenToClientMessage(client);

		} while (client.username.length() == 0);
		SendClientMessage(client, "You are now connected, " + client.username + "\n");
		cout << client.username << " has connected" << endl;
	}

	void ClientSentMessageEvent(Client& client, std::string message)
	{
		mx.lock();
		ClickerGame::Click();
		mx.unlock();

		BroadcastMessage("~~~~" + client.username + " has clicked" + "~~~~\n");
	}

	void ClientDisconnectedEvent(Client& client)
	{
		string message = client.username + " has disconnected";
		BroadcastMessage(message);
		cout << message << endl;
	}
}