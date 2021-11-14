#pragma comment(lib, "ws2_32")
#include <iostream>
#include <vector>
#include "protocol.h"





void SetCursorPosition(int y)
{
	COORD pos = { 0,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}


HANDLE send_and_recieve_data_from_server_Event;
HANDLE game_loop_Event;
CRITICAL_SECTION cs;

PlayerData player_data[3];
PlayerData* p = player_data;
NetworkManager* network_manager_pointer;
ClientToServer client_to_server_data;

int main()
{
	


	int retval;
	
	network_manager_pointer = new NetworkManager(&p);

	send_and_recieve_data_from_server_Event = CreateEvent(NULL, FALSE, TRUE, NULL);
	game_loop_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	network_manager_pointer->Connect();
	
	if (network_manager_pointer->receive_number_from_server() != 0)
		return  -1;

	
	int n = 0;
	while (1) {
		WaitForSingleObject(send_and_recieve_data_from_server_Event, INFINITE);

		Sleep(1000);
		printf("gameloop\n");
		
		client_to_server_data.x = n++;
		client_to_server_data.y = n++;
		client_to_server_data.draw_state = n++;
		client_to_server_data.PlayerNum = 2;
		client_to_server_data.AttackedPlayerNum[0] = 1;
		client_to_server_data.AttackedPlayerNum[1] = 3;


		SetEvent(game_loop_Event);


	}

	WSACleanup();
	return 0;

}

DWORD WINAPI client_thread(LPVOID arg) {

	printf("thread\n");

	int retval;

	SOCKET sock = (SOCKET)arg;

	int n = 0;
	while (1) {

		WaitForSingleObject(game_loop_Event, INFINITE);

		network_manager_pointer->sendData(client_to_server_data);

		printf("internet_Thread (%d)\n", n++);

		SetEvent(send_and_recieve_data_from_server_Event);
	}
	
}