#include "PlayerNetworkManager.h"
#include "network.h"

void PlayerNetworkManager::setSocket(const SOCKET& sock,int pn) {
	socket = sock;
	//ULONG isNonBlocking = 1;
	//ioctlsocket(socket,        //Non-Blocking���� ������ ����
	//	FIONBIO,       //������ ������ ����� ���
	//	&isNonBlocking //�ѱ�� ����, ���⼭�� nonblocking���� ��
	//);
	playerNum = pn;
	
	WaitMainStream = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitRecvComplete = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitMainStreamForSend = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitSendComplete = CreateEvent(NULL, FALSE, FALSE, NULL);
	CreateThread(NULL, 0, this->recvData, this, 0, NULL);
	CreateThread(NULL, 0, this->sendThread, this, 0, NULL);
}

void PlayerNetworkManager::setSTC(ServerToClient* stc)
{
	pSTC = stc;
}

DWORD WINAPI PlayerNetworkManager::recvData(LPVOID pPNM)
{
	PlayerNetworkManager* This = (PlayerNetworkManager*)pPNM;

	while (1)
	{
		printf("Pwait... %d\n", This->playerNum);

		WaitForSingleObject(This->WaitMainStream, INFINITE);
		//This->makeDone = false;
		int retval;
		retval = recvn(This ->socket, (char*)&(This->CTS), sizeof(ClientToServer), 0);

		printf("%d %d %d %d %d\n", This->CTS.PlayerNum, This->CTS.drawState, This->CTS.x, This->CTS.y, This->CTS.AttackedPlayerNum[0]);
		printf("Pdone! %d\n", This->playerNum);

		Sleep(0);
		//This->makeDone = true;
		SetEvent(This->WaitRecvComplete);
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

void PlayerNetworkManager::sendData()
{
	int retval;
	static int count = 0;
	retval = send(socket, (char*)pSTC, sizeof(ServerToClient), 0);
	printf("%d Send : %d\n", count++, pSTC->PlayerData[0].drawState);
}

DWORD WINAPI PlayerNetworkManager::sendThread(LPVOID pPNM)
{
	PlayerNetworkManager* This = (PlayerNetworkManager*)pPNM;
	while (true) 
	{
		WaitForSingleObject(This->WaitMainStreamForSend, INFINITE);
		This->sendData();
		SetEvent(This->WaitSendComplete);
	}
	return NULL;
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
