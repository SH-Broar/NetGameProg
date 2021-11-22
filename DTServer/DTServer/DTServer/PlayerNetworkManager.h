#pragma once
#include "protocol.h"

class PlayerNetworkManager
{
	SOCKET socket;
	int playerNum;
	ServerToClient* pData;
	ClientToServer CTS;

public:
	PlayerNetworkManager() { playerNum = 0; ZeroMemory(&CTS, sizeof(CTS)); }
	~PlayerNetworkManager(){}
	void setNum(int n) { playerNum = n; }
	void setData(ServerToClient* p) { pData = p; }
	void setSocket(const SOCKET&);
	//void setSocket(const SOCKET&, int);

	static DWORD WINAPI recvData(LPVOID);
	void sendData(const ServerToClient&);

	ClientToServer& getCTS() { return CTS; };
};

