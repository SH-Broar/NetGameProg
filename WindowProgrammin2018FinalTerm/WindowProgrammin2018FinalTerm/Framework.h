#pragma once

#include <Windows.h>
#include <iostream>
#include "Scene.h"
#include "Timer.h"

class CObject_Player;
class CGameTimer;
// �������̽��̹Ƿ� header�� include �Ѵ�.
					// ������Ͽ��� �� �����ӿ�ũ���� ����ϴ� �������̽��� �� Ŭ������ �θ� �ܿ��� include���� �ʵ��� �Ѵ�.

//template<typename Enum> // enum class�� �������� �˷��ָ� ���ڿ� �����ϴ� ���� ��ȯ�ϴ� �Լ�
//inline constexpr auto GetFnumValueByType(Enum enumerator) noexcept	// enum class E : int {a, b, c}; �� ��,
//{																	// auto data = GetEnumValueByType(E::a);
//	return static_cast<std::underlying_type_t<enum>>(enummerator);	// data�� ������ int�̰�, ���� 0
//}
// enum class�� enum�� class�� ���� ���̴�.
// �̷����ϸ� enum�� �ڷ����� �Ǿ�����Ƿ� ���� enum class�ۿ� ���ִ´�.

// #define GetSceneEnumInt(Enum) GetEnumValueType(CScene::CurrentScene::Enum)	// ���� CScene�� �������� �ʾ����Ƿ� �ּ�ó��

class CFramework
{
private:
	HINSTANCE	m_hInstance{ NULL };

	HWND		m_hWnd{ NULL };
	RECT		m_rcClient{ 0, 0, 0, 0 };	// Ŭ���̾�Ʈ ũ��
	HDC			hhdc;
	//---------------------
	HBITMAP		m_totalBackBitmap = NULL;	// ��Ʈ�� �ڵ�. ���� ��¹��� ���� ���� ��µȴ�.
	HDC			m_totalBackDC = NULL;		// �������� �ڵ�
	HBITMAP		m_PlayerMemBitmap = NULL;   	// �÷��̾ ��µ� ��Ʈ��.
	HDC			m_PlayerMemDC = NULL;			// ��Ʈ���� �ڵ�
	HBITMAP		m_tileMemBitmap = NULL;   	// �ΰ��� Ÿ���� ��µ� ��Ʈ��.
	HDC			m_tileMemDC = NULL;			// ��Ʈ���� �ڵ�

	//---------------------
	COLORREF	m_clrBackBuffer = 0x00000000;	// ����� ����. �������� �ʱ�ȭ
	HBRUSH		m_hbrBackground = NULL;			// �׸��� ����. ��ĥ�� �� ����.

	bool inactive = false;
	CGameTimer*		m_ticker = NULL;
	
	CObject_Player* player1 = NULL , *player2 = NULL;

	std::chrono::system_clock::time_point m_current_time;
	std::chrono::duration<double> m_timeElapsed;	// �ð��� �󸶳� �����°�?
	double m_fps;

	TCHAR m_CaptionTitle[TITLE_MX_LENGTH];	// SetWindow �Լ����ٰ� �� Ÿ��Ʋ�� ����ִ´�.
	int m_TitleLength;
	//-------------------------------------------------------


public:
	CFramework();
	~CFramework();

	RECT p1, p2;
	bool OnCreate(HINSTANCE hInstance, HWND hWnd, const RECT &rc); // rc�� ������ũ���̰�, m_rcClient�� �����Ѵ�.
	RECT GetRect();
	HDC* GetTileDC();
	HDC* GetPlayerDC();
	CObject_Player* GetPlayer(int Playernum);
	HDC GetTotalDC();
	void CreatebackBuffer();	// HBITMAP�� �����.
	void BuildScene();
	void BuildPlayer(int p1, int p2);
	void ReleaseScene();	

	bool OnDestroy();	// HDC�� ���� ���۰� ������ ����� �Ѵ�.

	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	HRESULT OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void Update(float fTimeElapsed);
	
	void SetBKColor(COLORREF color);	// COLORREF ��������� �ٲ��ִ� �Լ�
	void ClearBackgroundColor();		// ����� �ʱ�ȭ
	void PreprocessingForDraw();		// ������ ����ۿ� ���������� �׸��°�
	void OnDraw(HDC hDC);				// Paint���� Bitblt �� �ϳ��� �Ѵ�.

//	void ChangeScene(CScene::CurrentScene tag, bool bDestroy = false);	// ���������� CScene�� �𸣹Ƿ� �ּ�ó��

	void FrameAdvance();				// Update�� PreProceessingForDraw�� ó���Ѵ�. InvalidateRect�� �ϰ� ĸ�ǵ� ���ְ�...
										// ������ �޽��� �Ʒ��� �Ҹ��� �Լ��� ���. ��� ������ �� �ȿ��� ó���Ǳ� �����̴�.

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void curSceneCreate();
	void ChangeScene(CScene::SceneTag tag);
private:
	CScene * arrScene[CScene::SceneTag::count];
	CScene * m_pCurrScene;
};