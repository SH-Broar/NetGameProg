#include "PlayerNetworkManager.h"
#include "network.h"

//HANDLE hReceiveEvent[3];
//HANDLE hReadCompeleteEvent;

void PlayerNetworkManager::setSocket(const SOCKET& sock) {
	socket = sock;
	int optval = 200;
	int retval = 0;
	//retval = setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&optval, sizeof(optval));
	//if (retval == SOCKET_ERROR) err_quit("setsocketopt()");
	//u_long on = 1;
	//retval = ioctlsocket(sock, FIONBIO, &on);
	//if (retval == SOCKET_ERROR) err_quit("ioctlsocket()");
	CreateThread(NULL, 0, recvData, (LPVOID)this, 0, NULL);
}

DWORD WINAPI PlayerNetworkManager::recvData(LPVOID arg) {
	PlayerNetworkManager* P = (PlayerNetworkManager*)arg;
	ClientToServer tmp;
	while (true) {
		WaitForSingleObject(hReadCompeleteEvent[P->playerNum], INFINITE);
		
		recvn(P->socket, (char*)&tmp, sizeof(ClientToServer), NULL);
		P->CTS = tmp;
		SetEvent(hReceiveEvent[P->playerNum]);
	}
	return 0;
}
void PlayerNetworkManager::sendData(const ServerToClient& data) {
	send(socket, (char*)&data, sizeof(data), NULL);
}

