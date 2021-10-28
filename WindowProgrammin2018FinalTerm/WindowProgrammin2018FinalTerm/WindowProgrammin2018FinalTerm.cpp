// ���� ���α׷��� �������� �����մϴ�.
//
#include "stdafx.h"
#include "WindowProgrammin2018FinalTerm.h"
#include "Framework.h"

#define MAX_LOADSTRING	100
#define CLIENT_WIDTH	1920
#define CLIENT_HEIGHT	1080

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.
int windowX = ::GetSystemMetrics(SM_CXSCREEN);  //����� x����
int windowY = ::GetSystemMetrics(SM_CYSCREEN);  //����� y����
CFramework myFramework;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ���⿡ �ڵ带 �Է��մϴ�.

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINDOWPROGRAMMIN2018FINALTERM, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);	// Ŭ������ ����Ѵٴ� ���̴�. ������ Ŭ������ ���¸� �����ش�. 


								// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWPROGRAMMIN2018FINALTERM));

	MSG msg;
	// �⺻ �޽��� �����Դϴ�.
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))	// GetMessage(&msg, nullptr, 0, 0)
														// PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)
														// ���� ������ ����������� ����������, ���� Ŭ���Ҷ����� �����´�.
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			if (msg.message == WM_QUIT) break; // esc ������ ������ ����.
			if (msg.message != WM_MOUSEMOVE && msg.message != WM_CHAR)
			{
				TranslateMessage(&msg);	// �޽��� �̵�
				DispatchMessage(&msg);	// �޽��� �İ�
			}
		}
		// ���⼭ �������� ���� ���α׷��� ������ �Ѵ�.
		// FrameAdvance�� ���� ���α׷��� ������.
		myFramework.FrameAdvance();


	}

	return (int)msg.wParam;
}

