//#include "USERINTERFACE.h"
//
////������
//USER_INTERFACE::USER_INTERFACE() {
//	
//	//=====================================
//	//         ��ġ ������
//	//=====================================
//	//���ηκ�
//	//TIP
//	R_MainLobby[TIP].top = 600;
//	R_MainLobby[TIP].left = 1600;
//	R_MainLobby[TIP].right = 250;
//	R_MainLobby[TIP].bottom = 106;
//
//	//START
//	R_MainLobby[START].top = 720;
//	R_MainLobby[START].left = 1600;
//	R_MainLobby[START].right = 250;
//	R_MainLobby[START].bottom = 106;
//
//	//EXIT
//	R_MainLobby[EXIT].top = 840;
//	R_MainLobby[EXIT].left = 1600;
//	R_MainLobby[EXIT].right = 250;
//	R_MainLobby[EXIT].bottom = 106;
//
//	//------------------------------
//
//
//
//	//===================================
//}
//
//SceneNAME USER_INTERFACE::returnScene() // ���� �� ������ ��ȯ
//{
//	return Scene;
//}
//
//void USER_INTERFACE::ALLReset()
//{
//
//}
//
//void USER_INTERFACE::nextScene() //���� ���� �ҷ��´�
//{
//	switch (Scene){
//	case TITLE:
//		Scene = MAIN_LOBBY;
//		break;
//	case MAIN_LOBBY:
//		Scene = SELECT_MODE;
//		break;
//	case SELECT_MODE:
//		Scene = SELECT_CHAR;
//		break;
//	case SELECT_CHAR:
//		Scene = INGAME;
//		break;
//	case INGAME:
//		Scene = SCOREBOARD;
//		break;
//	case SCOREBOARD:
//		Scene = TITLE;
//		break;
//	}
//}
//
////UI�� Ŭ�� �Ǿ����� üũ�մϴ�.
//bool USER_INTERFACE::checkInside(int x, int y, RECT rect) {
//	if (rect.top<y&&rect.bottom>y&&rect.left<x&&rect.right>y) {
//		return true;
//	}
//	else {
//		return false;
//	}
//}
//
////��Ʈ�� ���� ���ϴ�.
//void USER_INTERFACE::CallTip() {
//	if (Scene == MAIN_LOBBY) {
//		Scene = CONTROL_TIP;
//	}
//	else {
//		Scene = MAIN_LOBBY;
//	}	
//}
//
////UI�� �����մϴ�.
//void USER_INTERFACE::SelectUI(WPARAM wParam) {
//	//SCENE�� ���� ����� �ٸ��� �����մϴ�.
//	switch (Scene) {
//	case MAIN_LOBBY:
//		switch (wParam) {
//		case VK_UP:
//			switch (ML_Choice) {
//			case TIP:
//				ML_Choice = START;
//				break;
//			case START:
//				ML_Choice = EXIT;
//				break;
//			case EXIT:
//				ML_Choice = TIP;
//				break;
//			}		
//			break;
//		case VK_DOWN:
//			switch (ML_Choice) {
//			case TIP:
//				ML_Choice = EXIT;
//				break;
//			case START:
//				ML_Choice = TIP;
//				break;
//			case EXIT:
//				ML_Choice = START;
//				break;
//			}
//			break;
//		}
//		break;
//	case SELECT_MODE:
//		break;
//
//	}
//}
//
//int USER_INTERFACE::ReturnChoice() {
//	switch (Scene)
//	{
//	case MAIN_LOBBY:
//		return ML_Choice;
//		break;
//	case SELECT_MODE:
//		return 1;
//		break;
//	}
//}
//
//RECT USER_INTERFACE::GetRect(int choice) {
//	switch (Scene)
//	{
//	case MAIN_LOBBY:
//		return R_MainLobby[choice];
//		break;
//	case SELECT_MODE:
//		break;
//	}
//
//
//}