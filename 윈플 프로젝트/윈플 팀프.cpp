#include"resource1.h"
#include "Block.h"
//#include <fmod.h>
random_device dis;
mt19937 gen(dis());
uniform_int_distribution<int>CreateBlockNum(0, 8);
uniform_int_distribution<int>CreateDaggarNum(200, 600);

HINSTANCE g_hinst;
LPCTSTR IpszClass = L"Window Class Name";
LPCTSTR IpszWindowName = L"Break Down";

enum class state {
	On, Off
};

enum class drawState {
	Stand, Sit, Crawl, Run, Jump, Fall
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM IParam);

typedef struct _Player {
	CImage Crawl[6];
	CImage Crawl_R[6];
	CImage Stand[32];
	CImage Stand_R[32];
	CImage Jumpdown[5];
	CImage Jumpdown_R[5];
	CImage Run[8];
	CImage Run_R[8];
	CImage Sit[13];
	CImage Sit_R[13];
	CImage Tumbling[12];
	CImage Tumbling_R[12];
	BOOL isPupple;
	RECT Box;
	BOOL isLeft;
	POINT Position;
	float Accelerate;
	float Speed;
	state State;
	drawState DS;
	int CrawlCount;
	int StandCount;
	int JumpdownCount;
	int RunCount;
	int SitCount;
	int TumblingCount;
	int HowJumpCount;
	int OnBoardNum;
}Player;

typedef struct _Number {
	int NowNormal;
	int NowPupple;
	int NowYellow;
	int NowBreak;
	int NowThorn;
	int NowDaggar;
}Number;

typedef struct _BackgroundInfor {
	CImage Image;
	int yPos;
}BackgroundInfor;

typedef struct _SystemInfor {
	CImage GameoverImage;
	CImage PauseImage[3];
	CImage PauseButton;
	CImage RetryButton;
	CImage GameClearImage;
	RECT PauseButtonPos;
	RECT RetryPos;
	BOOL GameOver;
	BOOL canSkill;
	BOOL isStart;
	BOOL Reset;
	BOOL Clear;
}SystemInfor;

Player PlayerPupple;
Player PlayerYellow;
BackgroundInfor Background[4];
Block* Blocks[BlockNum];
Block* Wall[WallBlockNum];
RECT Client;
RECT BothCrushCheck;
RECT Circle;
BOOL WhatKey[256];
BOOL isSpacePupple;
SystemInfor System;
Number CountNum;
int BackgroundCheck;
int CreateBlockCount = 0;
int StoreNum[9];
int GameOverCount = 0;
int ShakeCount = 0;
int PauseCount = 0;
int TargetCount = 0;

//FMOD_SYSTEM* soundSystem;
//FMOD_SOUND* bgmsound[3];
//FMOD_SOUND* effectSound;
//FMOD_CHANNEL* Channel[10];

TCHAR StoreScore[100];
int Score;

