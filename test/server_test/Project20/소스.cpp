#pragma comment(lib, "ws2_32")

#include <iostream>
#include <vector>
#include "protocol.h"


#define SERVERPORT 9000
#define BUFSIZE 512

CRITICAL_SECTION cs;

DWORD WINAPI ProcessClient(LPVOID arg);


void SetCursorPosition(int y)
{
	COORD pos = { 0,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}


int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

int main()
{

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

}

DWORD WINAPI ProcessClient(LPVOID arg)
{
	printf("thread\n");

	char buf[BUFSIZE + 1];

	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN clientaddr;

	int retval;
	int addrlen;
	
	
	int code;
	int client_num = 6;

	//---

	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	char recvBuff[BUFSIZE];

	//접속 확인 플래그 (4비트)
	
	
	retval = recvn(client_sock, (char*)&code, sizeof(int), 0);

	if (code == 9999) {
		code = 777;
		send(client_sock, (char*)&code, sizeof(int), 0);
		send(client_sock, (char*)&client_num, sizeof(int), 0);
		client_num++;
	}
	

	ClientToServer client_to_server_data;

	ServerToClient server_to_client_data;


	int n = 0;
	while (1) {
		retval = recvn(client_sock, (char*)&client_to_server_data, sizeof(ClientToServer), 0);

		printf("[%d receive] playerNum:%d, x:%d, y:%d, draw:%d,    ",
			n++, client_to_server_data.PlayerNum, client_to_server_data.x, client_to_server_data.y, client_to_server_data.draw_state);

		server_to_client_data.PlayerData[0] = client_to_server_data;
		server_to_client_data.PlayerData[1] = client_to_server_data;
		server_to_client_data.PlayerData[2] = client_to_server_data;

		send(client_sock, (char*)&server_to_client_data, sizeof(server_to_client_data), 0);
		printf("send\n");
	}
	

	
	closesocket(client_sock);
}