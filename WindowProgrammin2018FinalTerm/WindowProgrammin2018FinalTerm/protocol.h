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
	int PlayerNum; 	//플레이어 번호
	int x;	//플레이어의 x좌표
	int y;	//플레이어의 y좌표
	int state; // *2일때는 characterNum, 플레이어 상태
	bool AttackedPlayerNum[3];   //맞은 플레이어의 번호
	// *2일때는 ready, 다른 캐릭터에게 공격을 적중시켰는지 확인하는 플래그

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
		//수정
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
	ClientToServer PlayerData[3];	// 플레이어 데이터
	int CoinX;	//현재 코인의 X좌표
	int CoinY;	//현재 코인의 Y좌표
	int CoinState;	//코인의 상태 (누가 갖고 있는지)
	int Time;		//남은 시간

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
//인 게임에서 서버가 클라이언트에 보내는 데이터
//동기 TCP 사용


class NetworkManager
{
public:
	NetworkManager();	//소켓 생성
	~NetworkManager();	//소켓 정리
	bool connection();		//네트워크 연결
	void sendData(ClientToServer CtS);	//게임 로직 처리부분에서 넘겨주는 데이터

	//ClientToServer CreateClientToServer(); //player데이터를 전송할 프로토콜로 만들기
	//void sendToServer(ClientToServer&);	//전송

	void recvData(ServerToClient& StC); //게임 로직 처리 부분으로 넘길 데이터
	//void setData(ServerToClient&);	//시간 플레이어 전체 코인


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

	SOCKET sock;	//서버 소켓
	SOCKET socket_self;	//자신의 소켓
	//CObject_Player** players; 	// 플레이어 배열 포인터
	int playerNum; 	// 플레이어 번호
	int* time;		// 시간 포인터
	//Coin* c;	//코인 포인터
};

#endif
#define PROTOCOLS 1