void PuppleRepotision();
void YellowRepotision();
void BlockUpdate();
void Loop();
void Position_Compute(BOOL);
void CreateBlockPosition();
void CheckNow();
void ThornChange();
void End(HWND);
BOOL ClientCrush();
BOOL BothCrush();
BOOL Pupple_BlockCrush(int = -1);
BOOL Yellow_BlockCrush(int = -1);
int Pupple_WallCrush();
int Yellow_WallCrush();
void Skill();
void ShakeMap(int);
void _Reset(HWND);
void Sound_Setup();

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR IpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hinst = hinstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hinstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.lpszClassName = IpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(IpszClass, IpszWindowName, WS_SYSMENU
		, 0, 0, Window_XSize, Window_YSize, NULL, (HMENU)NULL, hinstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM IParam)
{
	PAINTSTRUCT ps;
	HDC hDC, mem1DC, mem2DC;
	static HBITMAP FinalBit, hBit;
	HBITMAP oldBit, oldBit2;
	HPEN oldPen, hPen;

	static int mx, my;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		{
			PlayerPupple.Crawl[0].Load(TEXT("Resource/Player_Purple/Crawl_0.png"));
			PlayerPupple.Crawl[1].Load(TEXT("Resource/Player_Purple/Crawl_1.png"));
			PlayerPupple.Crawl[2].Load(TEXT("Resource/Player_Purple/Crawl_2.png"));
			PlayerPupple.Crawl[3].Load(TEXT("Resource/Player_Purple/Crawl_3.png"));
			PlayerPupple.Crawl[4].Load(TEXT("Resource/Player_Purple/Crawl_4.png"));
			PlayerPupple.Crawl[5].Load(TEXT("Resource/Player_Purple/Crawl_5.png"));

			PlayerPupple.Crawl_R[0].Load(TEXT("Resource/Player_Purple - Right/Crawl_0.png"));
			PlayerPupple.Crawl_R[1].Load(TEXT("Resource/Player_Purple - Right/Crawl_1.png"));
			PlayerPupple.Crawl_R[2].Load(TEXT("Resource/Player_Purple - Right/Crawl_2.png"));
			PlayerPupple.Crawl_R[3].Load(TEXT("Resource/Player_Purple - Right/Crawl_3.png"));
			PlayerPupple.Crawl_R[4].Load(TEXT("Resource/Player_Purple - Right/Crawl_4.png"));
			PlayerPupple.Crawl_R[5].Load(TEXT("Resource/Player_Purple - Right/Crawl_5.png"));

			PlayerPupple.Stand[0].Load(TEXT("Resource/Player_Purple/idle_0.png"));
			PlayerPupple.Stand[1].Load(TEXT("Resource/Player_Purple/idle_1.png"));
			PlayerPupple.Stand[2].Load(TEXT("Resource/Player_Purple/idle_2.png"));
			PlayerPupple.Stand[3].Load(TEXT("Resource/Player_Purple/idle_3.png"));
			PlayerPupple.Stand[4].Load(TEXT("Resource/Player_Purple/idle_4.png"));
			PlayerPupple.Stand[5].Load(TEXT("Resource/Player_Purple/idle_5.png"));
			PlayerPupple.Stand[6].Load(TEXT("Resource/Player_Purple/idle_6.png"));
			PlayerPupple.Stand[7].Load(TEXT("Resource/Player_Purple/idle_7.png"));
			PlayerPupple.Stand[8].Load(TEXT("Resource/Player_Purple/idle_8.png"));
			PlayerPupple.Stand[9].Load(TEXT("Resource/Player_Purple/idle_9.png"));
			PlayerPupple.Stand[10].Load(TEXT("Resource/Player_Purple/idle_10.png"));
			PlayerPupple.Stand[11].Load(TEXT("Resource/Player_Purple/idle_11.png"));
			PlayerPupple.Stand[12].Load(TEXT("Resource/Player_Purple/idle_12.png"));
			PlayerPupple.Stand[13].Load(TEXT("Resource/Player_Purple/idle_13.png"));
			PlayerPupple.Stand[14].Load(TEXT("Resource/Player_Purple/idle_14.png"));
			PlayerPupple.Stand[15].Load(TEXT("Resource/Player_Purple/idle_15.png"));
			PlayerPupple.Stand[16].Load(TEXT("Resource/Player_Purple/idle_16.png"));
			PlayerPupple.Stand[17].Load(TEXT("Resource/Player_Purple/idle_17.png"));
			PlayerPupple.Stand[18].Load(TEXT("Resource/Player_Purple/idle_18.png"));
			PlayerPupple.Stand[19].Load(TEXT("Resource/Player_Purple/idle_19.png"));
			PlayerPupple.Stand[20].Load(TEXT("Resource/Player_Purple/idle_20.png"));
			PlayerPupple.Stand[21].Load(TEXT("Resource/Player_Purple/idle_21.png"));
			PlayerPupple.Stand[22].Load(TEXT("Resource/Player_Purple/idle_22.png"));
			PlayerPupple.Stand[23].Load(TEXT("Resource/Player_Purple/idle_23.png"));
			PlayerPupple.Stand[24].Load(TEXT("Resource/Player_Purple/idle_24.png"));
			PlayerPupple.Stand[25].Load(TEXT("Resource/Player_Purple/idle_25.png"));
			PlayerPupple.Stand[26].Load(TEXT("Resource/Player_Purple/idle_26.png"));
			PlayerPupple.Stand[27].Load(TEXT("Resource/Player_Purple/idle_27.png"));
			PlayerPupple.Stand[28].Load(TEXT("Resource/Player_Purple/idle_28.png"));
			PlayerPupple.Stand[29].Load(TEXT("Resource/Player_Purple/idle_29.png"));
			PlayerPupple.Stand[30].Load(TEXT("Resource/Player_Purple/idle_30.png"));
			PlayerPupple.Stand[31].Load(TEXT("Resource/Player_Purple/idle_31.png"));

			PlayerPupple.Stand_R[0].Load(TEXT("Resource/Player_Purple - Right/idle_0.png"));
			PlayerPupple.Stand_R[1].Load(TEXT("Resource/Player_Purple - Right/idle_1.png"));
			PlayerPupple.Stand_R[2].Load(TEXT("Resource/Player_Purple - Right/idle_2.png"));
			PlayerPupple.Stand_R[3].Load(TEXT("Resource/Player_Purple - Right/idle_3.png"));
			PlayerPupple.Stand_R[4].Load(TEXT("Resource/Player_Purple - Right/idle_4.png"));
			PlayerPupple.Stand_R[5].Load(TEXT("Resource/Player_Purple - Right/idle_5.png"));
			PlayerPupple.Stand_R[6].Load(TEXT("Resource/Player_Purple - Right/idle_6.png"));
			PlayerPupple.Stand_R[7].Load(TEXT("Resource/Player_Purple - Right/idle_7.png"));
			PlayerPupple.Stand_R[8].Load(TEXT("Resource/Player_Purple - Right/idle_8.png"));
			PlayerPupple.Stand_R[9].Load(TEXT("Resource/Player_Purple - Right/idle_9.png"));
			PlayerPupple.Stand_R[10].Load(TEXT("Resource/Player_Purple - Right/idle_10.png"));
			PlayerPupple.Stand_R[11].Load(TEXT("Resource/Player_Purple - Right/idle_11.png"));
			PlayerPupple.Stand_R[12].Load(TEXT("Resource/Player_Purple - Right/idle_12.png"));
			PlayerPupple.Stand_R[13].Load(TEXT("Resource/Player_Purple - Right/idle_13.png"));
			PlayerPupple.Stand_R[14].Load(TEXT("Resource/Player_Purple - Right/idle_14.png"));
			PlayerPupple.Stand_R[15].Load(TEXT("Resource/Player_Purple - Right/idle_15.png"));
			PlayerPupple.Stand_R[16].Load(TEXT("Resource/Player_Purple - Right/idle_16.png"));
			PlayerPupple.Stand_R[17].Load(TEXT("Resource/Player_Purple - Right/idle_17.png"));
			PlayerPupple.Stand_R[18].Load(TEXT("Resource/Player_Purple - Right/idle_18.png"));
			PlayerPupple.Stand_R[19].Load(TEXT("Resource/Player_Purple - Right/idle_19.png"));
			PlayerPupple.Stand_R[20].Load(TEXT("Resource/Player_Purple - Right/idle_20.png"));
			PlayerPupple.Stand_R[21].Load(TEXT("Resource/Player_Purple - Right/idle_21.png"));
			PlayerPupple.Stand_R[22].Load(TEXT("Resource/Player_Purple - Right/idle_22.png"));
			PlayerPupple.Stand_R[23].Load(TEXT("Resource/Player_Purple - Right/idle_23.png"));
			PlayerPupple.Stand_R[24].Load(TEXT("Resource/Player_Purple - Right/idle_24.png"));
			PlayerPupple.Stand_R[25].Load(TEXT("Resource/Player_Purple - Right/idle_25.png"));
			PlayerPupple.Stand_R[26].Load(TEXT("Resource/Player_Purple - Right/idle_26.png"));
			PlayerPupple.Stand_R[27].Load(TEXT("Resource/Player_Purple - Right/idle_27.png"));
			PlayerPupple.Stand_R[28].Load(TEXT("Resource/Player_Purple - Right/idle_28.png"));
			PlayerPupple.Stand_R[29].Load(TEXT("Resource/Player_Purple - Right/idle_29.png"));
			PlayerPupple.Stand_R[30].Load(TEXT("Resource/Player_Purple - Right/idle_30.png"));
			PlayerPupple.Stand_R[31].Load(TEXT("Resource/Player_Purple - Right/idle_31.png"));

			PlayerPupple.Run[0].Load(TEXT("Resource/Player_Purple/Run_0.png"));
			PlayerPupple.Run[1].Load(TEXT("Resource/Player_Purple/Run_1.png"));
			PlayerPupple.Run[2].Load(TEXT("Resource/Player_Purple/Run_2.png"));
			PlayerPupple.Run[3].Load(TEXT("Resource/Player_Purple/Run_3.png"));
			PlayerPupple.Run[4].Load(TEXT("Resource/Player_Purple/Run_4.png"));
			PlayerPupple.Run[5].Load(TEXT("Resource/Player_Purple/Run_5.png"));
			PlayerPupple.Run[6].Load(TEXT("Resource/Player_Purple/Run_6.png"));
			PlayerPupple.Run[7].Load(TEXT("Resource/Player_Purple/Run_7.png"));

			PlayerPupple.Run_R[0].Load(TEXT("Resource/Player_Purple - Right/Run_0.png"));
			PlayerPupple.Run_R[1].Load(TEXT("Resource/Player_Purple - Right/Run_1.png"));
			PlayerPupple.Run_R[2].Load(TEXT("Resource/Player_Purple - Right/Run_2.png"));
			PlayerPupple.Run_R[3].Load(TEXT("Resource/Player_Purple - Right/Run_3.png"));
			PlayerPupple.Run_R[4].Load(TEXT("Resource/Player_Purple - Right/Run_4.png"));
			PlayerPupple.Run_R[5].Load(TEXT("Resource/Player_Purple - Right/Run_5.png"));
			PlayerPupple.Run_R[6].Load(TEXT("Resource/Player_Purple - Right/Run_6.png"));
			PlayerPupple.Run_R[7].Load(TEXT("Resource/Player_Purple - Right/Run_7.png"));

			PlayerPupple.Sit[0].Load(TEXT("Resource/Player_Purple/Sit_0.png"));
			PlayerPupple.Sit[1].Load(TEXT("Resource/Player_Purple/Sit_1.png"));
			PlayerPupple.Sit[2].Load(TEXT("Resource/Player_Purple/Sit_2.png"));
			PlayerPupple.Sit[3].Load(TEXT("Resource/Player_Purple/Sit_3.png"));
			PlayerPupple.Sit[4].Load(TEXT("Resource/Player_Purple/Sit_4.png"));
			PlayerPupple.Sit[5].Load(TEXT("Resource/Player_Purple/Sit_5.png"));
			PlayerPupple.Sit[6].Load(TEXT("Resource/Player_Purple/Sit_6.png"));
			PlayerPupple.Sit[7].Load(TEXT("Resource/Player_Purple/Sit_7.png"));
			PlayerPupple.Sit[8].Load(TEXT("Resource/Player_Purple/Sit_8.png"));
			PlayerPupple.Sit[9].Load(TEXT("Resource/Player_Purple/Sit_9.png"));
			PlayerPupple.Sit[10].Load(TEXT("Resource/Player_Purple/Sit_10.png"));
			PlayerPupple.Sit[11].Load(TEXT("Resource/Player_Purple/Sit_11.png"));
			PlayerPupple.Sit[12].Load(TEXT("Resource/Player_Purple/Sit_12.png"));

			PlayerPupple.Sit_R[0].Load(TEXT("Resource/Player_Purple - Right/Sit_0.png"));
			PlayerPupple.Sit_R[1].Load(TEXT("Resource/Player_Purple - Right/Sit_1.png"));
			PlayerPupple.Sit_R[2].Load(TEXT("Resource/Player_Purple - Right/Sit_2.png"));
			PlayerPupple.Sit_R[3].Load(TEXT("Resource/Player_Purple - Right/Sit_3.png"));
			PlayerPupple.Sit_R[4].Load(TEXT("Resource/Player_Purple - Right/Sit_4.png"));
			PlayerPupple.Sit_R[5].Load(TEXT("Resource/Player_Purple - Right/Sit_5.png"));
			PlayerPupple.Sit_R[6].Load(TEXT("Resource/Player_Purple - Right/Sit_6.png"));
			PlayerPupple.Sit_R[7].Load(TEXT("Resource/Player_Purple - Right/Sit_7.png"));
			PlayerPupple.Sit_R[8].Load(TEXT("Resource/Player_Purple - Right/Sit_8.png"));
			PlayerPupple.Sit_R[9].Load(TEXT("Resource/Player_Purple - Right/Sit_9.png"));
			PlayerPupple.Sit_R[10].Load(TEXT("Resource/Player_Purple - Right/Sit_10.png"));
			PlayerPupple.Sit_R[11].Load(TEXT("Resource/Player_Purple - Right/Sit_11.png"));
			PlayerPupple.Sit_R[12].Load(TEXT("Resource/Player_Purple - Right/Sit_12.png"));

			PlayerPupple.Tumbling[0].Load(TEXT("Resource/Player_Purple/tumbling_0.png"));
			PlayerPupple.Tumbling[1].Load(TEXT("Resource/Player_Purple/tumbling_1.png"));
			PlayerPupple.Tumbling[2].Load(TEXT("Resource/Player_Purple/tumbling_2.png"));
			PlayerPupple.Tumbling[3].Load(TEXT("Resource/Player_Purple/tumbling_3.png"));
			PlayerPupple.Tumbling[4].Load(TEXT("Resource/Player_Purple/tumbling_4.png"));
			PlayerPupple.Tumbling[5].Load(TEXT("Resource/Player_Purple/tumbling_5.png"));
			PlayerPupple.Tumbling[6].Load(TEXT("Resource/Player_Purple/tumbling_6.png"));
			PlayerPupple.Tumbling[7].Load(TEXT("Resource/Player_Purple/tumbling_7.png"));
			PlayerPupple.Tumbling[8].Load(TEXT("Resource/Player_Purple/tumbling_8.png"));
			PlayerPupple.Tumbling[9].Load(TEXT("Resource/Player_Purple/tumbling_9.png"));
			PlayerPupple.Tumbling[10].Load(TEXT("Resource/Player_Purple/tumbling_10.png"));
			PlayerPupple.Tumbling[11].Load(TEXT("Resource/Player_Purple/tumbling_11.png"));

			PlayerPupple.Tumbling_R[0].Load(TEXT("Resource/Player_Purple - Right/tumbling_0.png"));
			PlayerPupple.Tumbling_R[1].Load(TEXT("Resource/Player_Purple - Right/tumbling_1.png"));
			PlayerPupple.Tumbling_R[2].Load(TEXT("Resource/Player_Purple - Right/tumbling_2.png"));
			PlayerPupple.Tumbling_R[3].Load(TEXT("Resource/Player_Purple - Right/tumbling_3.png"));
			PlayerPupple.Tumbling_R[4].Load(TEXT("Resource/Player_Purple - Right/tumbling_4.png"));
			PlayerPupple.Tumbling_R[5].Load(TEXT("Resource/Player_Purple - Right/tumbling_5.png"));
			PlayerPupple.Tumbling_R[6].Load(TEXT("Resource/Player_Purple - Right/tumbling_6.png"));
			PlayerPupple.Tumbling_R[7].Load(TEXT("Resource/Player_Purple - Right/tumbling_7.png"));
			PlayerPupple.Tumbling_R[8].Load(TEXT("Resource/Player_Purple - Right/tumbling_8.png"));
			PlayerPupple.Tumbling_R[9].Load(TEXT("Resource/Player_Purple - Right/tumbling_9.png"));
			PlayerPupple.Tumbling_R[10].Load(TEXT("Resource/Player_Purple - Right/tumbling_10.png"));
			PlayerPupple.Tumbling_R[11].Load(TEXT("Resource/Player_Purple - Right/tumbling_11.png"));

			PlayerPupple.Jumpdown[0].Load(TEXT("Resource/Player_Purple/Jump_down_0.png"));
			PlayerPupple.Jumpdown[1].Load(TEXT("Resource/Player_Purple/Jump_down_1.png"));
			PlayerPupple.Jumpdown[2].Load(TEXT("Resource/Player_Purple/Jump_down_2.png"));
			PlayerPupple.Jumpdown[3].Load(TEXT("Resource/Player_Purple/Jump_down_3.png"));
			PlayerPupple.Jumpdown[4].Load(TEXT("Resource/Player_Purple/Jump_down_4.png"));

			PlayerPupple.Jumpdown_R[0].Load(TEXT("Resource/Player_Purple - Right/Jump_down_0.png"));
			PlayerPupple.Jumpdown_R[1].Load(TEXT("Resource/Player_Purple - Right/Jump_down_1.png"));
			PlayerPupple.Jumpdown_R[2].Load(TEXT("Resource/Player_Purple - Right/Jump_down_2.png"));
			PlayerPupple.Jumpdown_R[3].Load(TEXT("Resource/Player_Purple - Right/Jump_down_3.png"));
			PlayerPupple.Jumpdown_R[4].Load(TEXT("Resource/Player_Purple - Right/Jump_down_4.png"));

			//===================================================================================================================

			PlayerYellow.Crawl[0].Load(TEXT("Resource/Player_Yellow/Crawl_0.png"));
			PlayerYellow.Crawl[1].Load(TEXT("Resource/Player_Yellow/Crawl_1.png"));
			PlayerYellow.Crawl[2].Load(TEXT("Resource/Player_Yellow/Crawl_2.png"));
			PlayerYellow.Crawl[3].Load(TEXT("Resource/Player_Yellow/Crawl_3.png"));
			PlayerYellow.Crawl[4].Load(TEXT("Resource/Player_Yellow/Crawl_4.png"));
			PlayerYellow.Crawl[5].Load(TEXT("Resource/Player_Yellow/Crawl_5.png"));

			PlayerYellow.Crawl_R[0].Load(TEXT("Resource/Player_Yellow - Right/Crawl_0.png"));
			PlayerYellow.Crawl_R[1].Load(TEXT("Resource/Player_Yellow - Right/Crawl_1.png"));
			PlayerYellow.Crawl_R[2].Load(TEXT("Resource/Player_Yellow - Right/Crawl_2.png"));
			PlayerYellow.Crawl_R[3].Load(TEXT("Resource/Player_Yellow - Right/Crawl_3.png"));
			PlayerYellow.Crawl_R[4].Load(TEXT("Resource/Player_Yellow - Right/Crawl_4.png"));
			PlayerYellow.Crawl_R[5].Load(TEXT("Resource/Player_Yellow - Right/Crawl_5.png"));

			PlayerYellow.Stand[0].Load(TEXT("Resource/Player_Yellow/idle_0.png"));
			PlayerYellow.Stand[1].Load(TEXT("Resource/Player_Yellow/idle_1.png"));
			PlayerYellow.Stand[2].Load(TEXT("Resource/Player_Yellow/idle_2.png"));
			PlayerYellow.Stand[3].Load(TEXT("Resource/Player_Yellow/idle_3.png"));
			PlayerYellow.Stand[4].Load(TEXT("Resource/Player_Yellow/idle_4.png"));
			PlayerYellow.Stand[5].Load(TEXT("Resource/Player_Yellow/idle_5.png"));
			PlayerYellow.Stand[6].Load(TEXT("Resource/Player_Yellow/idle_6.png"));
			PlayerYellow.Stand[7].Load(TEXT("Resource/Player_Yellow/idle_7.png"));
			PlayerYellow.Stand[8].Load(TEXT("Resource/Player_Yellow/idle_8.png"));
			PlayerYellow.Stand[9].Load(TEXT("Resource/Player_Yellow/idle_9.png"));
			PlayerYellow.Stand[10].Load(TEXT("Resource/Player_Yellow/idle_10.png"));
			PlayerYellow.Stand[11].Load(TEXT("Resource/Player_Yellow/idle_11.png"));
			PlayerYellow.Stand[12].Load(TEXT("Resource/Player_Yellow/idle_12.png"));
			PlayerYellow.Stand[13].Load(TEXT("Resource/Player_Yellow/idle_13.png"));
			PlayerYellow.Stand[14].Load(TEXT("Resource/Player_Yellow/idle_14.png"));
			PlayerYellow.Stand[15].Load(TEXT("Resource/Player_Yellow/idle_15.png"));
			PlayerYellow.Stand[16].Load(TEXT("Resource/Player_Yellow/idle_16.png"));
			PlayerYellow.Stand[17].Load(TEXT("Resource/Player_Yellow/idle_17.png"));
			PlayerYellow.Stand[18].Load(TEXT("Resource/Player_Yellow/idle_18.png"));
			PlayerYellow.Stand[19].Load(TEXT("Resource/Player_Yellow/idle_19.png"));
			PlayerYellow.Stand[20].Load(TEXT("Resource/Player_Yellow/idle_20.png"));
			PlayerYellow.Stand[21].Load(TEXT("Resource/Player_Yellow/idle_21.png"));
			PlayerYellow.Stand[22].Load(TEXT("Resource/Player_Yellow/idle_22.png"));
			PlayerYellow.Stand[23].Load(TEXT("Resource/Player_Yellow/idle_23.png"));
			PlayerYellow.Stand[24].Load(TEXT("Resource/Player_Yellow/idle_24.png"));
			PlayerYellow.Stand[25].Load(TEXT("Resource/Player_Yellow/idle_25.png"));
			PlayerYellow.Stand[26].Load(TEXT("Resource/Player_Yellow/idle_26.png"));
			PlayerYellow.Stand[27].Load(TEXT("Resource/Player_Yellow/idle_27.png"));
			PlayerYellow.Stand[28].Load(TEXT("Resource/Player_Yellow/idle_28.png"));
			PlayerYellow.Stand[29].Load(TEXT("Resource/Player_Yellow/idle_29.png"));
			PlayerYellow.Stand[30].Load(TEXT("Resource/Player_Yellow/idle_30.png"));
			PlayerYellow.Stand[31].Load(TEXT("Resource/Player_Yellow/idle_31.png"));

			PlayerYellow.Stand_R[0].Load(TEXT("Resource/Player_Yellow - Right/idle_0.png"));
			PlayerYellow.Stand_R[1].Load(TEXT("Resource/Player_Yellow - Right/idle_1.png"));
			PlayerYellow.Stand_R[2].Load(TEXT("Resource/Player_Yellow - Right/idle_2.png"));
			PlayerYellow.Stand_R[3].Load(TEXT("Resource/Player_Yellow - Right/idle_3.png"));
			PlayerYellow.Stand_R[4].Load(TEXT("Resource/Player_Yellow - Right/idle_4.png"));
			PlayerYellow.Stand_R[5].Load(TEXT("Resource/Player_Yellow - Right/idle_5.png"));
			PlayerYellow.Stand_R[6].Load(TEXT("Resource/Player_Yellow - Right/idle_6.png"));
			PlayerYellow.Stand_R[7].Load(TEXT("Resource/Player_Yellow - Right/idle_7.png"));
			PlayerYellow.Stand_R[8].Load(TEXT("Resource/Player_Yellow - Right/idle_8.png"));
			PlayerYellow.Stand_R[9].Load(TEXT("Resource/Player_Yellow - Right/idle_9.png"));
			PlayerYellow.Stand_R[10].Load(TEXT("Resource/Player_Yellow - Right/idle_10.png"));
			PlayerYellow.Stand_R[11].Load(TEXT("Resource/Player_Yellow - Right/idle_11.png"));
			PlayerYellow.Stand_R[12].Load(TEXT("Resource/Player_Yellow - Right/idle_12.png"));
			PlayerYellow.Stand_R[13].Load(TEXT("Resource/Player_Yellow - Right/idle_13.png"));
			PlayerYellow.Stand_R[14].Load(TEXT("Resource/Player_Yellow - Right/idle_14.png"));
			PlayerYellow.Stand_R[15].Load(TEXT("Resource/Player_Yellow - Right/idle_15.png"));
			PlayerYellow.Stand_R[16].Load(TEXT("Resource/Player_Yellow - Right/idle_16.png"));
			PlayerYellow.Stand_R[17].Load(TEXT("Resource/Player_Yellow - Right/idle_17.png"));
			PlayerYellow.Stand_R[18].Load(TEXT("Resource/Player_Yellow - Right/idle_18.png"));
			PlayerYellow.Stand_R[19].Load(TEXT("Resource/Player_Yellow - Right/idle_19.png"));
			PlayerYellow.Stand_R[20].Load(TEXT("Resource/Player_Yellow - Right/idle_20.png"));
			PlayerYellow.Stand_R[21].Load(TEXT("Resource/Player_Yellow - Right/idle_21.png"));
			PlayerYellow.Stand_R[22].Load(TEXT("Resource/Player_Yellow - Right/idle_22.png"));
			PlayerYellow.Stand_R[23].Load(TEXT("Resource/Player_Yellow - Right/idle_23.png"));
			PlayerYellow.Stand_R[24].Load(TEXT("Resource/Player_Yellow - Right/idle_24.png"));
			PlayerYellow.Stand_R[25].Load(TEXT("Resource/Player_Yellow - Right/idle_25.png"));
			PlayerYellow.Stand_R[26].Load(TEXT("Resource/Player_Yellow - Right/idle_26.png"));
			PlayerYellow.Stand_R[27].Load(TEXT("Resource/Player_Yellow - Right/idle_27.png"));
			PlayerYellow.Stand_R[28].Load(TEXT("Resource/Player_Yellow - Right/idle_28.png"));
			PlayerYellow.Stand_R[29].Load(TEXT("Resource/Player_Yellow - Right/idle_29.png"));
			PlayerYellow.Stand_R[30].Load(TEXT("Resource/Player_Yellow - Right/idle_30.png"));
			PlayerYellow.Stand_R[31].Load(TEXT("Resource/Player_Yellow - Right/idle_31.png"));

			PlayerYellow.Run[0].Load(TEXT("Resource/Player_Yellow/Run_0.png"));
			PlayerYellow.Run[1].Load(TEXT("Resource/Player_Yellow/Run_1.png"));
			PlayerYellow.Run[2].Load(TEXT("Resource/Player_Yellow/Run_2.png"));
			PlayerYellow.Run[3].Load(TEXT("Resource/Player_Yellow/Run_3.png"));
			PlayerYellow.Run[4].Load(TEXT("Resource/Player_Yellow/Run_4.png"));
			PlayerYellow.Run[5].Load(TEXT("Resource/Player_Yellow/Run_5.png"));
			PlayerYellow.Run[6].Load(TEXT("Resource/Player_Yellow/Run_6.png"));
			PlayerYellow.Run[7].Load(TEXT("Resource/Player_Yellow/Run_7.png"));

			PlayerYellow.Run_R[0].Load(TEXT("Resource/Player_Yellow - Right/Run_0.png"));
			PlayerYellow.Run_R[1].Load(TEXT("Resource/Player_Yellow - Right/Run_1.png"));
			PlayerYellow.Run_R[2].Load(TEXT("Resource/Player_Yellow - Right/Run_2.png"));
			PlayerYellow.Run_R[3].Load(TEXT("Resource/Player_Yellow - Right/Run_3.png"));
			PlayerYellow.Run_R[4].Load(TEXT("Resource/Player_Yellow - Right/Run_4.png"));
			PlayerYellow.Run_R[5].Load(TEXT("Resource/Player_Yellow - Right/Run_5.png"));
			PlayerYellow.Run_R[6].Load(TEXT("Resource/Player_Yellow - Right/Run_6.png"));
			PlayerYellow.Run_R[7].Load(TEXT("Resource/Player_Yellow - Right/Run_7.png"));

			PlayerYellow.Sit[0].Load(TEXT("Resource/Player_Yellow/Sit_0.png"));
			PlayerYellow.Sit[1].Load(TEXT("Resource/Player_Yellow/Sit_1.png"));
			PlayerYellow.Sit[2].Load(TEXT("Resource/Player_Yellow/Sit_2.png"));
			PlayerYellow.Sit[3].Load(TEXT("Resource/Player_Yellow/Sit_3.png"));
			PlayerYellow.Sit[4].Load(TEXT("Resource/Player_Yellow/Sit_4.png"));
			PlayerYellow.Sit[5].Load(TEXT("Resource/Player_Yellow/Sit_5.png"));
			PlayerYellow.Sit[6].Load(TEXT("Resource/Player_Yellow/Sit_6.png"));
			PlayerYellow.Sit[7].Load(TEXT("Resource/Player_Yellow/Sit_7.png"));
			PlayerYellow.Sit[8].Load(TEXT("Resource/Player_Yellow/Sit_8.png"));
			PlayerYellow.Sit[9].Load(TEXT("Resource/Player_Yellow/Sit_9.png"));
			PlayerYellow.Sit[10].Load(TEXT("Resource/Player_Yellow/Sit_10.png"));
			PlayerYellow.Sit[11].Load(TEXT("Resource/Player_Yellow/Sit_11.png"));
			PlayerYellow.Sit[12].Load(TEXT("Resource/Player_Yellow/Sit_12.png"));

			PlayerYellow.Sit_R[0].Load(TEXT("Resource/Player_Yellow - Right/Sit_0.png"));
			PlayerYellow.Sit_R[1].Load(TEXT("Resource/Player_Yellow - Right/Sit_1.png"));
			PlayerYellow.Sit_R[2].Load(TEXT("Resource/Player_Yellow - Right/Sit_2.png"));
			PlayerYellow.Sit_R[3].Load(TEXT("Resource/Player_Yellow - Right/Sit_3.png"));
			PlayerYellow.Sit_R[4].Load(TEXT("Resource/Player_Yellow - Right/Sit_4.png"));
			PlayerYellow.Sit_R[5].Load(TEXT("Resource/Player_Yellow - Right/Sit_5.png"));
			PlayerYellow.Sit_R[6].Load(TEXT("Resource/Player_Yellow - Right/Sit_6.png"));
			PlayerYellow.Sit_R[7].Load(TEXT("Resource/Player_Yellow - Right/Sit_7.png"));
			PlayerYellow.Sit_R[8].Load(TEXT("Resource/Player_Yellow - Right/Sit_8.png"));
			PlayerYellow.Sit_R[9].Load(TEXT("Resource/Player_Yellow - Right/Sit_9.png"));
			PlayerYellow.Sit_R[10].Load(TEXT("Resource/Player_yellow - Right/Sit_10.png"));
			PlayerYellow.Sit_R[11].Load(TEXT("Resource/Player_yellow - Right/Sit_11.png"));
			PlayerYellow.Sit_R[12].Load(TEXT("Resource/Player_yellow - Right/Sit_12.png"));

			PlayerYellow.Tumbling[0].Load(TEXT("Resource/Player_Yellow/tumbling_0.png"));
			PlayerYellow.Tumbling[1].Load(TEXT("Resource/Player_Yellow/tumbling_1.png"));
			PlayerYellow.Tumbling[2].Load(TEXT("Resource/Player_Yellow/tumbling_2.png"));
			PlayerYellow.Tumbling[3].Load(TEXT("Resource/Player_Yellow/tumbling_3.png"));
			PlayerYellow.Tumbling[4].Load(TEXT("Resource/Player_Yellow/tumbling_4.png"));
			PlayerYellow.Tumbling[5].Load(TEXT("Resource/Player_Yellow/tumbling_5.png"));
			PlayerYellow.Tumbling[6].Load(TEXT("Resource/Player_Yellow/tumbling_6.png"));
			PlayerYellow.Tumbling[7].Load(TEXT("Resource/Player_Yellow/tumbling_7.png"));
			PlayerYellow.Tumbling[8].Load(TEXT("Resource/Player_Yellow/tumbling_8.png"));
			PlayerYellow.Tumbling[9].Load(TEXT("Resource/Player_Yellow/tumbling_9.png"));
			PlayerYellow.Tumbling[10].Load(TEXT("Resource/Player_Yellow/tumbling_10.png"));
			PlayerYellow.Tumbling[11].Load(TEXT("Resource/Player_Yellow/tumbling_11.png"));

			PlayerYellow.Tumbling_R[0].Load(TEXT("Resource/Player_Yellow - Right/tumbling_0.png"));
			PlayerYellow.Tumbling_R[1].Load(TEXT("Resource/Player_Yellow - Right/tumbling_1.png"));
			PlayerYellow.Tumbling_R[2].Load(TEXT("Resource/Player_Yellow - Right/tumbling_2.png"));
			PlayerYellow.Tumbling_R[3].Load(TEXT("Resource/Player_Yellow - Right/tumbling_3.png"));
			PlayerYellow.Tumbling_R[4].Load(TEXT("Resource/Player_Yellow - Right/tumbling_4.png"));
			PlayerYellow.Tumbling_R[5].Load(TEXT("Resource/Player_Yellow - Right/tumbling_5.png"));
			PlayerYellow.Tumbling_R[6].Load(TEXT("Resource/Player_Yellow - Right/tumbling_6.png"));
			PlayerYellow.Tumbling_R[7].Load(TEXT("Resource/Player_Yellow - Right/tumbling_7.png"));
			PlayerYellow.Tumbling_R[8].Load(TEXT("Resource/Player_Yellow - Right/tumbling_8.png"));
			PlayerYellow.Tumbling_R[9].Load(TEXT("Resource/Player_Yellow - Right/tumbling_9.png"));
			PlayerYellow.Tumbling_R[10].Load(TEXT("Resource/Player_Yellow - Right/tumbling_10.png"));
			PlayerYellow.Tumbling_R[11].Load(TEXT("Resource/Player_Yellow - Right/tumbling_11.png"));

			PlayerYellow.Jumpdown[0].Load(TEXT("Resource/Player_Yellow/Jump_down_0.png"));
			PlayerYellow.Jumpdown[1].Load(TEXT("Resource/Player_Yellow/Jump_down_1.png"));
			PlayerYellow.Jumpdown[2].Load(TEXT("Resource/Player_Yellow/Jump_down_2.png"));
			PlayerYellow.Jumpdown[3].Load(TEXT("Resource/Player_Yellow/Jump_down_3.png"));
			PlayerYellow.Jumpdown[4].Load(TEXT("Resource/Player_Yellow/Jump_down_4.png"));

			PlayerYellow.Jumpdown_R[0].Load(TEXT("Resource/Player_Yellow - Right/Jump_down_0.png"));
			PlayerYellow.Jumpdown_R[1].Load(TEXT("Resource/Player_Yellow - Right/Jump_down_1.png"));
			PlayerYellow.Jumpdown_R[2].Load(TEXT("Resource/Player_Yellow - Right/Jump_down_2.png"));
			PlayerYellow.Jumpdown_R[3].Load(TEXT("Resource/Player_Yellow - Right/Jump_down_3.png"));
			PlayerYellow.Jumpdown_R[4].Load(TEXT("Resource/Player_Yellow - Right/Jump_down_4.png"));
		}

		//===================================================================================================================
		PlayerPupple.isPupple = TRUE;
		PlayerPupple.Position = { 30,600 };
		PlayerPupple.isLeft = FALSE;
		PlayerPupple.State = state::Off;
		PlayerPupple.DS = drawState::Fall;
		PlayerPupple.HowJumpCount = 0;

		PlayerYellow.isPupple = FALSE;
		PlayerYellow.Position = { 750,600 };
		PlayerYellow.isLeft = TRUE;
		PlayerYellow.State = state::Off;
		PlayerYellow.DS = drawState::Fall;
		PlayerYellow.HowJumpCount = 0;

		PuppleRepotision();
		YellowRepotision();
		//===================================================================================================================
		Background[0].Image.Load(TEXT("Resource/배경/배경 1.jpg"));
		Background[1].Image.Load(TEXT("Resource/배경/배경 2.png"));
		Background[2].Image.Load(TEXT("Resource/배경/배경 3.bmp"));
		Background[3].Image.Load(TEXT("Resource/배경/배경 4.jpg"));
		Background[0].yPos = Client.top;
		Background[1].yPos = Client.bottom;
		BackgroundCheck = 0;
		//===================================================================================================================
		System.GameoverImage.Load(TEXT("Resource/배경/게임오버/게임 오버_1.png"));
		System.PauseImage[0].Load(TEXT("Resource/배경/카운트/카운트_1.png"));
		System.PauseImage[1].Load(TEXT("Resource/배경/카운트/카운트_2.png"));
		System.PauseImage[2].Load(TEXT("Resource/배경/카운트/카운트_3.png"));
		System.PauseButton.Load(TEXT("Resource/배경/카운트/정지버튼.png"));
		System.RetryButton.Load(TEXT("Resource/배경/카운트/다시하기.png"));
		System.GameClearImage.Load(TEXT("Resource/배경/카운트/게임 클리어.png"));
		System.PauseButtonPos = { Window_Clinet_Right - PauseButton_Size, Window_Clinet_Top,
								Window_Clinet_Right, Window_Clinet_Top + PauseButton_Size };
		System.RetryPos = { Window_Clinet_Right / 2 - 75, Window_Clinet_Bottom * 3 / 4 +75,
		Window_Clinet_Right / 2 + 75, Window_Clinet_Bottom * 3 / 4 + 125 };
		System.GameOver = FALSE;
		System.canSkill = TRUE;
		System.isStart = FALSE;
		System.Clear = FALSE;
		TargetCount = 500;
		//===================================================================================================================
		for (int i = 0; i < NormalBlockNum + 1; i++)
		{
			Blocks[i] = new Block;
			Blocks[i]->Loadimage(PlayerPupple.isPupple);
		}
		for (int i = NormalBlockNum + 1; i < NormalBlockNum + 1 + ColorBlockNum_Pupple; i++)
		{
			Blocks[i] = new ColorBlock(PlayerPupple.isPupple);
			Blocks[i]->Loadimage(PlayerPupple.isPupple);
		}
		for (int i = NormalBlockNum + 1 + ColorBlockNum_Pupple; i < NormalBlockNum + 1 + ColorBlockNum_Pupple + ColorBlockNum_Yellow; i++)
		{
			Blocks[i] = new ColorBlock(PlayerYellow.isPupple);
			Blocks[i]->Loadimage(PlayerYellow.isPupple);
		}
		for (int i = NormalBlockNum + 1 + ColorBlockNum_Pupple + ColorBlockNum_Yellow; i < NormalBlockNum + 1 + ColorBlockNum_Pupple + ColorBlockNum_Yellow + BreakBlockNum; i++)
		{
			Blocks[i] = new BreakBlock;
			Blocks[i]->Loadimage(PlayerYellow.isPupple);
		}
		for (int i = NormalBlockNum + 1 + ColorBlockNum_Pupple + ColorBlockNum_Yellow + BreakBlockNum; i < NormalBlockNum + 1 + ColorBlockNum_Pupple + ColorBlockNum_Yellow + BreakBlockNum + ThornBlockNum; i++)
		{
			Blocks[i] = new ThornBlock(isSpacePupple);
			Blocks[i]->Loadimage(isSpacePupple);
		}
		for (int i = NormalBlockNum + 1 + ColorBlockNum_Pupple + ColorBlockNum_Yellow + BreakBlockNum + ThornBlockNum; i < BlockNum; i++)
		{
			Blocks[i] = new DaggarBlock;
			Blocks[i]->Loadimage(isSpacePupple);
		}
		Blocks[0]->SetPosition(-100, -100);
		Blocks[1]->SetPosition(30, 800);
		Blocks[2]->SetPosition(Window_Clinet_Right - WallBlock_Size - Block_Size_Lenght, 800);
		Blocks[1]->canScore = FALSE;
		Blocks[2]->canScore = FALSE;
		for (int i = 3; i < BlockNum; i++)
		{
			Blocks[i]->SetPosition(-100, -100);
		}
		for (int i = 0; i < BlockNum; i++)
		{
			Blocks[i]->SetObjectNumber(i);
		}

		for (int i = 0; i < WallBlockNum; i++)
		{
			Wall[i] = new WallBlock;
			Wall[i]->Loadimage(isSpacePupple);
			Wall[i]->SetObjectNumber(i);

			if (i >= WallBlockNum / 2)
			{
				Wall[i]->isLeft = FALSE;
			}
			
			if (i < 30)
			{
				Wall[i]->SetPosition(Window_Clinet_Left, i* WallBlock_Size);
			}
			else if (WallBlockNum / 2 <= i && i < WallBlockNum / 2 + 30)
			{
				Wall[i]->SetPosition(Window_Clinet_Right - WallBlock_Size, (i- WallBlockNum / 2 )* WallBlock_Size);
			}
		}

		//===================================================================================================================
		for (int i = 0; i < 9; i++)
		{
			StoreNum[i] = i;
		}
		CountNum.NowNormal = 3;
		CountNum.NowPupple = NormalBlockNum + 1;
		CountNum.NowYellow = NormalBlockNum + 1 + ColorBlockNum_Pupple;
		CountNum.NowBreak = NormalBlockNum + 1 + ColorBlockNum_Pupple + ColorBlockNum_Yellow;
		CountNum.NowThorn = NormalBlockNum + 1 + ColorBlockNum_Pupple + ColorBlockNum_Yellow + BreakBlockNum;
		CountNum.NowDaggar = NormalBlockNum + 1 + ColorBlockNum_Pupple + ColorBlockNum_Yellow + BreakBlockNum + ThornBlockNum;
		//==================================================================================================================
		isSpacePupple = TRUE;
		Score = 0;
		Circle = { (Window_Clinet_Left + Window_Clinet_Right) / 2, (Window_Clinet_Top + Window_Clinet_Bottom) / 2,
		(Window_Clinet_Left + Window_Clinet_Right) / 2, (Window_Clinet_Top + Window_Clinet_Bottom) / 2 };
		
		//Sound_Setup();
		//FMOD_Channel_Stop(Channel[0]);
		//FMOD_System_PlaySound(soundSystem, bgmsound[0], NULL, 0, &Channel[0]);
		//FMOD_Channel_SetVolume(Channel[0], 0.05);

		GetClientRect(hWnd, &Client);
		SetTimer(hWnd, 1, 60, NULL);
		break;
	}
	case WM_KEYDOWN:
	{
		WhatKey[wParam] = TRUE;
		break;
	}
	case WM_KEYUP:
	{
		WhatKey[wParam] = FALSE;
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 40001:
		{
			TargetCount = 500;
			break;
		}
		case 40002:
		{
			TargetCount = 1000;
			break;
		}
		case 40003:
		{
			TargetCount = 2000;
			break;
		}
		case 40004:
		{
			TargetCount = 3000;
			break;
		}
		}
	}
	case WM_LBUTTONDOWN:
	{
		mx = LOWORD(IParam);
		my = HIWORD(IParam);

		if (System.isStart)
		{
			if (System.PauseButtonPos.left < mx && mx < System.PauseButtonPos.right
				&& System.PauseButtonPos.top < my && my < System.PauseButtonPos.bottom)
			{
				System.isStart = FALSE;
			}
		}
		if (System.GameOver)
		{
			if (System.RetryPos.left < mx && mx < System.RetryPos.right
				&& System.RetryPos.top < my && my < System.RetryPos.bottom)
			{
				System.Reset = TRUE;
				_Reset(hWnd);
			}
		}
		break;
	}
	case WM_TIMER:
	{
		hDC = GetDC(hWnd);

		FinalBit = CreateCompatibleBitmap(hDC, Window_XSize, Window_YSize);

		mem1DC = CreateCompatibleDC(hDC);

		oldBit = (HBITMAP)SelectObject(mem1DC, FinalBit);

		if (System.isStart)
		{
			End(hWnd);
			Loop();
			BlockUpdate();
			Score++;
			CreateBlockCount++;
			BackgroundCheck++;
			PauseCount = 0;
			Background[3].yPos += 1;
			if (!System.canSkill)
			{
				ShakeMap(ShakeCount);
				Circle.left -= 50;
				Circle.top -= 50;
				Circle.right += 50;
				Circle.bottom += 50;
				ShakeCount++;
				if (Circle.left < -300)
				{
					System.canSkill = TRUE;
					Circle = { (Window_Clinet_Left + Window_Clinet_Right) / 2, (Window_Clinet_Top + Window_Clinet_Bottom) / 2,
								(Window_Clinet_Left + Window_Clinet_Right) / 2, (Window_Clinet_Top + Window_Clinet_Bottom) / 2 };
				}
			}
		}
		else
		{
			PauseCount++;
			if (PauseCount == Pause_MaxCount)
			{
				System.isStart = TRUE;
			}
		}
		//-------------------------------------------------------------------------------------------------------------------------------------
		if (CreateBlockCount == 50)
		{
			CreateBlockCount = 0;
			CreateBlockPosition();
			CheckNow();
			Blocks[CountNum.NowNormal++]->Create(StoreNum[0]);
			Blocks[CountNum.NowPupple++]->Create(StoreNum[1]);
			Blocks[CountNum.NowYellow++]->Create(StoreNum[2]);
			Blocks[CountNum.NowBreak++]->Create(StoreNum[3]);
			if (StoreNum[8] == 5 || StoreNum[8] == 8)
			{
				Blocks[CountNum.NowThorn++]->Create(StoreNum[8]);
			}
			if (StoreNum[8] == 4 || StoreNum[8] == 6 || StoreNum[8] == 7)
			{
				Blocks[CountNum.NowDaggar++]->Create(CreateDaggarNum(gen));
			}
			CheckNow();
			if (StoreNum[6] % 4 == 0)
			{
				Blocks[CountNum.NowNormal++]->Create(StoreNum[4]);
				Blocks[CountNum.NowBreak++]->Create(StoreNum[5]);
			}
			else if (StoreNum[6] % 4 == 1)
			{
				Blocks[CountNum.NowPupple++]->Create(StoreNum[4]);
				Blocks[CountNum.NowBreak++]->Create(StoreNum[5]);
			}
			else if (StoreNum[6] % 4 == 2)
			{
				Blocks[CountNum.NowYellow++]->Create(StoreNum[4]);
				Blocks[CountNum.NowBreak++]->Create(StoreNum[5]);
			}
			else if (StoreNum[6] % 4 == 3)
			{
				Blocks[CountNum.NowBreak++]->Create(StoreNum[4]);
				Blocks[CountNum.NowNormal++]->Create(StoreNum[5]);
			}
			if (StoreNum[7] == 4 || StoreNum[7] == 6 || StoreNum[7] == 7)
			{
				Blocks[CountNum.NowDaggar++]->Create(CreateDaggarNum(gen));
			}
		}

		//------------------------------------------------------------------------------------------------------------------------------------- ▽ 배경

		/*if (BackgroundCheck < Background_SIZE_Raw)
		{
			Background[0].yPos += 1;

			Background[0].Image.Draw(mem1DC, Client.left, Client.top, Client.right, Client.bottom - Background[0].yPos,
				0, Background[0].yPos, Background_SIZE_Lenght, Background_SIZE_Raw - Background[0].yPos);
			Background[1].Image.Draw(mem1DC, Client.left, Client.bottom - Background[0].yPos, Client.right, Background[0].yPos,
				0, 0, Background_SIZE_Lenght, Background[0].yPos);
		}
		else if (Background_SIZE_Raw <= BackgroundCheck && BackgroundCheck < 2 * Background_SIZE_Raw)
		{
			Background[1].yPos += 1;
			if (BackgroundCheck == Background_SIZE_Raw)
			{
				Background[1].yPos = 0;
				Background[1].Image.Draw(mem1DC, Client.left, Client.top, Client.right, Client.bottom - Background[1].yPos,
					0, Background[1].yPos, Background_SIZE_Lenght, Background_SIZE_Raw - Background[1].yPos);
			}
			else
			{
				Background[1].Image.Draw(mem1DC, Client.left, Client.top, Client.right, Client.bottom - Background[1].yPos,
					0, Background[1].yPos, Background_SIZE_Lenght, Background_SIZE_Raw - Background[1].yPos);
				Background[2].Image.Draw(mem1DC, Client.left, Client.bottom - Background[1].yPos, Client.right, Background[1].yPos,
					0, 0, Background_SIZE_Lenght, Background[1].yPos);
			}
		}
		else if (2 * Background_SIZE_Raw <= BackgroundCheck && BackgroundCheck < 3 * Background_SIZE_Raw)
		{
			Background[2].yPos += 1;
			if (BackgroundCheck == 2 * Background_SIZE_Raw)
			{
				Background[2].yPos = 0;
				Background[2].Image.Draw(mem1DC, Client.left, Client.top, Client.right, Client.bottom - Background[2].yPos,
					0, Background[2].yPos, Background_SIZE_Lenght, Background_SIZE_Raw - Background[2].yPos);
			}
			else
			{
				Background[2].Image.Draw(mem1DC, Client.left, Client.top, Client.right, Client.bottom - Background[2].yPos,
					0, Background[2].yPos, Background_SIZE_Lenght, Background_SIZE_Raw - Background[2].yPos);
			}
		}*/
		if (BackgroundCheck < Background4_SIZE_Raw)
		{
			Background[3].Image.Draw(mem1DC, Client.left, Client.top, Client.right, Client.bottom,
				0, Background[3].yPos, Background4_SIZE_Lenght, Client.bottom + Background[3].yPos);
		}

		//------------------------------------------------------------------------------------------------------------------------------------- ▽ 블럭
		for (int i = 0; i < WallBlockNum; i++)
		{
			Wall[i]->Draw(mem1DC);
		}
		//------------------------------------------------------------------------------------------------------------------------------------- ▽ 보라색
		if (PlayerPupple.State == state::On)
		{
			if (PlayerPupple.DS == drawState::Stand)
			{
				PlayerPupple.StandCount++;
				if (PlayerPupple.StandCount >= IdleNum)
				{
					PlayerPupple.StandCount = 0;
				}
				if (PlayerPupple.isLeft)
				{
					PlayerPupple.Stand[PlayerPupple.StandCount].Draw(mem1DC, PlayerPupple.Position.x, PlayerPupple.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
				}
				else
				{
					PlayerPupple.Stand_R[PlayerPupple.StandCount].Draw(mem1DC, PlayerPupple.Position.x, PlayerPupple.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
				}
			}
			else if (PlayerPupple.DS == drawState::Run)
			{
				PlayerPupple.RunCount++;
				if (PlayerPupple.RunCount >= RunNum)
				{
					PlayerPupple.RunCount = 0;
				}
				if (PlayerPupple.isLeft)
				{
					PlayerPupple.Run[PlayerPupple.RunCount].Draw(mem1DC, PlayerPupple.Position.x, PlayerPupple.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
				}
				else
				{
					PlayerPupple.Run_R[PlayerPupple.RunCount].Draw(mem1DC, PlayerPupple.Position.x, PlayerPupple.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
				}
			}
			else if (PlayerPupple.DS == drawState::Sit)
			{
				if (PlayerPupple.SitCount < SitNum - 1)
				{
					PlayerPupple.SitCount++;
				}
				if (PlayerPupple.isLeft)
				{
					PlayerPupple.Sit[PlayerPupple.SitCount].Draw(mem1DC, PlayerPupple.Position.x, PlayerPupple.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
				}
				else
				{
					PlayerPupple.Sit_R[PlayerPupple.SitCount].Draw(mem1DC, PlayerPupple.Position.x, PlayerPupple.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
				}

			}
			else if (PlayerPupple.DS == drawState::Crawl)
			{
				PlayerPupple.SitCount = SitNum - 1;
				PlayerPupple.CrawlCount++;
				if (PlayerPupple.CrawlCount >= CrawlNum)
				{
					PlayerPupple.CrawlCount = 0;
				}
				if (PlayerPupple.isLeft)
				{
					PlayerPupple.Crawl[PlayerPupple.CrawlCount].Draw(mem1DC, PlayerPupple.Position.x, PlayerPupple.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
				}
				else
				{
					PlayerPupple.Crawl_R[PlayerPupple.CrawlCount].Draw(mem1DC, PlayerPupple.Position.x, PlayerPupple.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
				}
			}
		}
		else
		{
			Position_Compute(FALSE);
			Blocks[PlayerPupple.OnBoardNum]->OffCheck();

			if (PlayerPupple.DS == drawState::Fall)
			{
				PlayerPupple.JumpdownCount++;
				if (PlayerPupple.JumpdownCount >= JumpDownNum)
				{
					PlayerPupple.JumpdownCount = 0;
				}
				if (PlayerPupple.isLeft)
				{
					PlayerPupple.Jumpdown[PlayerPupple.JumpdownCount].Draw(mem1DC, PlayerPupple.Position.x, PlayerPupple.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
				}
				else
				{
					PlayerPupple.Jumpdown_R[PlayerPupple.JumpdownCount].Draw(mem1DC, PlayerPupple.Position.x, PlayerPupple.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
				}
			}
			else if (PlayerPupple.DS == drawState::Jump)
			{
				PlayerPupple.TumblingCount++;
				if (PlayerPupple.TumblingCount >= TumblingNum)
				{
					PlayerPupple.TumblingCount = 0;
				}
				if (PlayerPupple.HowJumpCount < 10)
				{
					PlayerPupple.HowJumpCount++;
				}
				else
				{
					if (PlayerPupple.HowJumpCount < 20)
					{
						PlayerPupple.HowJumpCount++;
					}
					else
					{
						PlayerPupple.DS = drawState::Fall;
						PlayerPupple.HowJumpCount = 0;
					}
				}
				if (PlayerPupple.isLeft)
				{
					PlayerPupple.Tumbling[PlayerPupple.TumblingCount].Draw(mem1DC, PlayerPupple.Position.x, PlayerPupple.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
				}
				else
				{
					PlayerPupple.Tumbling_R[PlayerPupple.TumblingCount].Draw(mem1DC, PlayerPupple.Position.x, PlayerPupple.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------------------- ▽ 노란색
		if (PlayerYellow.State == state::On)
		{
			if (PlayerYellow.DS == drawState::Stand)
			{
				PlayerYellow.StandCount++;
				if (PlayerYellow.StandCount >= IdleNum)
				{
					PlayerYellow.StandCount = 0;
				}
				if (PlayerYellow.isLeft)
				{
					PlayerYellow.Stand[PlayerYellow.StandCount].Draw(mem1DC, PlayerYellow.Position.x, PlayerYellow.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
				}
				else
				{
					PlayerYellow.Stand_R[PlayerYellow.StandCount].Draw(mem1DC, PlayerYellow.Position.x, PlayerYellow.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
				}
			}
			else if (PlayerYellow.DS == drawState::Run)
			{
				PlayerYellow.RunCount++;
				if (PlayerYellow.RunCount >= RunNum)
				{
					PlayerYellow.RunCount = 0;
				}
				if (PlayerYellow.isLeft)
				{
					PlayerYellow.Run[PlayerYellow.RunCount].Draw(mem1DC, PlayerYellow.Position.x, PlayerYellow.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
				}
				else
				{
					PlayerYellow.Run_R[PlayerYellow.RunCount].Draw(mem1DC, PlayerYellow.Position.x, PlayerYellow.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
				}
			}
			else if (PlayerYellow.DS == drawState::Sit)
			{
					if (PlayerYellow.SitCount < SitNum - 1)
					{
						PlayerYellow.SitCount++;
					}
					if (PlayerYellow.isLeft)
					{
						PlayerYellow.Sit[PlayerYellow.SitCount].Draw(mem1DC, PlayerYellow.Position.x, PlayerYellow.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
					}
					else
					{
						PlayerYellow.Sit_R[PlayerYellow.SitCount].Draw(mem1DC, PlayerYellow.Position.x, PlayerYellow.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
					}
			}
			else if (PlayerYellow.DS == drawState::Crawl)
			{
				PlayerYellow.SitCount = SitNum - 1;
				PlayerYellow.CrawlCount++;
				if (PlayerYellow.CrawlCount >= CrawlNum)
				{
					PlayerYellow.CrawlCount = 0;
				}
				if (PlayerYellow.isLeft)
				{
					PlayerYellow.Crawl[PlayerYellow.CrawlCount].Draw(mem1DC, PlayerYellow.Position.x, PlayerYellow.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
				}
				else
				{
					PlayerYellow.Crawl_R[PlayerYellow.CrawlCount].Draw(mem1DC, PlayerYellow.Position.x, PlayerYellow.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
				}
			}
		}
		else if(PlayerYellow.State == state::Off)
		{
			Position_Compute(TRUE);
			Blocks[PlayerYellow.OnBoardNum]->OffCheck();

			if (PlayerYellow.DS == drawState::Fall)
			{
				PlayerYellow.JumpdownCount++;
				if (PlayerYellow.JumpdownCount >= JumpDownNum)
				{
					PlayerYellow.JumpdownCount = 0;
				}
				if (PlayerYellow.isLeft)
				{
					PlayerYellow.Jumpdown[PlayerYellow.JumpdownCount].Draw(mem1DC, PlayerYellow.Position.x, PlayerYellow.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
				}
				else
				{
					PlayerYellow.Jumpdown_R[PlayerYellow.JumpdownCount].Draw(mem1DC, PlayerYellow.Position.x, PlayerYellow.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
				}
			}
			else if (PlayerYellow.DS == drawState::Jump)
			{
			PlayerYellow.TumblingCount++;
			if (PlayerYellow.TumblingCount >= TumblingNum)
			{
				PlayerYellow.TumblingCount = 0;
			}
			if (PlayerYellow.HowJumpCount < 10)
			{
				PlayerYellow.HowJumpCount++;
			}
			else
			{
				if (PlayerYellow.HowJumpCount < 20)
				{
					PlayerYellow.HowJumpCount++;
				}
				else
				{
					PlayerYellow.DS = drawState::Fall;
					PlayerYellow.HowJumpCount = 0;
				}
			}
			if (PlayerYellow.isLeft)
			{
				PlayerYellow.Tumbling[PlayerYellow.TumblingCount].Draw(mem1DC, PlayerYellow.Position.x, PlayerYellow.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
			}
			else
			{
				PlayerYellow.Tumbling_R[PlayerYellow.TumblingCount].Draw(mem1DC, PlayerYellow.Position.x, PlayerYellow.Position.y, 100, 100, 0, 0, Image_Size, Image_Size);
			}
			}
		}
		//-------------------------------------------------------------------------------------------------------------------------------------
		for (int i = 0; i < BlockNum; i++)
		{
			Blocks[i]->Draw(mem1DC);
		}
		//-------------------------------------------------------------------------------------------------------------------------------------
		if (!System.isStart)
		{
			if (0 <= PauseCount && PauseCount < Pause_MaxCount / 3)
			{
				System.PauseImage[2].Draw(mem1DC, Window_Clinet_Left, Window_Clinet_Top, Window_Clinet_Right, Window_Clinet_Bottom, 0, 0, Window_Clinet_Right, Window_Clinet_Bottom);
			}
			else if (Pause_MaxCount / 3 <= PauseCount && PauseCount < Pause_MaxCount / 3 * 2)
			{
				System.PauseImage[1].Draw(mem1DC, Window_Clinet_Left, Window_Clinet_Top, Window_Clinet_Right, Window_Clinet_Bottom, 0, 0, Window_Clinet_Right, Window_Clinet_Bottom);
			}
			else if (Pause_MaxCount / 3 * 2 <= PauseCount && PauseCount < Pause_MaxCount)
			{
				System.PauseImage[0].Draw(mem1DC, Window_Clinet_Left, Window_Clinet_Top, Window_Clinet_Right, Window_Clinet_Bottom, 0, 0, Window_Clinet_Right, Window_Clinet_Bottom);
			}
		}
		System.PauseButton.Draw(mem1DC, System.PauseButtonPos.left, System.PauseButtonPos.top
			, PauseButton_Size, PauseButton_Size, 0, 0, PauseButton_Size, PauseButton_Size);
		//-------------------------------------------------------------------------------------------------------------------------------------  메모리 해제
		Rectangle(mem1DC, 0, 0, 150, 50);
		wsprintf(StoreScore, L"%d", Score);
		TextOut(mem1DC, 65, 20, StoreScore, lstrlen(StoreScore));
		//-------------------------------------------------------------------------------------------------------------------------------------
		SelectObject(mem1DC, oldBit);
		DeleteDC(mem1DC);
		ReleaseDC(hWnd, hDC);
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	case WM_PAINT:
	{
		hDC = BeginPaint(hWnd, &ps);

		mem1DC = CreateCompatibleDC(hDC);
		oldBit = (HBITMAP)SelectObject(mem1DC, FinalBit);

		BitBlt(hDC, 0, 0, Window_XSize, Window_YSize, mem1DC, 0, 0, SRCCOPY);

		if (System.GameOver)
		{
			System.GameoverImage.Draw(hDC, Window_Clinet_Left, Window_Clinet_Top, Window_Clinet_Right, Window_Clinet_Bottom, 0, 0, Window_Clinet_Right, Window_Clinet_Bottom);
			System.RetryButton.Draw(hDC, System.RetryPos.left, System.RetryPos.top, 150, 50
				, 0, 0, RetryButton_Size_Lenght, RetryButton_Size_Raw);

			//FMOD_Channel_Stop(Channel[0]);
			//FMOD_System_PlaySound(soundSystem, bgmsound[1], NULL, 0, &Channel[0]);
			//FMOD_Channel_SetVolume(Channel[0], 0.1);
		}

		if (System.Clear)
		{
			System.GameClearImage.Draw(hDC, Window_Clinet_Left, Window_Clinet_Top, Window_Clinet_Right, Window_Clinet_Bottom, 0, 0, Window_Clinet_Right, Window_Clinet_Bottom);
			System.RetryButton.Draw(hDC, System.RetryPos.left, System.RetryPos.top, 150, 50
				, 0, 0, RetryButton_Size_Lenght, RetryButton_Size_Raw);

			//FMOD_Channel_Stop(Channel[0]);
			//FMOD_System_PlaySound(soundSystem, bgmsound[2], NULL, 0, &Channel[0]);
			//FMOD_Channel_SetVolume(Channel[0], 0.05);
		}

		if (!System.canSkill)
		{
			hPen = CreatePen(PS_SOLID, 11, RGB(255, 255, 255));
			oldPen = (HPEN)SelectObject(hDC, hPen);

			Arc(hDC, Circle.left, Circle.top, Circle.right, Circle.bottom, (Circle.left+ Circle.right)/2, (Circle.top + Circle.bottom)/2,
				(Circle.left + Circle.right) / 2, (Circle.top + Circle.bottom) / 2);

			SelectObject(hDC, oldPen);
			DeleteObject(hPen);
		}

		SelectObject(mem1DC, oldBit);
		DeleteDC(mem1DC);
		EndPaint(hWnd, &ps);

		break;
	}
	case WM_DESTROY:
	{
		//for (int i = 0; i < 3; i++)
		//{
		//	FMOD_Sound_Release(bgmsound[i]);
		//}
		//FMOD_Sound_Release(effectSound);
		//FMOD_System_Release(soundSystem);
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	}
	}
	return DefWindowProc(hWnd, uMsg, wParam, IParam);
}

BOOL BothCrush()
{
	if (IntersectRect(&BothCrushCheck, &PlayerPupple.Box, &PlayerYellow.Box))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void PuppleRepotision()
{
	if (PlayerPupple.DS == drawState::Jump)
	{
		PlayerPupple.Box = { PlayerPupple.Position.x + 37, PlayerPupple.Position.y + 34, PlayerPupple.Position.x + 82, PlayerPupple.Position.y + 80 };
	}
	else if (PlayerPupple.DS == drawState::Sit || PlayerPupple.DS == drawState::Crawl)
	{
		PlayerPupple.Box = { PlayerPupple.Position.x + 37, PlayerPupple.Position.y + 54, PlayerPupple.Position.x + 82, PlayerPupple.Position.y + 80 };
	}	
	else
	{
		PlayerPupple.Box = { PlayerPupple.Position.x + 37, PlayerPupple.Position.y + 14, PlayerPupple.Position.x + 82, PlayerPupple.Position.y + 80 };
	}
}

void YellowRepotision()
{
	if (PlayerYellow.DS == drawState::Jump)
	{
		PlayerYellow.Box = { PlayerYellow.Position.x + 37, PlayerYellow.Position.y + 34, PlayerYellow.Position.x + 82, PlayerYellow.Position.y + 80 };
	}
	else if (PlayerYellow.DS == drawState::Sit || PlayerYellow.DS == drawState::Crawl)
	{
		PlayerYellow.Box = { PlayerYellow.Position.x + 37, PlayerYellow.Position.y + 54, PlayerYellow.Position.x + 82, PlayerYellow.Position.y + 80 };
	}
	else
	{
		PlayerYellow.Box = { PlayerYellow.Position.x + 37, PlayerYellow.Position.y + 14, PlayerYellow.Position.x + 82, PlayerYellow.Position.y + 80 };
	}
}

void BlockUpdate()
{
	for (int i = 0; i < BlockNum; i++)
	{
		Blocks[i]->Update();
	}
	for (int i = 0; i < WallBlockNum; i++)
	{
		Wall[i]->Update();
	}
}

void Loop()
{
	if (PlayerPupple.State == state::Off)
	{
		if (WhatKey[65]&&!WhatKey[68])
		{
			PlayerPupple.isLeft = TRUE;
			PlayerPupple.Position.x -= Stand_xMove;

			if (Pupple_WallCrush() != -1)
			{
				if (Wall[Pupple_WallCrush()]->isLeft)
				{
					PlayerPupple.Position.x += Bounce_Power_x;
				}
				else
				{
					PlayerPupple.Position.x -= Bounce_Power_x;
				}
			}
		}
		else if (!WhatKey[65] && WhatKey[68])
		{
			PlayerPupple.isLeft = FALSE;
			PlayerPupple.Position.x += Stand_xMove;

			if (Pupple_WallCrush() != -1)
			{
				if (Wall[Pupple_WallCrush()]->isLeft)
				{
					PlayerPupple.Position.x += Bounce_Power_x;
				}
				else
				{
					PlayerPupple.Position.x -= Bounce_Power_x;
				}
			}
		}

		if (Pupple_BlockCrush())
		{
			PlayerPupple.Accelerate = 0;
			PlayerPupple.Speed = 0;
			PlayerPupple.State = state::On;
			PlayerPupple.DS = drawState::Stand;
		}
	}
	else if (PlayerPupple.State == state::On)
	{
		if (!WhatKey[65] && !WhatKey[83] && !WhatKey[68] && !WhatKey[87])
		{
			PlayerPupple.DS = drawState::Stand;
			PlayerPupple.SitCount = 0;
		}
		else if (WhatKey[87])
		{
			PlayerPupple.State = state::Off;
			PlayerPupple.DS = drawState::Jump;
			PlayerPupple.Position.y -= (PlayerPupple.Box.bottom - Blocks[PlayerPupple.OnBoardNum]->Box.top + 30);
			PlayerPupple.Accelerate = -Jump_Power;
			PlayerPupple.Speed = -Speed_Power;
		}
		else if (!WhatKey[65] && WhatKey[83] && !WhatKey[68])
		{
			PlayerPupple.DS = drawState::Sit;
		}
		else if (WhatKey[65] && WhatKey[83] && !WhatKey[68])
		{
			PlayerPupple.isLeft = TRUE;
			PlayerPupple.DS = drawState::Crawl;
			PlayerPupple.Position.x -= Crawl_xMove;

			if (Pupple_WallCrush() != -1)
			{
				if (Wall[Pupple_WallCrush()]->isLeft)
				{
					PlayerPupple.Position.x += Bounce_Power_x;
				}
				else
				{
					PlayerPupple.Position.x -= Bounce_Power_x;
				}
			}
		}
		else if (!WhatKey[65] && WhatKey[83] && WhatKey[68])
		{
			PlayerPupple.isLeft = FALSE;
			PlayerPupple.DS = drawState::Crawl;
			PlayerPupple.Position.x += Crawl_xMove;

			if (Pupple_WallCrush() != -1)
			{
				if (Wall[Pupple_WallCrush()]->isLeft)
				{
					PlayerPupple.Position.x += Bounce_Power_x;
				}
				else
				{
					PlayerPupple.Position.x -= Bounce_Power_x;
				}
			}
		}
		else if (WhatKey[65] && !WhatKey[83] && !WhatKey[68])
		{
			PlayerPupple.isLeft = TRUE;
			PlayerPupple.DS = drawState::Run;
			PlayerPupple.Position.x -= Stand_xMove;

			if (Pupple_WallCrush() != -1)
			{
				if (Wall[Pupple_WallCrush()]->isLeft)
				{
					PlayerPupple.Position.x += Bounce_Power_x;
				}
				else
				{
					PlayerPupple.Position.x -= Bounce_Power_x;
				}
			}
		}
		else if (!WhatKey[65] && !WhatKey[83] && WhatKey[68])
		{
			PlayerPupple.isLeft = FALSE;
			PlayerPupple.DS = drawState::Run;
			PlayerPupple.Position.x += Stand_xMove;

			if (Pupple_WallCrush() != -1)
			{
				if (Wall[Pupple_WallCrush()]->isLeft)
				{
					PlayerPupple.Position.x += Bounce_Power_x;
				}
				else
				{
					PlayerPupple.Position.x -= Bounce_Power_x;
				}
			}
		}
		if (!Pupple_BlockCrush())
		{
			float i = (PlayerPupple.Box.left + PlayerPupple.Box.right) * 0.5f;
			if (!(i > Blocks[PlayerPupple.OnBoardNum]->Box.left && i < Blocks[PlayerPupple.OnBoardNum]->Box.right) || !Blocks[PlayerPupple.OnBoardNum]->isVisible)
			{
				PlayerPupple.State = state::Off;
				PlayerPupple.DS = drawState::Fall;
			}
			
		}

		if (PlayerPupple.State == state::On)
		{
			if (Pupple_BlockCrush(PlayerPupple.OnBoardNum))
			{
				float i = (PlayerPupple.Box.left + PlayerPupple.Box.right) * 0.5f;
				if (i > Blocks[PlayerPupple.OnBoardNum]->Box.left && i < Blocks[PlayerPupple.OnBoardNum]->Box.right)
				{
					PlayerPupple.Position.y -= Blocks[PlayerYellow.OnBoardNum]->Speed;
				}
			}
		}
	}
	//=======================================================================================================================
	if (PlayerYellow.State == state::Off)
	{
		if (WhatKey[VK_LEFT] && !WhatKey[VK_RIGHT])
		{
			PlayerYellow.isLeft = TRUE;
			PlayerYellow.Position.x -= Stand_xMove;

			if (Yellow_WallCrush() != -1)
			{
				if (Wall[Yellow_WallCrush()]->isLeft)
				{
					PlayerYellow.Position.x += Bounce_Power_x;
				}
				else
				{
					PlayerYellow.Position.x -= Bounce_Power_x;
				}
			}
		}
		else if (!WhatKey[VK_LEFT] && WhatKey[VK_RIGHT])
		{
			PlayerYellow.isLeft = FALSE;
			PlayerYellow.Position.x += Stand_xMove;

			if (Yellow_WallCrush() != -1)
			{
				if (Wall[Yellow_WallCrush()]->isLeft)
				{
					PlayerYellow.Position.x += Bounce_Power_x;
				}
				else
				{
					PlayerYellow.Position.x -= Bounce_Power_x;
				}
			}
		}

		if (Yellow_BlockCrush())
		{
			PlayerYellow.Accelerate = 0;
			PlayerYellow.Speed = 0;
			PlayerYellow.State = state::On;
			PlayerYellow.DS = drawState::Stand;
		}
	}
	else if (PlayerYellow.State == state::On)
	{
		if (!WhatKey[VK_LEFT] && !WhatKey[VK_DOWN] && !WhatKey[VK_RIGHT] && !WhatKey[VK_UP])
		{
			PlayerYellow.DS = drawState::Stand;
			PlayerYellow.SitCount = 0;
		}
		else if (WhatKey[VK_UP])
		{
			PlayerYellow.State = state::Off;
			PlayerYellow.DS = drawState::Jump;
			PlayerYellow.Position.y -= (PlayerYellow.Box.bottom - Blocks[PlayerYellow.OnBoardNum]->Box.top + 30);
			PlayerYellow.Accelerate = -Jump_Power;
			PlayerYellow.Speed = -Speed_Power;
		}
		else if (!WhatKey[VK_LEFT] && WhatKey[VK_DOWN] && !WhatKey[VK_RIGHT])
		{
			PlayerYellow.DS = drawState::Sit;
		}
		else if (WhatKey[VK_LEFT] && WhatKey[VK_DOWN] && !WhatKey[VK_RIGHT])
		{
			PlayerYellow.isLeft = TRUE;
			PlayerYellow.DS = drawState::Crawl;
			PlayerYellow.Position.x -= Crawl_xMove;

			if (Yellow_WallCrush() != -1)
			{
				if (Wall[Yellow_WallCrush()]->isLeft)
				{
					PlayerYellow.Position.x += Bounce_Power_x;
				}
				else
				{
					PlayerYellow.Position.x -= Bounce_Power_x;
				}
			}
		}
		else if (!WhatKey[VK_LEFT] && WhatKey[VK_DOWN] && WhatKey[VK_RIGHT])
		{
			PlayerYellow.isLeft = FALSE;
			PlayerYellow.DS = drawState::Crawl;
			PlayerYellow.Position.x += Crawl_xMove;

			if (Yellow_WallCrush() != -1)
			{
				if (Wall[Yellow_WallCrush()]->isLeft)
				{
					PlayerYellow.Position.x += Bounce_Power_x;
				}
				else
				{
					PlayerYellow.Position.x -= Bounce_Power_x;
				}
			}
		}
		else if (WhatKey[VK_LEFT] && !WhatKey[VK_DOWN] && !WhatKey[VK_RIGHT])
		{
			PlayerYellow.isLeft = TRUE;
			PlayerYellow.DS = drawState::Run;
			PlayerYellow.Position.x -= Stand_xMove;

			if (Yellow_WallCrush() != -1)
			{
				if (Wall[Yellow_WallCrush()]->isLeft)
				{
					PlayerYellow.Position.x += Bounce_Power_x;
				}
				else
				{
					PlayerYellow.Position.x -= Bounce_Power_x;
				}
			}
		}
		else if (!WhatKey[VK_LEFT] && !WhatKey[VK_DOWN] && WhatKey[VK_RIGHT])
		{
			PlayerYellow.isLeft = FALSE;
			PlayerYellow.DS = drawState::Run;
			PlayerYellow.Position.x += Stand_xMove;

			if (Yellow_WallCrush() != -1)
			{
				if (Wall[Yellow_WallCrush()]->isLeft)
				{
					PlayerYellow.Position.x += Bounce_Power_x;
				}
				else
				{
					PlayerYellow.Position.x -= Bounce_Power_x;
				}
			}
		}
		if (!Yellow_BlockCrush())
		{
			float i = (PlayerYellow.Box.left + PlayerYellow.Box.right) * 0.5f;
			if (!(i > Blocks[PlayerYellow.OnBoardNum]->Box.left && i < Blocks[PlayerYellow.OnBoardNum]->Box.right) || !Blocks[PlayerYellow.OnBoardNum]->isVisible)
			{
				PlayerYellow.State = state::Off;
				PlayerYellow.DS = drawState::Fall;
			}

		}

		if (PlayerYellow.State == state::On)
		{
			if (Yellow_BlockCrush(PlayerYellow.OnBoardNum))
			{
				float i = (PlayerYellow.Box.left + PlayerYellow.Box.right) * 0.5f;
				if (i > Blocks[PlayerYellow.OnBoardNum]->Box.left && i < Blocks[PlayerYellow.OnBoardNum]->Box.right)
				{
					PlayerYellow.Position.y -= Blocks[PlayerYellow.OnBoardNum]->Speed;
				}
			}
		}
	}

	if (WhatKey[VK_SPACE])
	{
		for (int i = 0; i < BlockNum; i++)
		{
			Blocks[i]->Push_Space();
		}
	}

	if (WhatKey[VK_RETURN] && System.canSkill && Score > 200)
	{
		//FMOD_Channel_Stop(Channel[1]);
		//FMOD_System_PlaySound(soundSystem, effectSound, NULL, 0, &Channel[1]);
		//FMOD_Channel_SetVolume(Channel[1], 0.1);
		Skill();
		System.canSkill = FALSE;
	}
	PuppleRepotision();
	YellowRepotision();
}

BOOL Pupple_BlockCrush(int num)
{
	if (num == -1)
	{
		for (int i = 0; i < BlockNum; i++)
		{
			int tmp = Blocks[i]->Collision(PlayerPupple.Box, PlayerPupple.isPupple);
			if (tmp)
			{
				PlayerPupple.OnBoardNum = tmp;
				if (Blocks[PlayerPupple.OnBoardNum]->CrushLeft)
				{
					Blocks[PlayerPupple.OnBoardNum]->CrushLeft = FALSE;
					PlayerPupple.Position.x -= Crush_Power_x;
					PlayerPupple.Position.y -= Crush_Power_y;
				}
				if (Blocks[PlayerPupple.OnBoardNum]->CrushRight)
				{
					Blocks[PlayerPupple.OnBoardNum]->CrushRight = FALSE;
					PlayerPupple.Position.x += Crush_Power_x;
					PlayerPupple.Position.y -= Crush_Power_y;
				}
				if (Blocks[PlayerPupple.OnBoardNum]->CrushDown)
				{
					Blocks[PlayerPupple.OnBoardNum]->CrushDown = FALSE;
					PlayerPupple.Position.y += Crush_Down_Power;
				}
				if (Blocks[PlayerPupple.OnBoardNum]->FallOut)
				{
					Blocks[PlayerPupple.OnBoardNum]->FallOut = FALSE;
					PlayerPupple.Position.y -= FallOut_Power;
				}
				if (Blocks[PlayerPupple.OnBoardNum]->canScore)
				{
					Blocks[PlayerPupple.OnBoardNum]->canScore = FALSE;
					Score += ScorePoint;
				}
				PuppleRepotision();
				return TRUE;
			}
		}
		return FALSE;
	}
	else if (num >= 0 && num < 100)
	{
		int tmp = Blocks[num]->Collision(PlayerPupple.Box, PlayerPupple.isPupple);
		if (tmp)
		{
			PlayerPupple.OnBoardNum = tmp;
			if (Blocks[PlayerPupple.OnBoardNum]->CrushLeft)
			{
				Blocks[PlayerPupple.OnBoardNum]->CrushLeft = FALSE;
				PlayerPupple.Position.x -= Crush_Power_x;
				PlayerPupple.Position.y -= Crush_Power_y;
			}
			if (Blocks[PlayerPupple.OnBoardNum]->CrushRight)
			{
				Blocks[PlayerPupple.OnBoardNum]->CrushRight = FALSE;
				PlayerPupple.Position.x += Crush_Power_x;
				PlayerPupple.Position.y -= Crush_Power_y;
			}
			if (Blocks[PlayerPupple.OnBoardNum]->CrushDown)
			{
				Blocks[PlayerPupple.OnBoardNum]->CrushDown = FALSE;
				PlayerPupple.Position.y += Crush_Down_Power;
			}
			if (Blocks[PlayerPupple.OnBoardNum]->FallOut)
			{
				Blocks[PlayerPupple.OnBoardNum]->FallOut = FALSE;
				PlayerPupple.Position.y -= FallOut_Power;
			}
			if (Blocks[PlayerPupple.OnBoardNum]->canScore)
			{
				Blocks[PlayerPupple.OnBoardNum]->canScore = FALSE;
				Score += ScorePoint;
			}
			PuppleRepotision();
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

BOOL Yellow_BlockCrush(int num)
{
	if (num == -1)
	{
		for (int i = 0; i < BlockNum; i++)
		{
			int tmp = Blocks[i]->Collision(PlayerYellow.Box, PlayerYellow.isPupple);
			if (tmp)
			{
				PlayerYellow.OnBoardNum = tmp;
				if (Blocks[PlayerYellow.OnBoardNum]->CrushLeft)
				{
					Blocks[PlayerYellow.OnBoardNum]->CrushLeft = FALSE;
					PlayerYellow.Position.x -= Crush_Power_x;
					PlayerYellow.Position.y -= Crush_Power_y;
				}
				if (Blocks[PlayerYellow.OnBoardNum]->CrushRight)
				{
					Blocks[PlayerYellow.OnBoardNum]->CrushRight = FALSE;
					PlayerYellow.Position.x += Crush_Power_x;
					PlayerYellow.Position.y -= Crush_Power_y;
				}
				if (Blocks[PlayerYellow.OnBoardNum]->CrushDown)
				{
					Blocks[PlayerYellow.OnBoardNum]->CrushDown = FALSE;
					PlayerYellow.Position.y += Crush_Down_Power;
				}
				if (Blocks[PlayerYellow.OnBoardNum]->FallOut)
				{
					Blocks[PlayerYellow.OnBoardNum]->FallOut = FALSE;
					PlayerYellow.Position.y -= FallOut_Power;
				}
				if (Blocks[PlayerPupple.OnBoardNum]->canScore)
				{
					Blocks[PlayerPupple.OnBoardNum]->canScore = FALSE;
					Score += ScorePoint;
				}
				YellowRepotision();
				return TRUE;
			}
		}
		return FALSE;
	}
	else if (num == 100)
	{

	}
	else if (num >= 0 && num < 100)
	{
		int tmp = Blocks[num]->Collision(PlayerYellow.Box, PlayerYellow.isPupple);
		if (tmp)
		{
			PlayerYellow.OnBoardNum = tmp;
			if (Blocks[PlayerYellow.OnBoardNum]->CrushLeft)
			{
				Blocks[PlayerYellow.OnBoardNum]->CrushLeft = FALSE;
				PlayerYellow.Position.x -= Crush_Power_x;
				PlayerYellow.Position.y -= Crush_Power_y;
			}
			if (Blocks[PlayerYellow.OnBoardNum]->CrushRight)
			{
				Blocks[PlayerYellow.OnBoardNum]->CrushRight = FALSE;
				PlayerYellow.Position.x += Crush_Power_x;
				PlayerYellow.Position.y -= Crush_Power_y;
			}
			if (Blocks[PlayerYellow.OnBoardNum]->CrushDown)
			{
				Blocks[PlayerYellow.OnBoardNum]->CrushDown = FALSE;
				PlayerYellow.Position.y += Crush_Down_Power;
			}
			if (Blocks[PlayerYellow.OnBoardNum]->FallOut)
			{
				Blocks[PlayerYellow.OnBoardNum]->FallOut = FALSE;
				PlayerYellow.Position.y -= FallOut_Power;
			}
			if (Blocks[PlayerPupple.OnBoardNum]->canScore)
			{
				Blocks[PlayerPupple.OnBoardNum]->canScore = FALSE;
				Score += ScorePoint;
			}
			YellowRepotision();
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

int Pupple_WallCrush()
{
	for (int i = 0; i < WallBlockNum; i++)
	{
		if (IntersectRect(&BothCrushCheck, &PlayerPupple.Box, &Wall[i]->Box))
		{
			return i;
		}
	}
	return -1;
}

int Yellow_WallCrush()
{
	for (int i = 0; i < WallBlockNum; i++)
	{
		if (IntersectRect(&BothCrushCheck, &PlayerYellow.Box, &Wall[i]->Box))
		{
			return i;
		}
	}
	return -1;
}

BOOL ClientCrush()
{
	if (PlayerPupple.Box.bottom < Window_Clinet_Top || PlayerPupple.Box.top > Window_Clinet_Bottom ||
		PlayerYellow.Box.bottom < Window_Clinet_Top || PlayerYellow.Box.top > Window_Clinet_Bottom)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void Position_Compute(BOOL isYellow)
{
	if (System.isStart)
	{
		if (isYellow)
		{
			PlayerYellow.Accelerate += Gravity;
			PlayerYellow.Speed += PlayerYellow.Accelerate;
			PlayerYellow.Position.y += PlayerYellow.Speed;
		}
		else
		{
			PlayerPupple.Accelerate += Gravity;
			PlayerPupple.Speed += PlayerPupple.Accelerate;
			PlayerPupple.Position.y += PlayerPupple.Speed;
		}

		PuppleRepotision();
		YellowRepotision();
	}
}

void CreateBlockPosition()
{
	int x, y, tmp;

	for (int i = 0; i < ShakeNum; i++)
	{
		x = CreateBlockNum(gen);
		y = CreateBlockNum(gen);

		if (x != y)
		{
			tmp = StoreNum[y];
			StoreNum[y] = StoreNum[x];
			StoreNum[x] = tmp;
		}
	}
}

void CheckNow()
{
	if (CountNum.NowNormal > NormalBlockNum)
	{
		CountNum.NowNormal = 1;
	}
	if (CountNum.NowPupple > NormalBlockNum + ColorBlockNum_Pupple)
	{
		CountNum.NowPupple = NormalBlockNum + 1;
	}
	if (CountNum.NowYellow > NormalBlockNum + ColorBlockNum_Pupple + ColorBlockNum_Yellow)
	{
		CountNum.NowYellow = NormalBlockNum + ColorBlockNum_Pupple + 1;
	}
	if (CountNum.NowBreak > NormalBlockNum + ColorBlockNum_Pupple + ColorBlockNum_Yellow + BreakBlockNum)
	{
		CountNum.NowBreak = NormalBlockNum + ColorBlockNum_Pupple + ColorBlockNum_Yellow + 1;
	}
	if (CountNum.NowThorn > BlockNum - 1)
	{
		CountNum.NowThorn = NormalBlockNum + ColorBlockNum_Pupple + ColorBlockNum_Yellow + BreakBlockNum + 1;
	}
	if (CountNum.NowDaggar > NormalBlockNum + ColorBlockNum_Pupple + ColorBlockNum_Yellow + BreakBlockNum + ThornBlockNum)
	{
		CountNum.NowDaggar = NormalBlockNum + ColorBlockNum_Pupple + ColorBlockNum_Yellow + BreakBlockNum + ThornBlockNum + 1;
	}
}

void End(HWND hWnd)
{
	for (int i = 0; i < BlockNum; i++)
	{
		if (Blocks[i]->End)
		{
			KillTimer(hWnd, 1);
			System.GameOver = TRUE;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
	}

	if (ClientCrush())
	{
		KillTimer(hWnd, 1);
		System.GameOver = TRUE;
		InvalidateRect(hWnd, NULL, FALSE);
	}	

	if (Score == TargetCount)
	{
		KillTimer(hWnd, 1);
		System.Clear = TRUE;
		InvalidateRect(hWnd, NULL, FALSE);
	}
}

void Skill()
{
	Score -= 200;
	for (int i = 0; i < BlockNum; i++)
	{
		if (i > NormalBlockNum && i < NormalBlockNum + ColorBlockNum_Pupple + ColorBlockNum_Yellow + BreakBlockNum && Blocks[i]->yPos > 0)
		{
			Blocks[CountNum.NowNormal]->isVisible = TRUE;
			Blocks[CountNum.NowNormal++]->SetPosition(Blocks[i]->xPos, Blocks[i]->yPos);
			Blocks[i]->SetPosition(-100, -100);
			CheckNow();
		}
		if (i > NormalBlockNum + ColorBlockNum_Pupple + ColorBlockNum_Yellow + BreakBlockNum && Blocks[i]->yPos > 0)
		{
			Blocks[i]->SetPosition(-100, -100);
			Blocks[i]->isVisible = FALSE;
		}
	}
}

void ShakeMap(int Count)
{
	for (int i = 0; i < BlockNum; i++)
	{
		if (0 <= Count % 15 && Count % 15 < 3 && Blocks[i]->yPos > 0)
		{
			Blocks[i]->SetPosition(Blocks[i]->xPos - 10, Blocks[i]->yPos);
		}
		else if (3 <= Count % 15 && Count % 15 < 6 && Blocks[i]->yPos > 0)
		{
			Blocks[i]->SetPosition(Blocks[i]->xPos + 15, Blocks[i]->yPos);
		}
		else if (6 <= Count % 15 && Count % 15 < 9 && Blocks[i]->yPos > 0)
		{
			Blocks[i]->SetPosition(Blocks[i]->xPos - 7, Blocks[i]->yPos);
		}
		else if (9 <= Count % 15 && Count % 15 < 12 && Blocks[i]->yPos > 0)
		{
			Blocks[i]->SetPosition(Blocks[i]->xPos + 3, Blocks[i]->yPos);
		}
		else if (12 <= Count % 15 && Count % 15 < 15 && Blocks[i]->yPos > 0)
		{
			Blocks[i]->SetPosition(Blocks[i]->xPos - 1, Blocks[i]->yPos);
		}
	}
}

void _Reset(HWND hWnd)
{
	if (System.Reset)
	{
		System.Reset = FALSE;

		System.GameOver = FALSE;
		System.canSkill = TRUE;
		System.isStart = FALSE;

		PlayerPupple.isPupple = TRUE;
		PlayerPupple.Position = { 30,600 };
		PlayerPupple.isLeft = FALSE;
		PlayerPupple.State = state::Off;
		PlayerPupple.DS = drawState::Fall;
		PlayerPupple.HowJumpCount = 0;
		PlayerPupple.Accelerate = 0;
		PlayerPupple.Speed = 0;

		PlayerYellow.isPupple = FALSE;
		PlayerYellow.Position = { 750,600 };
		PlayerYellow.isLeft = TRUE;
		PlayerYellow.State = state::Off;
		PlayerYellow.DS = drawState::Fall;
		PlayerYellow.HowJumpCount = 0;
		PlayerYellow.Accelerate = 0;
		PlayerYellow.Speed = 0;

		PuppleRepotision();
		YellowRepotision();

		Background[3].yPos = 0;

		Blocks[0]->SetPosition(-100, -100);
		Blocks[1]->SetPosition(30, 800);
		Blocks[2]->SetPosition(Window_Clinet_Right - WallBlock_Size - Block_Size_Lenght, 800);
		Blocks[1]->isVisible = TRUE;
		Blocks[2]->isVisible = TRUE;
		Blocks[1]->canScore = FALSE;
		Blocks[2]->canScore = FALSE;
		for (int i = 3; i < BlockNum; i++)
		{
			Blocks[i]->SetPosition(-100, -100);
		}

		for (int i = 0; i < BlockNum; i++)
		{
			Blocks[i]->End = FALSE;
		}

		CountNum.NowNormal = 3;
		CountNum.NowPupple = NormalBlockNum + 1;
		CountNum.NowYellow = NormalBlockNum + 1 + ColorBlockNum_Pupple;
		CountNum.NowBreak = NormalBlockNum + 1 + ColorBlockNum_Pupple + ColorBlockNum_Yellow;
		CountNum.NowThorn = NormalBlockNum + 1 + ColorBlockNum_Pupple + ColorBlockNum_Yellow + BreakBlockNum;
		CountNum.NowDaggar = NormalBlockNum + 1 + ColorBlockNum_Pupple + ColorBlockNum_Yellow + BreakBlockNum + ThornBlockNum;

		isSpacePupple = TRUE;
		Score = 0;
		SetTimer(hWnd, 1, 60, NULL);

		//FMOD_Channel_Stop(Channel[0]);
		//FMOD_System_PlaySound(soundSystem, bgmsound[0], NULL, 0, &Channel[0]);
		//FMOD_Channel_SetVolume(Channel[0], 0.05);
	}
}

void Sound_Setup()
{
	//FMOD_System_Create(&soundSystem,0);
	//FMOD_System_Init(soundSystem, 10, FMOD_INIT_NORMAL, NULL);

	//FMOD_System_CreateSound(soundSystem, "Resource/Music/Main_Theme.mp3", FMOD_LOOP_NORMAL, 0, &bgmsound[0]);
	//FMOD_System_CreateSound(soundSystem, "Resource/Music/Death.mp3", FMOD_LOOP_OFF, 0, &bgmsound[1]);
	//FMOD_System_CreateSound(soundSystem, "Resource/Music/Clear.wav", FMOD_LOOP_OFF, 0, &bgmsound[2]);
	//FMOD_System_CreateSound(soundSystem, "Resource/Effect/Skill.mp3", FMOD_LOOP_OFF, 0, &effectSound);
}