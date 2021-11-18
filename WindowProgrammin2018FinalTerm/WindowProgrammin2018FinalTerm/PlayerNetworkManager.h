#pragma once
#include "protocol.h"

class PlayerNetworkManager
{
	SOCKET socket;
	ClientToServer CTS;

public:
	PlayerNetworkManager(){}
	~PlayerNetworkManager(){}
	void setSocket(const SOCKET&, int);
	static DWORD WINAPI recvData(LPVOID);
	void sendData(const ServerToClient&);

	ClientToServer& getCTS();
	HANDLE WaitAllDataWriting;
	HANDLE WaitMainStream;

	int isDone();

private:
	int playerNum;
	bool makeDone;
};

