#include "stdafx.h"
// 
// Ÿ��Ʋ �� ���� include�� ����
#include "Scene_Main.h"
#include "Scene_CharSel.h"
#include "Scene_Ingame.h"

// 
#include "OBJECT_Player.h"
#include "Framework.h"

using namespace std;

template<typename T>
T GetUserDataPtr(HWND hWnd)
{
	return reinterpret_cast<T>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
}
void SetUserDataPtr(HWND hWnd, LPVOID ptr)
{
	LONG_PTR result = ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(ptr));
}

CFramework::CFramework()
{
	m_ticker = new CGameTimer();
	m_ticker->Start();
}

CFramework::~CFramework()
{
	OnDestroy();
	m_ticker->Stop();
}

bool CFramework::OnCreate(HINSTANCE hInstance, HWND hWnd, const RECT & rc) //�����ڿ��� �ؾ��� ���� �ϴµ� ���� Ŭ���� �����̹Ƿ� �����ڸ� ���� �ȵǱ� ����
{
	srand((unsigned int)time(NULL));
	m_hInstance = hInstance;
	m_hWnd = hWnd;
	GetClientRect(hWnd, &m_rcClient);
	// Ŭ���̾�Ʈ ��ǥ �ʱ�ȭ
	m_rcClient.right -= m_rcClient.left;
	m_rcClient.bottom -= m_rcClient.top;
	m_rcClient.left = 0;
	m_rcClient.top = 0;

	//
	// ���� ����
	CreatebackBuffer();

	//�÷��̾� ����
	//BuildPlayer();
	p1.top = 0; p1.left = 0; p1.right = m_rcClient.right / 2; p1.bottom = m_rcClient.bottom;
	p2.top = 0; p2.left = m_rcClient.right / 2; p2.right = m_rcClient.right; p2.bottom = m_rcClient.bottom;
	// Ŭ������ ������ ���ν��� ����
	::SetUserDataPtr(m_hWnd, this);

	// ...

	// ĸ�� ����
	lstrcpy(m_CaptionTitle, TITLESTRING);

#if defined(SHOW_CAPTIONFPS)
	lstrcat(m_CaptionTitle, TEXT("("));
#endif
	m_TitleLength = lstrlen(m_CaptionTitle);
	SetWindowText(m_hWnd, m_CaptionTitle);

	// Ÿ�̸� �ʱ�ȭ
//	m_LastUpdate_time = chrono::system_clock::now(); // �������� �ʾҴ�.
	m_current_time = chrono::system_clock::now();
	m_fps = 0;

	// �� ����
	BuildScene();


	// ������ ���� �����ΰ�?
	ChangeScene(CScene::SceneTag::Main_Lobby);

	return (m_hWnd != NULL);
}

RECT CFramework::GetRect()
{
	return m_rcClient;
}

HDC* CFramework::GetTileDC()
{
	return &m_tileMemDC;
}

HDC* CFramework::GetPlayerDC()
{
	return &m_PlayerMemDC;
}

CObject_Player* CFramework::GetPlayer(int PlayerNum)
{
	switch (PlayerNum)
	{
	case 1:
		return player1;
		break;
	case 2:
		return player2;
		break;
	default:
		break;
	}
}

HDC CFramework::GetTotalDC()
{
	return m_totalBackDC;
}

