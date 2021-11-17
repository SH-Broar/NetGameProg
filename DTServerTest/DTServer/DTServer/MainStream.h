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
	void DataCrowl();
	static DWORD WINAPI sendData(LPVOID arg);

};


