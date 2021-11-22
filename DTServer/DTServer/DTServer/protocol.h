#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <chrono>

#define TEST_BEFORE_CLIENT_COMPLETE
//#define TEST_TEST

struct ClientToServer
{
	int playerNum;
	int x;	//플레이어의 x좌표
	int y;	//플레이어의 y좌표
	int drawState; // *2일때는 characterNum, 플레이어 상태
	char AttackedPlayerNum[3];   // *2일때는 ready, 다른 캐릭터에게 공격을 적중시켰는지 확인하는 플래그
#define ready AttackedPlayerNum
#define characterNum drawState
};

struct ServerToClient
{
	ClientToServer PlayerData[3];
	int CoinX;	//현재 코인의 X좌표
	int CoinY;	//현재 코인의 Y좌표
	int CoinState;	//코인의 상태 (누가 갖고 있는지)
	int Time; 	//남은 시간

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

extern HANDLE hReceiveEvent[3];
extern HANDLE hReadCompeleteEvent;





//인 게임에서 서버가 클라이언트에 보내는 데이터
//동기 TCP 사용
/*
//서버 네트워크 클래스;
class PlayerNetworkManager
{
	PlayerNetworkManager();	//소켓 생성
	~PlayerNetworkManager();	//소켓 정리

	static void recvData(); // 현재 클라이언트로부터 데이터 받기 (<-socket)
	static void sendData(); // 타 클라이언트로 데이터 보내기	(->othersocket)
	const ClientToServer getData(); // 게임 로직에서 Data를 사용하기 위해 호출

private:
	SOCKET socket;	//현재 클래스가 가리키는 플레이어의 소켓
	SOCKET othersocket;	//현재 클래스가 가리키지 않는 플레이어의 소켓
	ClientToServer Data;	//받은 플레이어 데이터 저장
};

*/