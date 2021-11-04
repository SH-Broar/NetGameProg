#ifndef PROTOCOLS 1
#include <WinSock2.h>
#include <Windows.h>

#define SERVERIP		"127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

struct ClientToServer
{
	int x;	//�÷��̾��� x��ǥ
	int y;	//�÷��̾��� y��ǥ
	int DrawState; // *2�϶��� characterNum, �÷��̾� ����
	bool attack;   // *2�϶��� ready, �ٸ� ĳ���Ϳ��� ������ ���߽��״��� Ȯ���ϴ� �÷���
};

struct ServerToClient
{
	int EnemyX;	//����� x��ǥ
	int EnemyY;	//����� y��ǥ
	int EnemyDrawState; // *2�϶��� characterNum, ����� ����
	bool attacked;	// *2�϶��� ready, �ǰ� ���ߴ��� �˷��ִ� �÷���
	int CoinX;	//���� ������ X��ǥ
	int CoinY;	//���� ������ Y��ǥ
	int CoinState;	//������ ���� (���� ���� �ִ���)
	int Time; 	//���� �ð�
};
//�� ���ӿ��� ������ Ŭ���̾�Ʈ�� ������ ������
//���� TCP ���

class NetworkManager
{
public:

	NetworkManager();	//���� ����
	~NetworkManager();	//���� ����
	void connection();		//��Ʈ��ũ ����
	void sendData(const ClientToServer);	//���� ���� ó���κп��� �Ѱ��ִ� ������
	void recvData(int, ServerToClient); //���� ���� ó�� �κ����� �ѱ� ������

	void err_quit(char* msg);
	void err_display(char* msg);
	int recvn(SOCKET s, char* buf, int len, int flags);
	int init();

	SOCKET sock;	//���� ����

private:
	SOCKET sock_self;	//�ڽ��� ����

};
#endif
#define PROTOCOLS 1