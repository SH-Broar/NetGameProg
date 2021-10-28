#include <Windows.h>
#include <iostream>
//==========================================================
//               USERINTERFACE

//���� �������̽��� Ŭ���� ����� ��Ÿ ���� �����Դϴ�.


//==========================================================

#define MainLobbyCount 3
#define SelectMode 5


//SCENE�� ��ġ�� �����س��� ������ �Դϴ�.
enum SceneNAME {
	TITLE = 0, MAIN_LOBBY = 1, CONTROL_TIP = 2, SELECT_MODE = 3,
	SELECT_CHAR = 4, INGAME = 5, SCOREBOARD = 6
};

enum MainLobby {
	TIP = 0, START = 1, EXIT = 2
};



class USER_INTERFACE {
private:
	//Ÿ��Ʋ�� ���õ� ������ �ʵ��Դϴ�.
	//�ʱ� �����δ� Ÿ��Ʋ ���� �����ϴ�.
	//SceneNAME Scene = TITLE;
	SceneNAME Scene = INGAME;
	bool gameState = false;

	RECT R_MainLobby[MainLobbyCount];
	MainLobby ML_Choice = TIP;

public:
	USER_INTERFACE();
	//���� ���� �����մϴ�.
	SceneNAME returnScene();
	
	//���� ������ ��� �ʱ�ȭ �մϴ�.
	void ALLReset();

	//�⺻���¿��� ���� ���� ��ȯ�մϴ�.
	void nextScene();

	//UI�� Ŭ�� �Ǿ����� üũ�մϴ�.
	bool checkInside(int x, int y, RECT rect);

	//��Ʈ�� ���� ���ϴ�.
	void CallTip();

	
	//UI�� �����ϰ� �����մϴ�.
	void SelectUI(WPARAM wParam);

	int ReturnChoice();

	RECT GetRect(int choice);

};