//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;	// WND CLASS EX W == '������ Ŭ���� Ȯ�� �����ڵ�' ��� ���� ����ü�̴�.
						// { style , WndProc, hInstance, ClassName, return RegisterClassW }
						// style�� msdn ���� ���� �˾Ƽ� ������ �Ѵ�.
						// WndProc�� �ݹ� �Լ��̴�. Ȱ��ȭ�� Input���� Getmsg�� �޾Ƽ� �ҷ��´�.
						// hInstance�� main �ܼ��Լ������� �ִ�. GetModuleHandle(NULL)�Լ��ε� ������ �� �ִ�.
						// hInstance�� ���μ����� ���̵��̴�. ���̵� �����ͼ� ���α׷��� ���� �ĺ���ȣ�� ����Ѵ�.
						// ClassName�� �� �״�� Ŭ������ �̸��� �����Ѵ�.

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style =
		CS_HREDRAW		// Ŭ���̾�Ʈ�� �ʺ� �����ϸ�, ��ü �����츦 �ٽ� �׸��� �Ѵ�.	WM{_SIZE�� ���� ����.
		| CS_VREDRAW	// Ŭ���̾�Ʈ�� ���̸� �����ϸ�, ��ü �����츦 �ٽ� �׸��� �Ѵ�.
						//  | CS_DBLCLKS	// �ش� �����쿡�� ����Ŭ���� ����ؾ� �Ѵٸ� �߰��ؾ� �Ѵ�.
		;
	wcex.lpfnWndProc = CFramework::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_WINDOWPROGRAMMIN2018FINALTERM));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL; // MAKEINTRESOURCEW(IDC_CSTUDYWINDOW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	// ���� ������ �ڵ�
	HWND hWnd;

	// ������ ��Ÿ��
	DWORD dwStyle =
		WS_OVERLAPPED			// ����Ʈ ������. Ÿ��Ʋ �ٿ� ũ�� ������ �ȵǴ� ��輱�� ������. �ƹ��� ��Ÿ�ϵ� ���� ������ �� ��Ÿ���� ����ȴ�.
		| WS_CAPTION			// Ÿ��Ʋ �ٸ� ���� �����츦 ����� WS_BORDER ��Ÿ���� �����Ѵ�.
		| WS_SYSMENU			// �ý��� �޴��� ���� �����츦 �����.
		| WS_MINIMIZEBOX		// �ּ�ȭ ��ư�� �����.
		| WS_BORDER				// �ܼ����� �� ��輱�� ����� ũ�� ������ �� �� ����.
								//	  | WS_THICKFRAME		// ũ�� ������ ������ �β��� ��輱�� ������. WS_BORDER�� ���� ����� �� ����.
		;						// �߰��� �ʿ��� ������ ��Ÿ���� http://www.soen.kr/lecture/win32api/reference/Function/CreateWindow.htm ����

	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	RECT getWinSize;
	GetWindowRect(GetDesktopWindow(), &getWinSize);

	// Ŭ���̾�Ʈ ������
	RECT rc;
	
	rc.left = rc.top = 0;
	rc.right = windowX;
	rc.bottom = windowY;
	// ������ ����� ������ �߰��Ǵ� (ĸ��, �ܰ��� ��) ũ�⸦ ����.
	AdjustWindowRect(&rc, dwStyle, FALSE);

	// Ŭ���̾�Ʈ ���� ��ǥ(left, top)
	// ����ũ���� �߾ӿ� Ŭ���̾�Ʈ�� ��ġ�ϵ��� ����
	POINT ptClientWorld;
	ptClientWorld.x = (getWinSize.right - windowX) / 2;
	ptClientWorld.y = (getWinSize.bottom - windowY) / 2;

	/*
	HWND hWnd = CreateWindowW(
	szWindowClass			// ������ Ŭ���� ��
	, szTitle				// ĸ�� ǥ�� ���ڿ�
	, WS_OVERLAPPEDWINDOW	// ������ ��Ÿ��
	, CW_USEDEFAULT			// �θ� ������ ��� ������ ������ǥ : x
	, 0						// �θ� ������ ��� ������ ������ǥ : y
	, CW_USEDEFAULT			// ������ ������ : width
	, 0						// ������ ������ : height
	, nullptr				// �θ� ������.
	, nullptr				// �޴� �ڵ�
	, hInstance				// �ν��Ͻ� �ڵ�
	, nullptr);				// �߰� �Ķ��Ÿ : NULL
	*/

	hWnd = CreateWindowW(
		szWindowClass			// ������ Ŭ���� ��
		, szTitle				// ĸ�� ǥ�� ���ڿ�
		, dwStyle				// ������ ��Ÿ��
		, ptClientWorld.x		// �θ� ������ ��� ������ ������ǥ : x
		, ptClientWorld.y		// �θ� ������ ��� ������ ������ǥ : y
		, windowX		// ������ ������ : width
		, windowY		// ������ ������ : height
		, nullptr				// �θ� ������.
		, nullptr				// �޴� �ڵ�
		, hInstance				// �ν��Ͻ� �ڵ�
		, nullptr);				// �߰� �Ķ��Ÿ : NULL

								// CreateWindow �Լ���? 
								// ���� ������ �����찡 �ִµ�, �̴�� ����ڴ� ��� ���̴�. 
								// ��������Ŭ������ ������ �İ�, �� �Լ��� ù ���ڷ� ������ ã�´�.
								// 2��° ���ڴ� ĸ������, Ÿ��Ʋ �̸��̴�.
								// 3��° �÷��״� msdn ����
								// ȭ���� �»�� x
								// ȭ���� �»�� y
								// ���� ����� â�� ���� ũ��
								// ���� ����� â�� ���� ũ��
								// �� �ڷδ� null null hInstance(���μ��� �ּ�) null

								// ���� ���н� ���α׷� ����   
	if (!hWnd) return FALSE;
	if (!myFramework.OnCreate(hInstance, hWnd, rc)) return FALSE;
	// ������ ǥ��
	ShowWindow(hWnd, nCmdShow); // ������ â�� �����ش�. �̰� ������ ������ â�� �� ���̱� ������ Ȱ��ȭ�� �ȵǹǷ� �ƹ��͵� ���Ѵ�. ���� ������!
								// Ȯ�� : WnbdProc�� default msg handler�� DefWindowProc
								// �Լ��� ��ȯ�ϴ°�?
	ShowCursor(FALSE);
	UpdateWindow(hWnd);

	// ���� ��ȯ
	return TRUE;
}