#pragma once
#include "protocol.h"

class PlayerNetworkManager
{
	SOCKET socket;

public:
	PlayerNetworkManager(){}
	~PlayerNetworkManager(){}
	void setSocket(const SOCKET&);
	static DWORD WINAPI recvData(LPVOID);
	void sendData(const ServerToClient&);
};

