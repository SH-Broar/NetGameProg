#ifndef PROTOCOLS 1
#include <WinSock2.h>
#include <Windows.h>
#include "OBJECT_Player.h"

#define SERVERIP		"127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

#pragma pack (1)

struct ClientToServer
{
	int PlayerNum; 	//�÷��̾� ��ȣ
	int x;	//�÷��̾��� x��ǥ
	int y;	//�÷��̾��� y��ǥ
	int state; // *2�϶��� characterNum, �÷��̾� ����
	bool AttackedPlayerNum[3];   //���� �÷��̾��� ��ȣ
	// *2�϶��� ready, �ٸ� ĳ���Ϳ��� ������ ���߽��״��� Ȯ���ϴ� �÷���

	void set(int pn, int charNum, bool isReady)
	{
		PlayerNum = pn;
		x = 0;
		y = 0;
		state = charNum;
		
		AttackedPlayerNum[0] = isReady;
		AttackedPlayerNum[1] = false;
		AttackedPlayerNum[2] = false;
	}

	void set(CObject_Player* CP)
	{
		//����
		x = CP->x;
		y = CP->y;
		state = CP->CharacterStatus;
	}

	void clear()
	{
		x = 0;
		y = 0;
		state = 1;

		AttackedPlayerNum[0] = false;
		AttackedPlayerNum[1] = false;
		AttackedPlayerNum[2] = false;
	}
};

struct ServerToClient
{
	ClientToServer PlayerData[3];	// �÷��̾� ������
	int CoinX;	//���� ������ X��ǥ
	int CoinY;	//���� ������ Y��ǥ
	int CoinState;	//������ ���� (���� ���� �ִ���)
	int Time;		//���� �ð�

	void clear()
	{
		PlayerData[0].clear();
		PlayerData[1].clear();
		PlayerData[2].clear();
		CoinX = 0;
		CoinY = 0;
		CoinState = 0;
		Time = 99;
	}

};
//�� ���ӿ��� ������ Ŭ���̾�Ʈ�� ������ ������
//���� TCP ���


class NetworkManager
{
public:
	NetworkManager();	//���� ����
	~NetworkManager();	//���� ����
	bool connection();		//��Ʈ��ũ ����
	void sendData(ClientToServer CtS);	//���� ���� ó���κп��� �Ѱ��ִ� ������

	//ClientToServer CreateClientToServer(); //player�����͸� ������ �������ݷ� �����
	//void sendToServer(ClientToServer&);	//����

	void recvData(ServerToClient& StC); //���� ���� ó�� �κ����� �ѱ� ������
	//void setData(ServerToClient&);	//�ð� �÷��̾� ��ü ����


	void err_quit(char* msg);
	void err_display(char* msg);
	int recvn(SOCKET s, char* buf, int len, int flags);
	int init();

	SOCKET getSock(bool server=false)
	{
		if (server)
		{
			return sock;
		}
		else
		{
			return socket_self;
		}
	}

	int getPN()
	{
		return playerNum;
	}

private:

	SOCKET sock;	//���� ����
	SOCKET socket_self;	//�ڽ��� ����
	//CObject_Player** players; 	// �÷��̾� �迭 ������
	int playerNum; 	// �÷��̾� ��ȣ
	int* time;		// �ð� ������
	//Coin* c;	//���� ������
};

#endif
#define PROTOCOLS 1