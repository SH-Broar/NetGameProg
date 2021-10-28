#include <windows.h> // 윈도우 헤더 파일
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <atlimage.h>
#include "o_USERINTERFACE.h"
#include "o_Ingame.h"

//윈도우 창의 크기입니다.
int windowX = ::GetSystemMetrics(SM_CXSCREEN); //모니터 x길이
int windowY = ::GetSystemMetrics(SM_CYSCREEN); //모니터 y길이

RECT clientRECT;
//게임이 나오는 스크린 창 입니다.
#define screenX 1920
#define screenY 1080

HINSTANCE g_hInst;
LPCTSTR lpszClass = _T("Window Class Name");

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

USER_INTERFACE UI;
HDC hDC;
PlayerData player1, player2;
BOOL keydown;
BOOL keydownList[14] = { 0 }; // 0 1 2 3 p2 이동 4 5 6 p2 공격 스킬 대시 7 8 9 10 p1 이동 11 12 13 p1 공격 스킬 대시
int Tileindex[100][100];

void IngameMovementManager() //속도때문에 그냥 나열함
{
	// 0 1 2 3 p1 이동 4 5 6 p1 공격 스킬 대시 7 8 9 10 p2 이동 11 12 13 p2 공격 스킬 대시
	if (keydownList[1])
	{
		if (Tileindex[player2.x / 64][(player2.y + 60) / 64] == 1)
		{
			player2.y -= 25;
		}
		else if (Tileindex[player2.x / 64][(player2.y + 60 - 20) / 64] == 2)
		{

		}
		else
		{
			player2.y -= 10;
		}
		player2.CharacterStatus = 3;
		player2.isWalk = TRUE;
		player2.WalkingTimerTick = 0;
		player2.WalkingImageTick++;
	}
	if (keydownList[0])
	{
		if (Tileindex[player2.x / 64][(player2.y + 60) / 64] == 1)
		{
			player2.x -= 25;
		}
		else if (Tileindex[(player2.x - 30) / 64][(player2.y + 60) / 64] == 2)
		{

		}
		else
		{
			player2.x -= 10;
		}
		player2.CharacterStatus = 2;
		player2.isWalk = TRUE;
		player2.WalkingTimerTick = 0;
		player2.WalkingImageTick++;
	}
	if (keydownList[3])
	{
		if (Tileindex[player2.x / 64][(player2.y + 60) / 64] == 1)
		{
			player2.y += 25;
		}
		else if (Tileindex[player2.x / 64][(player2.y + 60 + 30) / 64] == 2)
		{

		}
		else
		{
			player2.y += 10;
		}
		player2.CharacterStatus = 5;
		player2.isWalk = TRUE;
		player2.WalkingTimerTick = 0;
		player2.WalkingImageTick++;
	}
	if (keydownList[2])
	{
		if (Tileindex[player2.x / 64][(player2.y + 60) / 64] == 1)
		{
			player2.x += 25;
		}
		else if (Tileindex[(player2.x + 30) / 64][(player2.y + 60) / 64] == 2)
		{

		}
		else
		{
			player2.x += 10;
		}
		player2.CharacterStatus = 4;
		player2.isWalk = TRUE;
		player2.WalkingTimerTick = 0;
		player2.WalkingImageTick++;
	}
	
	if (keydownList[4])
	{
		
	}
	if (keydownList[5])
	{

	}
	if (keydownList[6])
	{

	}
	if (keydownList[8])
	{
		if (Tileindex[player1.x / 64][(player1.y + 60) / 64] == 1)
		{
			player1.y -= 25;
		}
		else if (Tileindex[player1.x / 64][(player1.y + 60 - 30) / 64] == 2)
		{

		}
		else
		{
			player1.y -= 10;
		}
		player1.CharacterStatus = 3;
		player1.isWalk = TRUE;
		player1.WalkingTimerTick = 0;
		player1.WalkingImageTick++;
	}
	if (keydownList[7])
	{
		if (Tileindex[player1.x / 64][(player1.y + 60) / 64] == 1)
		{
			player1.x -= 25;
		}
		else if (Tileindex[(player1.x - 30) / 64][(player1.y + 60) / 64] == 2)
		{

		}
		else
		{
			player1.x -= 10;
		}
		player1.CharacterStatus = 2;
		player1.isWalk = TRUE;
		player1.WalkingTimerTick = 0;
		player1.WalkingImageTick++;
	}
	if (keydownList[10])
	{
		if (Tileindex[player1.x / 64][(player1.y + 60) / 64] == 1)
		{
			player1.y += 25;
		}
		else if (Tileindex[player1.x / 64][(player1.y + 60 + 30) / 64] == 2)
		{

		}
		else
		{
			player1.y += 10;
		}
		player1.CharacterStatus = 5;
		player1.isWalk = TRUE;
		player1.WalkingTimerTick = 0;
		player1.WalkingImageTick++;
	}
	if (keydownList[9])
	{
		if (Tileindex[player1.x / 64][(player1.y + 60) / 64] == 1)
		{
			player1.x += 25;
		}
		else if (Tileindex[(player1.x + 30) / 64][(player1.y + 60) / 64] == 2)
		{

		}
		else
		{
			player1.x += 10;
		}
		player1.CharacterStatus = 4;
		player1.isWalk = TRUE;
		player1.WalkingTimerTick = 0;
		player1.WalkingImageTick++;
	}
	
	if (keydownList[11])
	{

	}
	if (keydownList[12])
	{

	}
	if (keydownList[13])
	{

	}
}

