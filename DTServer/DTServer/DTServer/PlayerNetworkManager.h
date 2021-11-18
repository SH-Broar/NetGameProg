#pragma once
#include "protocol.h"

class PlayerNetworkManager
{
	SOCKET socket;
	int playerNum;
	ServerToClient* pData;

public:
	PlayerNetworkManager() { playerNum = 0; }
	~PlayerNetworkManager(){}
	void setNum(int n) { playerNum = n; }
	void setData(ServerToClient* p) { pData = p; }
	void setSocket(const SOCKET&);
	static DWORD WINAPI recvData(LPVOID);
	void sendData(const ServerToClient&);
};