void CFramework::CreatebackBuffer() //backBuffer �ʱ�ȭ
{
	if (m_totalBackDC)
	{
		::SelectObject(m_totalBackDC, NULL);
		::DeleteDC(m_totalBackDC);
	}
	if (m_totalBackBitmap) ::DeleteObject(m_totalBackBitmap);

	HDC hdc = ::GetDC(m_hWnd);
	m_totalBackDC = ::CreateCompatibleDC(hdc);	// ȣȯ�� �����ִ� �Լ�
	// ������ ��ũ ���ο� ����۸� �����. hdc�� ȣȯ�� ��Ʈ���� ����Ѵ�.
	m_totalBackBitmap = ::CreateCompatibleBitmap(hdc, m_rcClient.right, m_rcClient.bottom);
	::SelectObject(m_totalBackDC, m_totalBackBitmap);

	m_PlayerMemDC = CreateCompatibleDC(hdc);
	m_PlayerMemBitmap = CreateCompatibleBitmap(hdc, 6400, 6400);
	::SelectObject(m_PlayerMemDC, (HBITMAP)m_PlayerMemBitmap);

	m_tileMemDC = CreateCompatibleDC(m_PlayerMemDC);
	m_tileMemBitmap = CreateCompatibleBitmap(hdc, 6400, 6400);
	::SelectObject(m_tileMemDC, (HBITMAP)m_tileMemBitmap);

	SetBKColor(RGB(255, 255, 255));

	ClearBackgroundColor();

	::ReleaseDC(m_hWnd, hdc);
}

void CFramework::BuildScene()
{
	// arrScene[SceneTag::Title] = new TitleScene();	// �̷� ������� ���� ������.
	arrScene[CScene::SceneTag::Main_Lobby] = new CMainScene(CScene::SceneTag::Main_Lobby, this);
	arrScene[CScene::SceneTag::Select_Char] = new Scene_Charsel(CScene::SceneTag::Select_Char, this);
	arrScene[CScene::SceneTag::Ingame] = new CIngameScene(CScene::SceneTag::Ingame, this);
	arrScene[CScene::SceneTag::Main_Lobby]->OnCreate();
}

void CFramework::BuildPlayer(int p1, int p2)
{
	while (player1 == NULL)
		player1 = new CObject_Player();
	while (player2 == NULL)
		player2 = new CObject_Player();
	//printf("%d\n", player1->x);
	player1->OnCreate(p1);
	player1->x = 35 * 64;
	player1->y = 15 * 64;
	player2->x = 58 * 64;
	player2->y = 15 * 64;
	//printf("%d\n", player1->x);
	player2->OnCreate(p2);
}

void CFramework::ReleaseScene() // �� ��������
{

}

bool CFramework::OnDestroy() // Framework �ı� (������� ����)
{
	if (m_hbrBackground) ::DeleteObject(m_hbrBackground);

	::SelectObject(m_totalBackDC, NULL);
	if (m_totalBackBitmap)  ::DeleteObject(m_totalBackBitmap);
	if (m_totalBackDC) ::DeleteDC(m_totalBackDC);
	return false;
}

bool CFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		return true;
	case WM_CHAR:
		return true;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);	// ESC Ű�� ������ �� �����츦 �����Ѵ�.
			break;
		}
		return true;
	}
	return false;
}

bool CFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONUP:
		break;
	case WM_LBUTTONDOWN:
		break;

	case WM_MOUSEMOVE:
		break;

	}
	return false;
}

HRESULT CFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_SIZE:	// ������ ũ�⸦ �����ϴ� ���� ���� ���´�.
		break;
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			//��Ȱ��ȭ ó��
			inactive = true;
		}
		else
		{
			//Ȱ��ȭ ó��
			inactive = false;
		}
		break;
	default:
		return ::DefWindowProc(hWnd, nMessageID, wParam, lParam);
	}
	//	return E_NOTIMPL;
	return 0;
}

void CFramework::Update(float fTimeElapsed)
{
	m_pCurrScene->Update(fTimeElapsed);
}

void CFramework::SetBKColor(COLORREF color)
{
	m_clrBackBuffer = color;
	if (m_hbrBackground) ::DeleteObject(m_hbrBackground);
	m_hbrBackground = ::CreateSolidBrush(m_clrBackBuffer);	// �� ����� �귯���� ������.
}

void CFramework::ClearBackgroundColor()
{
	::FillRect(m_totalBackDC, &m_rcClient, m_hbrBackground);	// ���� ������� ��׶��忡 ���� Ŭ���̾�Ʈ��ŭ�� ������ ��� ���� �귯���� ������ ��ĥ�ض�.
}