BOOL IngameKeydownManager()
{
	keydown = FALSE;
	for (int i = 0; i < 14; i++)
	{
		keydownList[i] = FALSE;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		keydownList[1] = TRUE;
		
		keydown = TRUE;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		keydownList[0] = TRUE;
		
		keydown = TRUE;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		keydownList[3] = TRUE;
		
		keydown = TRUE;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		keydownList[2] = TRUE;
		
		keydown = TRUE;
	}

	if (GetAsyncKeyState(0x46) & 0x8000) // f
	{
		keydownList[7] = TRUE;
		
		keydown = TRUE;
	}
	if (GetAsyncKeyState(0x54) & 0x8000) // t
	{
		keydownList[8] = TRUE;
		
		keydown = TRUE;
	}
	if (GetAsyncKeyState(0x48) & 0x8000) // h
	{
		keydownList[9] = TRUE;
		
		keydown = TRUE;
	}
	if (GetAsyncKeyState(0x47) & 0x8000) // g
	{
		keydownList[10] = TRUE;
		
		keydown = TRUE;
	}

	if (keydown)
		return TRUE;
}

int WalkingMessageTick = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) //메인
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, L"Run over with the coin", WS_OVERLAPPEDWINDOW, 0, 0, windowX, windowY, NULL, (HMENU)NULL, hInstance, NULL);
	//전체화면 
	ShowWindow(hWnd, SW_MAXIMIZE);
	//ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (1)
	{
		if (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
				break;
			TranslateMessage(&Message);
			DispatchMessage(&Message);
			
			if (WalkingMessageTick == 0)
			{
				if (IngameKeydownManager())
				{
					IngameMovementManager();
				}
			}
		}
		else
		{
			if (UI.returnScene() == INGAME)
			{
				++WalkingMessageTick;
				
				WalkingMessageTick %= 25;
			}
		}
	}
	return Message.wParam;
}

CImage sample; // 이미지를 관리하는 클래스.

CImage C_TITLE;
CImage C_MainLobby_BG, C_MainLobby_START[2], C_MainLobby_TIP[2], C_MainLobby_EXIT[2];
CImage C_TIP;

