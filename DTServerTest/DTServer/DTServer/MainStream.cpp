#include "MainStream.h"
#include "protocol.h"
#include "network.h"

#define MEMBERS 1
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

	char recvBuff[512];

	for (int i = 0; i < MEMBERS; ++i)
		players[i].WaitAllDataWriting = CreateEvent(NULL, FALSE, FALSE, NULL);
	for (int i = 0; i < MEMBERS; ++i)
		players[i].WaitMainStream = CreateEvent(NULL, FALSE, TRUE, NULL);

	while (player_num < MEMBERS)
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

		//접속 확인 플래그 (4비트)
		recvn(client_sock, (char*)&recvBuff, sizeof(int), 0);

		if (atoi(recvBuff) == 9999) // 접속 플래그
		{
			printf("send done!");
			send(client_sock, (char*)&player_num, sizeof(int), 0);

			printf("%d player socket created\n", player_num);
			players[player_num++].setSocket(client_sock, player_num);
		}

//#ifdef TEST_BEFORE_CLIENT_COMPLETE
//		char test_num[] = "777";
//		send(client_sock, (char*)&test_num, sizeof(char) * 4, 0);
//#else

//#endif

	}

	return;
}

void MainStream::PlayerSelectStart()
{
	ServerToClient scene;
	scene.setInitForSelect();
	for (int i = 0; i < MEMBERS; ++i) {
		players[i].getCTS() = scene.PlayerData[i];
		players[i].sendData(scene);	//캐릭터 선택 창으로 넘어갔다는 것을 알린다.
	}

	data = scene;
	printf("선택창으로 넘어가기 %d\n", data.CoinState);
	Sleep(17);
	for (int i = 0; i < MEMBERS; ++i) {
		players[i].getCTS() = scene.PlayerData[i];
		players[i].sendData(scene);	 // 한번 더미 보내줘야함 (ㅠㅠ?)
	}
	for (int i = 0; i < MEMBERS; ++i) {
		SetEvent(players[i].WaitMainStream);
	}
	while (true)
	{
		printf("wait...");
		for (int i = 0; i < MEMBERS; ++i) {
			WaitForSingleObject(players[i].WaitAllDataWriting, INFINITE);
		}
		printf("done!");
		DataCrowl();
		for (int i = 0; i < MEMBERS; ++i) {
			players[i].sendData(data);
		}

		for (int i = 0; i < MEMBERS; ++i) {
			SetEvent(players[i].WaitMainStream);
		}

		if (players[0].getCTS().AttackedPlayerNum[0] == 1 )//&& players[1].getCTS().AttackedPlayerNum[0] == 1 && players[2].getCTS().AttackedPlayerNum[0] == 1)
		{
			break;
		}
		Sleep(17);
	}
}
void MainStream::GameLogic(){}

void MainStream::DataCrowl()
{
	for (int i = 0; i < MEMBERS; ++i)
	{
		data.PlayerData[i] = players[i].getCTS();
	}
}


DWORD WINAPI sendData(LPVOID arg) {
	return NULL;
}

DWORD WINAPI MainStream::sendData(LPVOID arg) {
	return 0;
}


