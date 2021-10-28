// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>
#include <atlimage.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// C++ ��Ÿ�� ��� �����Դϴ�.
#include <iostream>
#include <chrono>
#include <string>

// Ÿ�̸Ӹ� ���� ����
#pragma comment(lib, "winmm.lib")
#include <Mmsystem.h>

#define TITLE_MX_LENGTH 64

#define TITLESTRING TEXT("Run away with the Coin")	// Ÿ��Ʋ �̸��� �� ���ڿ��̹Ƿ� ������ ���ϴ� ��� �ٲ�����.

// ĸ�� FPS ��� ����
// �׻� ĸ�ǿ� FPS�� ���	(0: ��Ȱ�� | 1: Ȱ��)
#define USE_CAPTIONFPS_ALWAYS	1

#if USE_CAPTIONFPS_ALWAYS
#define SHOW_CAPTIONFPS
#elif _DEBUG	// debug������ �׻� ����
#define SHOW_CAPTIONFPS
#endif


#if defined(SHOW_CAPTIONFPS)
#define MAX_UPDATE_FPS 1.0 / 3.0
#endif

// �ִ� FPS
#if _DEBUG
#define MAX_FPS 0.0 //60������ �ְ������ 1/60
#else
// #define MAX_FPS 1.0 / 60.0
#define MAX_FPS 0.0
#endif

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
template <class T>
class SmartPointer
{
private:
	T* p;
public:
	SmartPointer() {}
	SmartPointer(T* other)
	{
		p = other;
	}
	~SmartPointer()
	{
		delete p;
	}

	T& operator* ()
	{
		return this->(*p);
	}
};
//SmartPointer<char> ptr = new char('a');
//*ptr;

#define DIR_FORWARD		0x01
#define DIR_BACKWARD	0x02
#define DIR_LEFT		0x04
#define DIR_RIGHT		0x08
#define DIR_UP			0x10
#define DIR_DOWN		0x20

#ifdef _DEBUG
#ifdef UNICODE

#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#endif

#endif