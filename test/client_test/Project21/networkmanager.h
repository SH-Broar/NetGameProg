#pragma once
#include "protocol.h"

class NetworkManager {
public:
	NetworkManager(ClientToServer*);	//소켓 생성
	~NetworkManager();	//소켓 정리

	void Connect();		//네트워크 연결
	int receive_number_from_server();
	void sendData(const ClientToServer&);	//게임 로직 처리부분에서 넘겨주는 데이터
	//void Send(ClientToServer&);	//전송

	void recvData(); //게임 로직 처리 부분으로 넘길 데이터
	void Recvn(ServerToClient*);	//데이터를 받기
	void setData(ServerToClient&);	//시간 플레이어 전체 코인
	ClientToServer CreateClientToServer();	//player데이터를 전송할 프로토콜로 만들기

	void print_my_player_number();
	int get_playerNum();

private:

	WSADATA wsa;
	SOCKET Socket;	//자신의 소켓
	ClientToServer* players;	// 플레이어 배열 포인터
	int playerNum;	// 플레이어 번호
	int* time;	// 시간 포인터
	//Coin*;	//코인 포인터
};