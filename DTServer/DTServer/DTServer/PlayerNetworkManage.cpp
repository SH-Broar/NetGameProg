#include "PlayerNetworkManager.h"


void PlayerNetworkManager::setSocket(const SOCKET& sock) {
	socket = sock;
	CreateThread(NULL, 0, recvData, (LPVOID)this, 0, NULL);
}

DWORD WINAPI PlayerNetworkManager::recvData(LPVOID arg) {
	return 0;
}
void PlayerNetworkManager::sendData(const ServerToClient& data) {
	send(socket, (char*)&data, sizeof(data), NULL);
}