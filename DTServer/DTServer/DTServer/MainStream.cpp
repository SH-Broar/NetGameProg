#include "MainStream.h"
#include "protocol.h"
#include "network.h"


void sendData(PlayerNetworkManager* players, ServerToClient& data);
float distance(int ax, int ay, int bx, int by);

#define SERVERPORT 9000

HANDLE hReceiveEvent[3];
HANDLE hReadCompeleteEvent[3];

MainStream::MainStream() {
	int retval;
	//InitializeCriticalSection(&cs);
	ZeroMemory(&data, sizeof(data));
	for (int i = 0; i < 3; ++i) {
		hReceiveEvent[i] = CreateEvent(NULL, FALSE, TRUE, NULL);
		hReadCompeleteEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
	//hReadCompeleteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	
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
		char buf[100];
		//recvn(client_sock, buf, 100, NULL);
#ifdef TEST_BEFORE_CLIENT_COMPLETE
		recvn(client_sock, (char*)buf, sizeof(int), 0);
		if (atoi(buf) != 9999) continue;
		sprintf(buf, "%d", player_num);
		send(client_sock, (char*)buf, sizeof(char) * 4, 0);
#elif defined(TEST_TEST)
		int code = 777;
		send(client_sock, (char*)&code, sizeof(int), 0);
		send(client_sock, (char*)&player_num, sizeof(int), 0);
#else
		send(client_sock, (char*)&player_num, sizeof(int), 0);
#endif

		players[player_num].setNum(player_num);
		players[player_num].setSocket(client_sock);
		player_num++;
	}
	return;
}

void MainStream::PlayerSelectStart(){
	ZeroMemory(&data, sizeof(data));
	data.setInitForSelect();
	sendData(players, data);
	//HANDLE tmpHandle = CreateThread(NULL, 0, SendDataThread, this, 0, NULL);
	//if (tmpHandle) CloseHandle(tmpHandle);
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	std::chrono::duration<double> sec;
	
	sendData(players, data);
	for (int i = 0; i < 3; ++i) {
		SetEvent(hReadCompeleteEvent[i]);
	}
	data.setInitForSelect();
	//sendData(players, data);
	while (true) {
		WaitForMultipleObjects(3, hReceiveEvent, TRUE, INFINITE);
		SetData();
#ifdef SELECT_TEST
		for (int i = 0; i < 3; ++i) {
			if (data.PlayerData[i].drawState == 0) data.PlayerData[i].drawState = 1;
		}
#endif
		sec = std::chrono::system_clock::now() - start;
		data.Time = 60 - sec.count();
		// 모두 준비되면
		if (data.PlayerData[0].ready[0] + data.PlayerData[1].ready[0] + data.PlayerData[2].ready[0] >= 3) {
			//data.CoinState = 2000;
			/*sendData(players, data);
			for (int i = 0; i < 3; ++i) {
				SetEvent(hReadCompeleteEvent[i]);
			}*/
			break;
		}
		sendData(players, data);
		for (int i = 0; i < 3; ++i) {
			SetEvent(hReadCompeleteEvent[i]);
		}
		Sleep(100);
	}
}
void MainStream::GameLogic(){
	data.setInitForGame();
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	std::chrono::duration<double> sec;

	while (true) {
		for (int i = 0; i < 3; ++i) {
			SetEvent(hReadCompeleteEvent[i]);
		}
		Sleep(100);
		WaitForMultipleObjects(3, hReceiveEvent, TRUE, INFINITE);
		SetData();
		//timeelapsed
		sec = std::chrono::system_clock::now() - start;
		data.Time = 90 - sec.count();

		//coinupdate


		// 모두 준비되면
		sendData(players, data);
	}
}

void MainStream::CoinUpdate() {
	if (data.CoinState == COIN_DROP) {
		float dist;
		float min_dist = distance(data.CoinX, data.CoinY, data.PlayerData[0].x, data.PlayerData[0].y);
		int min_index = 0;
		for (int i = 1; i < 3; ++i) {
			if (data.PlayerData[i].drawState == 8 || data.PlayerData[i].drawState == 9) continue;

			dist = distance(data.CoinX, data.CoinY, data.PlayerData[i].x, data.PlayerData[i].y);
			if (dist < min_dist) {
				min_dist = dist;
				min_index = i;
			}
		}

		if (min_dist < 30) {
			data.CoinState = min_index;
		}
		return;
	}
	if (data.PlayerData[data.CoinState].drawState == 8 || data.PlayerData[data.CoinState].drawState == 9) { 
		
		data.CoinX = data.PlayerData[data.CoinState].x - 10;
		data.CoinY = data.PlayerData[data.CoinState].y;
		data.CoinState = COIN_DROP;
	}
	return;
}


void sendData(PlayerNetworkManager* players, ServerToClient& data) {
	players[0].sendData(data);
	players[1].sendData(data);
	players[2].sendData(data);

}

DWORD WINAPI MainStream::SendDataThread(LPVOID arg) {
	MainStream* stream = (MainStream*)arg;
	while (true) {
		stream->players[0].sendData(stream->data);
		stream->players[1].sendData(stream->data);
		stream->players[2].sendData(stream->data);
		Sleep(100);
	}
}

void MainStream::SetData() {
	for (int i = 0; i < 3; ++i) {
		data.PlayerData[i] = players[i].getCTS();	
	}
}

// 거리의 제곱을 리턴
float distance(int ax, int ay, int bx, int by) {
	float cx = ax - bx;
	float cy = ay - by;

	return cx * cx + cy * cy;
}