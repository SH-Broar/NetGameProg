#pragma once
#include "protocol.h"

class PlayerNetworkManager
{
	SOCKET socket;
	ClientToServer CTS;
	ServerToClient* pSTC;
public:
	PlayerNetworkManager(){}
	~PlayerNetworkManager(){}
	void setSocket(const SOCKET&, int);
	void setSTC(ServerToClient*);
	static DWORD WINAPI recvData(LPVOID);
	static DWORD WINAPI sendThread(LPVOID);
	void sendData(const ServerToClient&);
	void sendData();
	ClientToServer& getCTS();
	HANDLE WaitRecvComplete;
	HANDLE WaitMainStream;
	HANDLE WaitMainStreamForSend;
	HANDLE WaitSendComplete;

	int isDone();

private:
	int playerNum;
	bool makeDone;
};

