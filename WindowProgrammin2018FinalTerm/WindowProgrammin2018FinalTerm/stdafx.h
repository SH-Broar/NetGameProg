// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>
#include <atlimage.h>

#pragma comment(lib, "ws2_32.lib")

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// C++ 런타임 헤더 파일입니다.
#include <iostream>
#include <chrono>
#include <string>

// 타이머를 쓰기 위함
#pragma comment(lib, "winmm.lib")
#include <Mmsystem.h>

#define TITLE_MX_LENGTH 64

#define TITLESTRING TEXT("Run away with the Coin")	// 타이틀 이름이 들어갈 문자열이므로 내용을 원하는 대로 바꿔주자.

// 캡션 FPS 출력 여부
// 항상 캡션에 FPS를 출력	(0: 비활성 | 1: 활성)
#define USE_CAPTIONFPS_ALWAYS	1

#if USE_CAPTIONFPS_ALWAYS
#define SHOW_CAPTIONFPS
#elif _DEBUG	// debug에서는 항상 실행
#define SHOW_CAPTIONFPS
#endif


#if defined(SHOW_CAPTIONFPS)
#define MAX_UPDATE_FPS 1.0 / 3.0
#endif

// 최대 FPS
#if _DEBUG
#define MAX_FPS 0.0 //60프레임 주고싶으면 1/60
#else
// #define MAX_FPS 1.0 / 60.0
#define MAX_FPS 0.0
#endif

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
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