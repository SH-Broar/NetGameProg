#pragma comment(lib, "ws2_32")
#include <iostream>
#include <vector>
#include "protocol.h"
#include "networkmanager.h"





HANDLE send_and_recieve_data_from_server_Event;
HANDLE game_loop_Event;
CRITICAL_SECTION cs;

ClientToServer player_data[3];

NetworkManager* network_manager_pointer;
ClientToServer client_to_server_data;

int main()
{

	int retval;
	
	ClientToServer* p = &player_data[0];
	network_manager_pointer = new NetworkManager(p);

	send_and_recieve_data_from_server_Event = CreateEvent(NULL, FALSE, TRUE, NULL);
	game_loop_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	
	network_manager_pointer->Connect();
	
	if (network_manager_pointer->receive_number_from_server() != 0)
		return  -1;

	network_manager_pointer->print_my_player_number();
	
	int n = 0;
	while (1) {
		WaitForSingleObject(send_and_recieve_data_from_server_Event, INFINITE);

		Sleep(3000);
		printf("gameloop\n");
		
		

		client_to_server_data.x = n++;
		client_to_server_data.y = n++;
		client_to_server_data.draw_state = n++;
		client_to_server_data.PlayerNum = network_manager_pointer->get_playerNum();
		client_to_server_data.AttackedPlayerNum[0] = 1;
		client_to_server_data.AttackedPlayerNum[1] = 3;


		SetEvent(game_loop_Event);


	}

	
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

		printf("[%d send,", n++);

		network_manager_pointer->recvData();

		printf("receive ] playerNum:%d, x:%d, y:%d, draw:%d\n" ,player_data[0].PlayerNum, player_data[0].x, player_data[0].y, player_data[0].draw_state);

		SetEvent(send_and_recieve_data_from_server_Event);
	}
	
}