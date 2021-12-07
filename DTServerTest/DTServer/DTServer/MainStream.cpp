#include "MainStream.h"
#include "protocol.h"
#include "network.h"

#define MEMBERS 3
#define SERVERPORT 9000

MainStream::MainStream() {
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

	int recvBuff;

	

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

		if (recvBuff == 9999) // 접속 플래그
		{
			printf("send done!\n");
			int sendBuff = player_num;
			//char sendBuff[512];
			//sprintf(sendBuff, "%d", player_num);
			send(client_sock, (char*)&sendBuff, sizeof(int), 0);

			printf("%d player socket created\n", player_num);
			players[player_num].setSTC(&data);
			players[player_num].setSocket(client_sock, player_num);
			WaitAllDataWriting[player_num] = players[player_num].WaitRecvComplete;
			WaitAllDataReading[player_num] = players[player_num].WaitSendComplete;
			player_num++;
		}

		//#ifdef TEST_BEFORE_CLIENT_COMPLETE
		//		char test_num[] = "777";
		//		send(client_sock, (char*)&test_num, sizeof(char) * 4, 0);
		//#else

		//#endif

	}
	closesocket(listen_sock);
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
		
		WaitForMultipleObjects(MEMBERS, WaitAllDataWriting, TRUE, INFINITE);

		printf("done!\n");

		sec = std::chrono::system_clock::now() - start;

		DataCrowl(sec.count());
		/*
		if (players[0].getCTS().AttackedPlayerNum[0] == 1 && players[1].getCTS().AttackedPlayerNum[0] == 1 && players[2].getCTS().AttackedPlayerNum[0] == 1)
		{
			printf("***Select Ended***\n");
			data.CoinState = 1;	//씬 끝나는 플래그 설정
		}
		*/
		
		int cnt = 0;
		for (int i = 0; i < MEMBERS; ++i)
		{
			cnt += players[i].getCTS().AttackedPlayerNum[0];
		}
		if (cnt >= MEMBERS)
		{
			printf("***Select Ended***\n");
			data.CoinState = 1;	//씬 끝나는 플래그 설정
		}

		for (int i = 0; i < MEMBERS; ++i) {	//	데이터 보내기
			SetEvent(players[i].WaitMainStreamForSend);
		}
		WaitForMultipleObjects(MEMBERS, WaitAllDataReading, TRUE, INFINITE);

		for (int i = 0; i < MEMBERS; ++i) {	//데이터 받기
			SetEvent(players[i].WaitMainStream);
		}

		if (data.CoinState == 1) //다음 씬으로 넘어갈 상태이면
		{
			break;
		}
	}
}
void MainStream::GameLogic()
{
	printf("***GameLogic Start***\n");
	ServerToClient scene;
	scene.setInitForIngame();

	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	std::chrono::duration<double> sec;

	int timeCut = 0;

	sec = std::chrono::system_clock::now() - start;

	data = scene;

	data.CoinState = 0;
	//DataCrowl(0);

	for (int i = 0; i < MEMBERS; ++i) {
		players[i].sendData(data);
	}

	while (true)
	{
		printf("wait...\n");
		WaitForMultipleObjects(MEMBERS, WaitAllDataWriting, TRUE, INFINITE);

		sec = std::chrono::system_clock::now() - start;

		DataCrowl(sec.count());

		//코인 처리
		for (int i = 0; i < MEMBERS; i++)
		{
			if (data.CoinState == 0)
			{
				if (data.PlayerData[i].drawState != 8 && data.PlayerData[i].drawState != 9)
				{
					if (abs(data.CoinX - data.PlayerData[i].x) < 30 && abs(data.CoinY - data.PlayerData[i].y) < 30)
					{
						data.CoinState = i + 1;
						//m_pFramework->GetPlayer(myPlayerNum)->iHaveCoin = TRUE;
						//CoinObject->SetDrawFalse();
					}
				}
			}
			else
			{
				for (int j = 0; j < MEMBERS; j++)
				{
					if (i != j)
					{
						if (data.PlayerData[i].AttackedPlayerNum[j])
						{
							if (j + 1 == data.CoinState)
							{
								data.CoinX = data.PlayerData[j].x;
								data.CoinY = data.PlayerData[j].y;
								data.CoinState = 0;
							}
						}
					}
				}

			}
		}


		for (int i = 0; i < MEMBERS; ++i) {	//데이터 보내기
			SetEvent(players[i].WaitMainStreamForSend);
		}
		WaitForMultipleObjects(MEMBERS, WaitAllDataReading, TRUE, INFINITE);

		for (int i = 0; i < MEMBERS; ++i) {	//데이터 받기
			SetEvent(players[i].WaitMainStream);
		}

		//게임 끝나면 종료해야함
		if (data.Time <= -1 && data.CoinState != 0)
		{
			printf("\n****Game End!!****\n");
			printf("****Game End!!****\n");
			WaitForMultipleObjects(MEMBERS, WaitAllDataWriting, TRUE, INFINITE);
			WaitForMultipleObjects(MEMBERS, WaitAllDataReading, TRUE, INFINITE);
			break;
		}
	}

}

void MainStream::DataCrowl(int timeCut)
{
	for (int i = 0; i < MEMBERS; ++i)
	{
		data.PlayerData[i] = players[i].getCTS();
	}
	data.Time = 99 - timeCut;
}

void MainStream::DataClear()
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


