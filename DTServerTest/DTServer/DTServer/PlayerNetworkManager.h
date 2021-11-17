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

private:
	int playerNum;

};

