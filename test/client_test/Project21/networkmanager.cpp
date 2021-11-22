#include "protocol.h"
#include "networkmanager.h"

#include <stdio.h>
void NetworkManager::Connect() {
	int retval;

	Socket = socket(AF_INET, SOCK_STREAM, 0);
	HANDLE socket_thread_Handle = CreateThread(NULL, 0, client_thread, (LPVOID)Socket, 0, NULL);

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(Socket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	
}

int NetworkManager::receive_number_from_server() {
	int retval;

	int code = 9999;
	retval = send(Socket, (char*)&code, sizeof(int), 0);
	retval = recvn(Socket, (char*)&code, sizeof(int), 0);

	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return -1;
	}

	else if (retval == 0)
		return -1;

	else if (code != 777)
		return -1;

	retval = recvn(Socket, (char*)&playerNum, sizeof(int), 0);
	return 0;
}

NetworkManager::~NetworkManager() {
	WSACleanup();

}


NetworkManager::NetworkManager(ClientToServer* pointer) {
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		throw ("WSAStartup");


	players = pointer;
}




void NetworkManager::sendData(const ClientToServer& data) {
	int retval;
	retval = send(Socket, (char*)&data, sizeof(ClientToServer), 0);

}

void NetworkManager::recvData() {
	int retval;

	for (int i = 0; i < 3; i++)
		retval = recvn(Socket, (char*)&players[i], sizeof(ClientToServer), 0);
	voidBuffer(this->Socket);
	

}

void NetworkManager::print_my_player_number() {
	printf("playernum:%d\n", playerNum);
}

int NetworkManager::get_playerNum() {
	return playerNum;
 }