#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <stdexcept>

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
/*
//���� ��Ʈ��ũ Ŭ����;
class PlayerNetworkManager
{
	PlayerNetworkManager();	//���� ����
	~PlayerNetworkManager();	//���� ����

	static void recvData(); // ���� Ŭ���̾�Ʈ�κ��� ������ �ޱ� (<-socket)
	static void sendData(); // Ÿ Ŭ���̾�Ʈ�� ������ ������	(->othersocket)
	const ClientToServer getData(); // ���� �������� Data�� ����ϱ� ���� ȣ��

private:
	SOCKET socket;	//���� Ŭ������ ����Ű�� �÷��̾��� ����
	SOCKET othersocket;	//���� Ŭ������ ����Ű�� �ʴ� �÷��̾��� ����
	ClientToServer Data;	//���� �÷��̾� ������ ����
};

*/