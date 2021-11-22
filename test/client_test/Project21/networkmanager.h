#pragma once
#include "protocol.h"

class NetworkManager {
public:
	NetworkManager(ClientToServer*);	//���� ����
	~NetworkManager();	//���� ����

	void Connect();		//��Ʈ��ũ ����
	int receive_number_from_server();
	void sendData(const ClientToServer&);	//���� ���� ó���κп��� �Ѱ��ִ� ������
	//void Send(ClientToServer&);	//����

	void recvData(); //���� ���� ó�� �κ����� �ѱ� ������
	void Recvn(ServerToClient*);	//�����͸� �ޱ�
	void setData(ServerToClient&);	//�ð� �÷��̾� ��ü ����
	ClientToServer CreateClientToServer();	//player�����͸� ������ �������ݷ� �����

	void print_my_player_number();
	int get_playerNum();

private:

	WSADATA wsa;
	SOCKET Socket;	//�ڽ��� ����
	ClientToServer* players;	// �÷��̾� �迭 ������
	int playerNum;	// �÷��̾� ��ȣ
	int* time;	// �ð� ������
	//Coin*;	//���� ������
};