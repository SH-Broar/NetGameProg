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
	int PlayerNum; 	//플레이어 번호
	int x;	//플레이어의 x좌표
	int y;	//플레이어의 y좌표
	int draw_state; // *2일때는 characterNum, 플레이어 상태
	char AttackedPlayerNum[3];//맞은 플레이어의 번호


};

struct ServerToClient
{
	ClientToServer PlayerData[3];	// 플레이어 데이터
	int CoinX;	//현재 코인의 X좌표
	int CoinY;	//현재 코인의 Y좌표
	int CoinState;	//코인의 상태 (누가 갖고 있는지)
	int Time;		//남은 시간

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
	NetworkManager(PlayerData**);	//소켓 생성
	//~NetworkManager();	//소켓 정리

	void Connect();		//네트워크 연결
	int receive_number_from_server();
	void sendData(const ClientToServer&);	//게임 로직 처리부분에서 넘겨주는 데이터
	//void Send(ClientToServer&);	//전송

	void recvData(int, ServerToClient&); //게임 로직 처리 부분으로 넘길 데이터
	void Recvn(ServerToClient*);	//데이터를 받기
	void setData(ServerToClient&);	//시간 플레이어 전체 코인
	ClientToServer CreateClientToServer();	//player데이터를 전송할 프로토콜로 만들기

	

private:

	SOCKET Socket;	//자신의 소켓
	PlayerData** players;	// 플레이어 배열 포인터
	int playerNum;	// 플레이어 번호
	int* time;	// 시간 포인터
	//Coin*;	//코인 포인터
};
