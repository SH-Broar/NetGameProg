#include "MainStream.h"
#include "protocol.h"
#include "network.h"


#define SERVERPORT 9000

MainStream::MainStream() {
	int retval;
	//InitializeCriticalSection(&cs);

	
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		err_quit("WSAStartup()");
}

MainStream::~MainStream(){

}

void MainStream::WaitForClientToConnect() {
	int retval;
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	//bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;

	int player_num = 0;

	while (player_num < 3)
	{
		int addrlen;

		//accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}
#ifdef TEST_BEFORE_CLIENT_COMPLETE
		char test_num[] = "777 ";
		send(client_sock, (char*)&test_num, sizeof(char) * 4, 0);
#else
		send(client_sock, (char*)&player_num, sizeof(int), 0);
#endif
		players[player_num++].setSocket(client_sock);
	}
	return;
}

void MainStream::PlayerSelectStart(){
	ServerToClient scene;
	for (int i = 0; i < 3; ++i) {
		players[i].sendData(scene);	//캐릭터 선택 창으로 넘어갔다는 것을 알린다.

	}
}
void MainStream::GameLogic(){}


DWORD WINAPI sendData(LPVOID arg) {

}

DWORD WINAPI MainStream::sendData(LPVOID arg) {
	return 0;
}


