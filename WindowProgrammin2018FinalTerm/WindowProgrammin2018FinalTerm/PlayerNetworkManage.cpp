#include "PlayerNetworkManager.h"
#include "network.h"

void PlayerNetworkManager::setSocket(const SOCKET& sock,int pn) {
	socket = sock;
	//ULONG isNonBlocking = 1;
	//ioctlsocket(socket,        //Non-Blocking으로 변경할 소켓
	//	FIONBIO,       //변경할 소켓의 입출력 모드
	//	&isNonBlocking //넘기는 인자, 여기서는 nonblocking설정 값
	//);
	playerNum = pn;
	CreateThread(NULL, 0, this->recvData, this, 0, NULL);
}

void voidBuffer(SOCKET s)
{
	u_long tmpl, i;
	char tmpc; ioctlsocket(s, FIONREAD, &tmpl);
	for (i = 0; i < tmpl; i++)
		recv(s, &tmpc, sizeof(char), 0);
}

DWORD WINAPI PlayerNetworkManager::recvData(LPVOID pPNM)
{
	PlayerNetworkManager* This = (PlayerNetworkManager*)pPNM;

	//voidBuffer(This->socket);
	while (1)
	{
		printf("Pwait... %d\n", This->playerNum);

		WaitForSingleObject(This->WaitMainStream, INFINITE);
		This->makeDone = false;
		int retval;
		retval = recvn(This ->socket, (char*)&(This->CTS), sizeof(ClientToServer), 0);

		printf("%d %d %d %d %d\n", This->CTS.PlayerNum, This->CTS.drawState, This->CTS.x, This->CTS.y, This->CTS.AttackedPlayerNum[0]);
		printf("Pdone! %d\n", This->playerNum);

		This->makeDone = true;
		Sleep(17);


		//SetEvent(This->WaitAllDataWriting);
	}
	return NULL;
}

void PlayerNetworkManager::sendData(const ServerToClient& stc)
{
	int retval;
	static int count = 0;
	retval = send(socket, (char*)&stc, sizeof(ServerToClient), 0);
	printf("%d Send : %d\n", count++, stc.PlayerData[0].drawState);
}

ClientToServer& PlayerNetworkManager::getCTS()
{
	printf("CTS :: %d\n", CTS.drawState);
	return CTS;
}

int PlayerNetworkManager::isDone()
{
	printf("Call isDone by : %d\n", playerNum);
	if (makeDone)
		return 1;
	else
		return 0;
}