//
CImage C_Tile[3];
CImage C_Numbers[10];
//


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static WCHAR text[100];
	WCHAR LoadText[100];
	switch (iMessage)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &clientRECT);

		sample.Load(L"Sample\\Laharl_alphaTest.png"); //이미지 연결. 전용 DC와 Bitmap이 연결되어있는 상태.
		//타이틀 이미지를 로드합시다.
		C_TITLE.Load(L"Graphic\\UI\\Title.jpg");

		//메인 로비 이미지를 로드 합시다.
		C_MainLobby_BG.Load(L"Graphic\\UI\\MainLobby_BG.jpg");
		C_MainLobby_START[0].Load(L"Graphic\\UI\\START.png");
		C_MainLobby_START[1].Load(L"Graphic\\UI\\START2.png");

		C_MainLobby_TIP[0].Load(L"Graphic\\UI\\TIP.png");
		C_MainLobby_TIP[1].Load(L"Graphic\\UI\\TIP2.png");

		C_Tile[0].Load(L"Graphic\\Tile\\Tile1.jpg");
		C_Tile[1].Load(L"Graphic\\Tile\\Tile2.png");
		C_Tile[2].Load(L"Graphic\\Tile\\Tile3.png");

		for (int i = 0; i < 10; i++)
		{
			wsprintf(LoadText, L"Graphic\\Numbers\\%d.png", i);
			C_Numbers[i].Load(LoadText);
		}

		C_MainLobby_EXIT[0].Load(L"Graphic\\UI\\EXIT.png");
		C_MainLobby_EXIT[1].Load(L"Graphic\\UI\\EXIT2.png");

		C_TIP.Load(L"Graphic\\UI\\HINT.jpg");
		break;
	case WM_PAINT: //Paint 메세지 불렸을 때
		hDC = BeginPaint(hWnd, &ps);

		//Rectangle(hDC, clientRECT.left, clientRECT.top, clientRECT.right, clientRECT.bottom);

		//sample.Draw(hDC, clientRECT);																//Cimage를 잘 못그릴때는 Draw해주면 알아서 그려준다


		switch (UI.returnScene())
		{
		case TITLE:
			C_TITLE.Draw(hDC, clientRECT);
			break;
		case MAIN_LOBBY:
			C_MainLobby_BG.Draw(hDC, clientRECT);
			C_MainLobby_TIP[0].Draw(hDC, UI.GetRect(TIP));
			C_MainLobby_START[0].Draw(hDC, UI.GetRect(START));
			C_MainLobby_EXIT[0].Draw(hDC, UI.GetRect(EXIT));

			switch (UI.ReturnChoice()) {
			case TIP:
				C_MainLobby_TIP[1].Draw(hDC, UI.GetRect(TIP));
				break;
			case START:
				C_MainLobby_START[1].Draw(hDC, UI.GetRect(START));
				break;
			case EXIT:
				C_MainLobby_EXIT[1].Draw(hDC, UI.GetRect(EXIT));
				break;
			}

			break;
		case CONTROL_TIP:
			C_TIP.Draw(hDC, clientRECT);
			break;
		case SELECT_MODE:
			wsprintf(text, L"%d", UI.returnScene());
			TextOut(hDC, clientRECT.right / 2, clientRECT.bottom / 2, text, 1);
			break;
		case SELECT_CHAR:
			wsprintf(text, L"%d", UI.returnScene());
			TextOut(hDC, clientRECT.right / 2, clientRECT.bottom / 2, text, 1);
			break;
		case INGAME:
		{
			//----------------------------------인게임 초기화
			RECT p1, p2;
			static HDC TotalMemDC, TileMemDC, PlayerMemDC;
			static HBITMAP TotalMemBitmap, TileMemBitmap, PlayerMemBitmap;
			static BOOL Tileset = FALSE;

			p1.top = 0; p1.left = 0; p1.right = clientRECT.right / 2; p1.bottom = clientRECT.bottom;
			p2.top = 0; p2.left = clientRECT.right / 2; p2.right = clientRECT.right; p2.bottom = clientRECT.bottom;
			//--

			//--
			TotalMemDC = CreateCompatibleDC(hDC);
			TotalMemBitmap = CreateCompatibleBitmap(hDC, clientRECT.right, clientRECT.bottom);
			SelectObject(TotalMemDC, (HBITMAP)TotalMemBitmap);

			PlayerMemDC = CreateCompatibleDC(hDC);
			PlayerMemBitmap = CreateCompatibleBitmap(hDC, 6400, 6400);
			SelectObject(PlayerMemDC, (HBITMAP)PlayerMemBitmap);

			if (Tileset == FALSE) // 초기화하는 부분
			{
				PlayerImageLoad(&player1, 1); //1 -> 선택한 캐릭터 (캐릭터 선택창 만든 후 변경)
				PlayerImageLoad(&player2, 2); //2 -> 선택한 캐릭터 (캐릭터 선택창 만든 후 변경)
				SetTimer(hWnd, 1, 10, NULL);
				for (int i = 0; i < 100; i++)
				{
					for (int j = 0; j < 100; j++)
					{
						Tileindex[j][i] = 0;
						if (j > 20 && j < 40)
							Tileindex[j][i] = 1;
						if (j > 15 && j < 17 && i % 3 == 0)
							Tileindex[j][i] = 2;
					}
				}
				
				player1.x = 500; player1.y = 500;
				player2.x = 700; player2.y = 700;

				TileMemDC = CreateCompatibleDC(TotalMemDC);
				TileMemBitmap = CreateCompatibleBitmap(hDC, 6400, 6400);
				SelectObject(TileMemDC, (HBITMAP)TileMemBitmap);

				for (int i = 0; i < 100; i++)
				{
					for (int j = 0; j < 100; j++)
					{
						C_Tile[Tileindex[j][i]].BitBlt(TileMemDC, 64 * j, 64 * i, 64, 64, 0, 0, SRCCOPY); //타일의 가로세로크기가 64바이트
						Tileset = TRUE;
					}
				}
			}
			
			//------------------------------------맵 출력 TileMemDC -> PlayerMemDC
			BitBlt(PlayerMemDC, player1.x - p1.right / 2, player1.y - p1.bottom / 2, p1.right, p1.bottom, TileMemDC, player1.x - p1.right / 2, player1.y - p1.bottom / 2, SRCCOPY);
			BitBlt(PlayerMemDC, player2.x - p1.right / 2, player2.y - p1.bottom / 2, p1.right, p2.bottom, TileMemDC, player2.x - p1.right / 2, player2.y - p2.bottom / 2, SRCCOPY);
			//------------------------------------캐릭터 출력 PlayerMemDC
			
			if (player1.y > player2.y)
			{
				PlayerDraw(&player2, PlayerMemDC);
				PlayerDraw(&player1, PlayerMemDC);
			}
			else
			{
				PlayerDraw(&player1, PlayerMemDC);
				PlayerDraw(&player2, PlayerMemDC);
			}

			Ellipse(PlayerMemDC, player1.x - 5, player1.y - 5, player1.x + 5, player1.y + 5);
			Ellipse(PlayerMemDC, player2.x - 5, player2.y - 5, player2.x + 5, player2.y + 5);
			//------------------------------------캐릭터 total로 복사 PlayerMemDC -> TileMemDC
			BitBlt(TotalMemDC, p1.left, p1.top, p1.right, p1.bottom, PlayerMemDC, player1.x - p1.right / 2, player1.y - p1.bottom / 2, SRCCOPY);
			BitBlt(TotalMemDC, p2.left, p2.top, p1.right, p2.bottom, PlayerMemDC, player2.x - p1.right / 2, player2.y - p2.bottom / 2, SRCCOPY);
			//------------------------------------
			//Rectangle(hDC, clientRECT.left, clientRECT.top, clientRECT.right, clientRECT.bottom);

			BitBlt(hDC, 0, 0, clientRECT.right, clientRECT.bottom, TotalMemDC, 0, 0, SRCCOPY);

			DeleteObject(TotalMemDC); DeleteObject(PlayerMemDC); //DeleteObject(TileMemDC);
			DeleteObject(TotalMemBitmap); DeleteObject(PlayerMemBitmap); //DeleteObject(TileMemBitmap);

		}
		break;
		case SCOREBOARD:

			break;
		default:
			if (MessageBox(hWnd, _T("오류입니다. 초기위치로 돌아가시겠습니까?\n만약 NO를 누르면 게임이 종료됩니다."), _T("오류"), MB_YESNO)) {
				UI.ALLReset();
			}
			else {
				PostQuitMessage(0);
			}
			break;
		}

		//TextOut 텍스트 출력 함수

		EndPaint(hWnd, &ps);
		break;

	case WM_KEYDOWN:
		InvalidateRect(hWnd, NULL, false);
		switch (UI.returnScene())
		{
		case TITLE:
			//아무키나 입력하면 넘어감
			UI.nextScene();
			break;

		case MAIN_LOBBY:
			UI.nextScene();
			break;

		case CONTROL_TIP:
			UI.CallTip();
			break;

		case SELECT_MODE:
			UI.nextScene();
			break;

		case SELECT_CHAR:
			UI.nextScene();
			break;

		case INGAME:
			
			break;

		case SCOREBOARD:
			UI.nextScene();
			break;
		}
		break;
	case WM_CHAR:
		//InvalidateRect(hWnd, NULL, false);
		switch (UI.returnScene())
		{
		case TITLE:
			//아무키나 입력하면 넘어감
			UI.nextScene();
			break;

		case MAIN_LOBBY:

			break;

		case CONTROL_TIP:

			break;

		case SELECT_MODE:

			break;

		case SELECT_CHAR:

			break;

		case INGAME:

			//InvalidateRect(hWnd, NULL, false);
			break;
	case SCOREBOARD:

		break;
	}
	break;
	case WM_TIMER:
		if (wParam == 1) // InGame Movement Manager
		{
			if (Tileindex[player1.y / 64][player1.x / 64] == 2)
			{
				switch (player1.CharacterStatus)
				{

				}
			}
			if (Tileindex[player2.y / 64][player2.x / 64] == 2)
			{
				switch (player1.CharacterStatus)
				{

				}
			}

			if (player1.isWalk)
			{
				player1.WalkingTimerTick++;
				if (player1.WalkingTimerTick >= 3)
				{
					player1.WalkingTimerTick = 0;
					player1.isWalk = FALSE;
					switch (player1.CharacterStatus)
					{
					case 2:
					case 5:
					case 6:
						player1.CharacterStatus = 0;
						break;
					case 3:
					case 4:
					case 7:
						player1.CharacterStatus = 1;
						break;
					}
				}
				InvalidateRect(hWnd, NULL, false);
			}
			if (player2.isWalk)
			{
				player2.WalkingTimerTick++;
				if (player2.WalkingTimerTick >= 3)
				{
					player2.WalkingTimerTick = 0;
					player2.isWalk = FALSE;
					switch (player2.CharacterStatus)
					{
					case 2:
					case 5:
					case 6:
						player2.CharacterStatus = 0;
						break;
					case 3:
					case 4:
					case 7:
						player2.CharacterStatus = 1;
						break;
					}
				}
				InvalidateRect(hWnd, NULL, false);
			}


		}
		break;
	case WM_DESTROY: //Destroy 메세지 불렸을 때
		PostQuitMessage(0); //창 종료
		return 0;

	case WM_MOUSEMOVE:
		switch (UI.returnScene())
		{

		case MAIN_LOBBY:

			break;

		case CONTROL_TIP:

			break;

		case SELECT_MODE:

			break;

		case SELECT_CHAR:

			break;

		case INGAME:

			break;

		case SCOREBOARD:

			break;
		}
		break;
	case WM_LBUTTONDOWN:
		switch (UI.returnScene())
		{

		case TITLE:
			//클릭해도 넘어감
			UI.nextScene();
			break;

		case MAIN_LOBBY:

			break;

		case CONTROL_TIP:

			break;

		case SELECT_MODE:

			break;

		case SELECT_CHAR:

			break;

		case INGAME:

			break;

		case SCOREBOARD:

			break;
		}

		break;
	case WM_RBUTTONDOWN:

		break;

}
return(DefWindowProc(hWnd, iMessage, wParam, lParam)); //처리되지 않은 메세지는 여기서 처리
}