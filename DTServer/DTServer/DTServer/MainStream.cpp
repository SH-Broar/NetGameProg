#include "MainStream.h"
#include "protocol.h"
#include "network.h"


#define SERVERPORT 9000

HANDLE hReceiveEvent[3];
HANDLE hReadCompeleteEvent;

MainStream::MainStream() {
	int retval;
	//InitializeCriticalSection(&cs);
	for (int i = 0; i < 3; ++i) {
		hReceiveEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
	hReadCompeleteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		err_quit("WSAStartup()");
}

MainStream::~MainStream(){
	if (hReceiveEvent)CloseHandle(hReceiveEvent);
	if (hReadCompeleteEvent) CloseHandle(hReadCompeleteEvent);
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
		char buf[100];
		//recvn(client_sock, buf, 100, NULL);
#ifdef TEST_BEFORE_CLIENT_COMPLETE
		char test_num[] = "777 ";
		send(client_sock, (char*)&test_num, sizeof(char) * 4, 0);
#elif defined(TEST_TEST)
		int code = 777;
		send(client_sock, (char*)&code, sizeof(int), 0);
		send(client_sock, (char*)&player_num, sizeof(int), 0);
#else
		send(client_sock, (char*)&player_num, sizeof(int), 0);
#endif
		players[player_num].setNum(player_num);
		players[player_num].setData(&data);
		players[player_num++].setSocket(client_sock);
	}
	return;
}

void MainStream::PlayerSelectStart(){
	data.setInitForSelect();
	sendData(players, data);
	HANDLE tmpHandle = CreateThread(NULL, 0, SendData, this, 0, NULL);
	if (tmpHandle) CloseHandle(tmpHandle);
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	std::chrono::duration<double> sec;

	
	SetEvent(hReadCompeleteEvent);
	while (true) {
		WaitForMultipleObjects(3, hReceiveEvent, TRUE, INFINITE);
		sec = std::chrono::system_clock::now() - start;
		data.Time = 60 - sec.count();
		
		if (data.PlayerData[0].AttackedPlayerNum[0] + data.PlayerData[1].AttackedPlayerNum[0] + data.PlayerData[2].AttackedPlayerNum[0] >= 3) {
			//data.CoinState = 2000;
			sendData(players, data);
			SetEvent(hReadCompeleteEvent);
			break;
		}
		sendData(players, data);
		SetEvent(hReadCompeleteEvent);
	}
}
void MainStream::GameLogic(){}

void sendData(PlayerNetworkManager* players, ServerToClient& data) {
	players[0].sendData(data);
	players[1].sendData(data);
	players[2].sendData(data);

}

DWORD WINAPI MainStream::SendData(LPVOID arg) {
	MainStream* stream = (MainStream*)arg;
	while (true) {
		stream->players[0].sendData(stream->data);
		stream->players[1].sendData(stream->data);
		stream->players[2].sendData(stream->data);
		Sleep(100);
	}
}