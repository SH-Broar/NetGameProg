#include "PlayerNetworkManager.h"


void PlayerNetworkManager::setSocket(const SOCKET& sock) {
	socket = sock;
	CreateThread(NULL, 0, recvData, (LPVOID)NULL, 0, NULL);
}

DWORD WINAPI PlayerNetworkManager::recvData(LPVOID) {

}
void PlayerNetworkManager::sendData(const ServerToClient&) {

}