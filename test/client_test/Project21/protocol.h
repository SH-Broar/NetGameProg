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

struct PlayerData {
	int x;
	int y;	
	int draw_state;
};


int recvn(SOCKET, char*, int, int);
void err_quit(char*);
void err_display(char*);


class NetworkManager{
public:
	NetworkManager(PlayerData**);	//���� ����
	//~NetworkManager();	//���� ����

	void Connect();		//��Ʈ��ũ ����
	int receive_number_from_server();
	void sendData(const ClientToServer&);	//���� ���� ó���κп��� �Ѱ��ִ� ������
	//void Send(ClientToServer&);	//����

	void recvData(int, ServerToClient&); //���� ���� ó�� �κ����� �ѱ� ������
	void Recvn(ServerToClient*);	//�����͸� �ޱ�
	void setData(ServerToClient&);	//�ð� �÷��̾� ��ü ����
	ClientToServer CreateClientToServer();	//player�����͸� ������ �������ݷ� �����

	

private:

	SOCKET Socket;	//�ڽ��� ����
	PlayerData** players;	// �÷��̾� �迭 ������
	int playerNum;	// �÷��̾� ��ȣ
	int* time;	// �ð� ������
	//Coin*;	//���� ������
};
