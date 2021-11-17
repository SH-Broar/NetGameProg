#ifndef PROTOCOLS 1
#include <WinSock2.h>
#include <Windows.h>

#define SERVERIP		"127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

struct ClientToServer
{
	int PlayerNum; 	//�÷��̾� ��ȣ
	int x;	//�÷��̾��� x��ǥ
	int y;	//�÷��̾��� y��ǥ
	int state; // *2�϶��� characterNum, �÷��̾� ����
	char AttackedPlayerNum[3];   //���� �÷��̾��� ��ȣ
	// *2�϶��� ready, �ٸ� ĳ���Ϳ��� ������ ���߽��״��� Ȯ���ϴ� �÷���
};

struct ServerToClient
{
	ClientToServer PlayerData[3];	// �÷��̾� ������
	int CoinX;	//���� ������ X��ǥ
	int CoinY;	//���� ������ Y��ǥ
	int CoinState;	//������ ���� (���� ���� �ִ���)
	int Time;		//���� �ð�
};
//�� ���ӿ��� ������ Ŭ���̾�Ʈ�� ������ ������
//���� TCP ���


class NetworkManager
{
public:
	NetworkManager();	//���� ����
	~NetworkManager();	//���� ����
	bool connection();		//��Ʈ��ũ ����
	void sendData(const ClientToServer CtS);	//���� ���� ó���κп��� �Ѱ��ִ� ������

	ClientToServer CreateClientToServer(); //player�����͸� ������ �������ݷ� �����
	void sendToServer(ClientToServer&);	//����

	void recvData(int Scene, ServerToClient& StC); //���� ���� ó�� �κ����� �ѱ� ������
	void setData(ServerToClient&);	//�ð� �÷��̾� ��ü ����


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