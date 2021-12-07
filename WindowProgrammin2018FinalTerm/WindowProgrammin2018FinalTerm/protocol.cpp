#include "stdafx.h"
#include "protocol.h"

#define SERVERIP		"127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

NetworkManager::NetworkManager()
{

}

NetworkManager::~NetworkManager()
{
}

bool NetworkManager::connection()
{
	
	int retval = 0;
	char sendBuff[BUFSIZE];
	//char recvBuff[BUFSIZE];
	int loginCode = 9999;
	//sprintf(sendBuff, "%d", loginCode);
	retval = send(sock, (char*)&loginCode, sizeof(int), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		return false;
	}

	int len = 0;
	int recvBuff;
	len = recvn(sock, (char*)&recvBuff, sizeof(int), 0);
	//printf("%d", len);
	if (len == SOCKET_ERROR)
	{
		err_display("recv()");
		return false;
	}
	else if (len == 0)
		return false;
#ifdef _DEBUG
	printf("player number : %d", recvBuff + 1);
#endif
	if (recvBuff >= 0 && recvBuff  <= 2)
	{
		playerNum = recvBuff + 1;
	}

}

void NetworkManager::sendData(ClientToServer CtS)
{
	int retval;
	retval = send(sock, (char*)(&CtS), sizeof(ClientToServer), 0);

	//retval 예외처리
}

void NetworkManager::recvData(ServerToClient& StC)
{
	int retval;
	retval = recvn(sock, (char*)(&StC), sizeof(ServerToClient), 0);

	//retval 예외처리
}

void NetworkManager::err_quit(char* msg)
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

void NetworkManager::err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	LocalFree(lpMsgBuf);
}

int NetworkManager::recvn(SOCKET s, char* buf, int len, int flags)
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

int NetworkManager::init()
{
	int retval;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)\
		return 1;

	//socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	//connet()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	int delay = 1;
	setsockopt(sock, SOL_SOCKET, TCP_NODELAY, (const char*)&delay, sizeof(delay));


	if (retval == SOCKET_ERROR)
		err_quit("connect()");

	//ULONG isNonBlocking = 1;
	//ioctlsocket(sock,        //Non-Blocking으로 변경할 소켓
	//	FIONBIO,       //변경할 소켓의 입출력 모드
	//	&isNonBlocking //넘기는 인자, 여기서는 nonblocking설정 값
	//);

	return 0;
}
