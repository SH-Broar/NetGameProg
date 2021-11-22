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
	int PlayerNum; 	//�÷��̾� ��ȣ
	int x;	//�÷��̾��� x��ǥ
	int y;	//�÷��̾��� y��ǥ
	int draw_state; // *2�϶��� characterNum, �÷��̾� ����
	char AttackedPlayerNum[3];//���� �÷��̾��� ��ȣ


};

struct ServerToClient
{
	ClientToServer PlayerData[3];	// �÷��̾� ������
	int CoinX;	//���� ������ X��ǥ
	int CoinY;	//���� ������ Y��ǥ
	int CoinState;	//������ ���� (���� ���� �ִ���)
	int Time;		//���� �ð�

};


#pragma pack()

void SetCursorPosition(int y);
void err_quit(char* msg);
void err_display(char* msg);
int recvn(SOCKET s, char* buf, int len, int flags);
void voidBuffer(SOCKET s);

