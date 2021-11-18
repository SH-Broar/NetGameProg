#include "PlayerNetworkManager.h"
#include "network.h"

//HANDLE hReceiveEvent[3];
//HANDLE hReadCompeleteEvent;

void PlayerNetworkManager::setSocket(const SOCKET& sock) {
	socket = sock;
	CreateThread(NULL, 0, recvData, (LPVOID)this, 0, NULL);
}

DWORD WINAPI PlayerNetworkManager::recvData(LPVOID arg) {
	PlayerNetworkManager* P = (PlayerNetworkManager*)arg;
	ClientToServer clientData;
	while (true) {
		WaitForSingleObject(hReadCompeleteEvent, INFINITE);
		recvn(P->socket, (char*)&clientData, sizeof(ClientToServer), NULL);
		P->pData->PlayerData[P->playerNum] = clientData;
		SetEvent(hReceiveEvent[P->playerNum]);
	}
	return 0;
}
void PlayerNetworkManager::sendData(const ServerToClient& data) {
	send(socket, (char*)&data, sizeof(data), NULL);
}

