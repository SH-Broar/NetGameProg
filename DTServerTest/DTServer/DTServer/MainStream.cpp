#include "MainStream.h"
#include "protocol.h"
#include "network.h"

#define MEMBERS 2
#define SERVERPORT 9000

MainStream::MainStream() {
	int retval;
	//InitializeCriticalSection(&cs);


	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		err_quit("WSAStartup()");


}

MainStream::~MainStream() {

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
		players[i].WaitMainStream = CreateEvent(NULL, FALSE, FALSE, NULL);

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
			printf("send done!\n");
			char sendBuff[512];
			sprintf(sendBuff, "%d", player_num);
			send(client_sock, (char*)&sendBuff, sizeof(int), 0);

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
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	std::chrono::duration<double> sec;

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

	//-----------------------------------------------------------------------------------------

	int timeCut = 0;
	while (true)
	{
		printf("wait...\n");
		while (recvDoneCount < MEMBERS)
		{
			recvDoneCount = 0;
			for (int i = 0; i < MEMBERS; ++i) {
				if (players[i].isDone() == 1)
					recvDoneCount++;
			}
			printf("recvDoneC : %d\n", recvDoneCount);
			Sleep(17);
			timeCut += 17;
		}
		//for (int i = 0; i < MEMBERS; ++i) {
		//	WaitForSingleObject(players[i].WaitAllDataWriting, INFINITE);
		//}
		recvDoneCount = 0;
		printf("done!\n");

		sec = std::chrono::system_clock::now() - start;

		DataCrowl(sec.count());
		if (players[0].getCTS().AttackedPlayerNum[0] == 1)//&& players[1].getCTS().AttackedPlayerNum[0] == 1 && players[2].getCTS().AttackedPlayerNum[0] == 1)
		{
			data.CoinState = 1;	//씬 끝나는 플래그 설정
		}

		for (int i = 0; i < MEMBERS; ++i) {
			players[i].sendData(data);
		}

		for (int i = 0; i < MEMBERS; ++i) {
			SetEvent(players[i].WaitMainStream);
		}

		if (data.CoinState == 1) //다음 씬으로 넘어갈 상태이면
		{
			break;
		}
	}
}
void MainStream::GameLogic() {}

void MainStream::DataCrowl(int timeCut)
{
	for (int i = 0; i < MEMBERS; ++i)
	{
		data.PlayerData[i] = players[i].getCTS();
	}
	data.Time = 60 - timeCut;
}


DWORD WINAPI sendData(LPVOID arg) {
	return NULL;
}

DWORD WINAPI MainStream::sendData(LPVOID arg) {
	return 0;
}


