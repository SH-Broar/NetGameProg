#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512



DWORD WINAPI client_thread(LPVOID arg);


#pragma pack(1)
struct ClientToServer
{
	int PlayerNum; 	//플레이어 번호
	int x;	//플레이어의 x좌표
	int y;	//플레이어의 y좌표
	int draw_state; // *2일때는 characterNum, 플레이어 상태
	char AttackedPlayerNum[3];//맞은 플레이어의 번호


};

struct ServerToClient
{
	ClientToServer PlayerData[3];	// 플레이어 데이터
	int CoinX;	//현재 코인의 X좌표
	int CoinY;	//현재 코인의 Y좌표
	int CoinState;	//코인의 상태 (누가 갖고 있는지)
	int Time;		//남은 시간

};


#pragma pack()

void SetCursorPosition(int y);
void err_quit(char* msg);
void err_display(char* msg);
int recvn(SOCKET s, char* buf, int len, int flags);
void voidBuffer(SOCKET s);

