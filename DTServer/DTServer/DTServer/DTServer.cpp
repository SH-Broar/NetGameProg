#pragma comment(lib, "ws2_32")


#include "protocol.h"
#include "MainStream.h"

#define SERVERPORT 9000
#define BUFSIZE 512

CRITICAL_SECTION cs;

DWORD WINAPI ProcessClient(LPVOID arg);

void SetCursorPosition(int y)
{
	COORD pos = { 0,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


int main()
{
	//서버 초기화
	MainStream mainStream;
	//클라이언트 접속 대기 
	mainStream.WaitForClientToConnect();


	mainStream.PlayerSelectStart();
	//모든 클라이언트에 씬 변환 데이터 전송
	//캐릭터 선택 창 

	//수신 쓰레드 3개 만들기

	//송신 쓰레드 1개 만들기

	mainStream.GameLogic();
	//모든 클라이언트가 준비완료가 되면 게임 시작

	//게임 씬

	/*
	int retval;
	InitializeCriticalSection(&cs);

	//initialize
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	//MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);

	//socket
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

	while (1)
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

		HANDLE tmpHandle = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if (tmpHandle == NULL) closesocket(client_sock);
		else CloseHandle(tmpHandle);
	}

	//closesocket()
	closesocket(listen_sock);

	DeleteCriticalSection(&cs);
	WSACleanup();
	return 0;
	*/
}
/*
DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN clientaddr;

	int retval;
	int addrlen;
	char buf[BUFSIZE + 1];

	int len;

	//---

	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	char recvBuff[BUFSIZE];

	//접속 확인 플래그 (4비트)
	len = recvn(client_sock, (char*)&recvBuff, sizeof(int), 0);
	if (len == SOCKET_ERROR)
	{
		err_display("recv()");
		return -1;
	}
	else if (len == 0)
		return -1;

	if (atoi(recvBuff) == 9999) // 접속 플래그
	{
		char sendBuff[BUFSIZE];
		sprintf(sendBuff, "%d",777);

		retval = send(client_sock, sendBuff, sizeof(int), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			return 0;
		}
		printf("send done!");
	}

	//통신
	//while (1)
	//{
	//	//가변 길이
	//	retval = recvn(client_sock, buf, 10, 0);

	//	if (retval == SOCKET_ERROR)
	//	{
	//		err_display("recv()");
	//		break;
	//	}
	//	else if (retval == 0)
	//		break;

	//	//Logic Here

	//}

	//closeSocket()
	closesocket(client_sock);
}
*/
