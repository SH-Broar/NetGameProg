#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <stdexcept>

#define TEST_BEFORE_CLIENT_COMPLETE

#pragma pack (1)
struct ClientToServer
{
	int PlayerNum; 	//�÷��̾� ��ȣ
	int x;	//�÷��̾��� x��ǥ
	int y;	//�÷��̾��� y��ǥ
	int drawState; // *2�϶��� characterNum, �÷��̾� ����
	bool AttackedPlayerNum[3];   //���� �÷��̾��� ��ȣ
	// *2�϶��� ready, �ٸ� ĳ���Ϳ��� ������ ���߽��״��� Ȯ���ϴ� �÷���

	void set(int pn, int charNum, bool isReady)
	{
		PlayerNum = pn;
		x = 0;
		y = 0;
		drawState = charNum;

		AttackedPlayerNum[0] = isReady;
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

	void setInitForSelect()
	{
		CoinState = 1000;

		PlayerData[0].drawState = 1;
		PlayerData[1].drawState = 2;
		PlayerData[2].drawState = 3;

		PlayerData[0].AttackedPlayerNum[0] = false;
		PlayerData[1].AttackedPlayerNum[0] = false;
		PlayerData[2].AttackedPlayerNum[0] = false;

		Time = 60;
	}
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