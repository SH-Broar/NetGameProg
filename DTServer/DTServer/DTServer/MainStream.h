#pragma once
#include "protocol.h"

#include "PlayerNetworkManager.h"

class MainStream
{
private:
	WSADATA wsa;
	ServerToClient data;
	PlayerNetworkManager players[3];
public:
	MainStream();
	~MainStream();

	void WaitForClientToConnect();
	void PlayerSelectStart();
	void GameLogic();
	void CoinUpdate();
	static DWORD WINAPI SendDataThread(LPVOID arg);
	void SetData();
};


