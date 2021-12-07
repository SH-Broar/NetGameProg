#pragma once
#include "protocol.h"

#include "PlayerNetworkManager.h"

class MainStream
{
private:
	WSADATA wsa;
	ServerToClient data;
	PlayerNetworkManager players[3];
	HANDLE WaitAllDataWriting[3];
	HANDLE WaitAllDataReading[3];
	int recvDoneCount = 0;

public:
	MainStream();
	~MainStream();

	void WaitForClientToConnect();
	void PlayerSelectStart();
	void GameLogic();
	void DataCrowl(int tc);
	void DataClear();
	static DWORD WINAPI sendData(LPVOID arg);

};