void CFramework::PreprocessingForDraw()
{
	ClearBackgroundColor();	// �׸��� ������ ���۵Ǹ� ������ �׶��带 �����ϰ� �����.
	::SetBkColor(m_totalBackDC, TRANSPARENT);	// ���ĺ��� ��� �����ϰ� ����
	::SetStretchBltMode(m_totalBackDC, COLORONCOLOR);	//���� ������ �޶� �þ�ų� �پ�� ������ �ִ� ��� �����.

	m_pCurrScene->Render(m_totalBackDC);
}

void CFramework::OnDraw(HDC hDC)
{
	// ���ڷ� �޴� hDC�� �ۿ��� ������� hDC�̴�. �� ��Ʈ�ʿ� m_hDC�� ����ۿ��� ������� �׸��� �Ѹ���.
	::BitBlt(hDC, m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom, m_totalBackDC, 0, 0, SRCCOPY);
}

void CFramework::FrameAdvance()
{
	// FPS ����

	//m_timeElapsed = chrono::system_clock::now() - m_current_time;
	//{
	//	if (m_timeElapsed.count() > MAX_FPS) // MAX_FPS�� 0 �̻��̸� ���α׷� ��� ����?
	//	{
	//		m_current_time = chrono::system_clock::now();
	//		if (m_timeElapsed.count() > 0.0)
	//			m_fps = 1.0 / m_timeElapsed.count();
	//	}
	//	else return;
	//}
	HDC hdc = ::GetDC(m_hWnd);	// �� ����̽� ���ؽ�Ʈ�� ����ϰڴ�.

	m_ticker->Tick(60.0f);
	m_fps = 1.0 / m_ticker->GetTimeElapsed();

	if (inactive == false)
	{
		Update(m_ticker->GetTimeElapsed()); // �׷��� �� �� ������Ʈ(�����Ӵ�)
		PreprocessingForDraw(); // 
		// ����� �����̹Ƿ� OnDraw�� �ƴϴ�. OnDraw ������ ����ۿ� �׷��ִ� ������ �Ѵ�.
		this->OnDraw(hdc); //Paint�� �� ������ �׳� �׸���
	}
	// ��ĸ�ǿ� ���ڸ� �� ������ �����ϴ� ĸ�� ��Ʈ�� ����
	{
		_itow_s(
			m_fps + 0.1f
			, m_CaptionTitle + m_TitleLength
			, TITLE_MX_LENGTH - m_TitleLength
			, 10);
		wcscat_s(
			m_CaptionTitle + m_TitleLength
			, TITLE_MX_LENGTH - m_TitleLength
			, TEXT("FPS )"));
		SetWindowText(m_hWnd, m_CaptionTitle);
	}


	::ReleaseDC(m_hWnd, hdc); // �׸��� �����.
}

LRESULT CFramework::WndProc(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	CFramework* self = ::GetUserDataPtr<CFramework*>(hWnd);	// static �Լ��� �����Ϸ��� ������ ������. Ŭ������ �����Լ��� ����ϱ� ���� �ļ�
	if (!self)
		return ::DefWindowProc(hWnd, nMessageID, wParam, lParam);	// �޽��� ó���� OS���� �ѱ��.

	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:

	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:

	case WM_MOUSEMOVE:
		self->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);

		break;


	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
		self->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);

		break;

	case WM_PAINT:
	{
		//WM_CHAR���� �켱������ ���Ƽ� ������� ����.
	}
	break;
	case WM_DESTROY:
		::SetUserDataPtr(hWnd, NULL);
		::PostQuitMessage(0);
		break;

	default:
		return self->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam);
	}
	return 0;
	//	return LRESULT();
}

void CFramework::curSceneCreate()
{
	m_pCurrScene->OnCreate();
}

void CFramework::ChangeScene(CScene::SceneTag tag)
{
	m_pCurrScene = arrScene[tag];
	//m_pCurrScene->OnCreate();
}