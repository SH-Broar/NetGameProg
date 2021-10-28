#include <Windows.h>
#include <iostream>
//==========================================================
//               USERINTERFACE

//유저 인터페이스의 클래스 선언과 기타 사항 정의입니다.


//==========================================================

#define MainLobbyCount 3
#define SelectMode 5


//SCENE의 위치를 선언해놓은 열거형 입니다.
enum SceneNAME {
	TITLE = 0, MAIN_LOBBY = 1, CONTROL_TIP = 2, SELECT_MODE = 3,
	SELECT_CHAR = 4, INGAME = 5, SCOREBOARD = 6
};

enum MainLobby {
	TIP = 0, START = 1, EXIT = 2
};



class USER_INTERFACE {
private:
	//타이틀과 관련된 데이터 필드입니다.
	//초기 값으로는 타이틀 신을 가집니다.
	//SceneNAME Scene = TITLE;
	SceneNAME Scene = INGAME;
	bool gameState = false;

	RECT R_MainLobby[MainLobbyCount];
	MainLobby ML_Choice = TIP;

public:
	USER_INTERFACE();
	//현재 신을 리턴합니다.
	SceneNAME returnScene();
	
	//신의 정보를 모두 초기화 합니다.
	void ALLReset();

	//기본상태에서 다음 씬을 소환합니다.
	void nextScene();

	//UI가 클릭 되었는지 체크합니다.
	bool checkInside(int x, int y, RECT rect);

	//컨트롤 팁을 띄웁니다.
	void CallTip();

	
	//UI를 선택하고 반응합니다.
	void SelectUI(WPARAM wParam);

	int ReturnChoice();

	RECT GetRect(int choice);

};