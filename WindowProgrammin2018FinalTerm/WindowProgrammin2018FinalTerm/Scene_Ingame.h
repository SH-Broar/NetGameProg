#pragma once
#include "Scene.h"

class CObject_Player;
class OBJECT_Coin;

class CIngameScene : public CScene
{
public:
	CIngameScene();
	~CIngameScene();
	CIngameScene(SceneTag tag, CFramework * pFramework);

	bool OnCreate() override;
	void BuildObjects() override;
	void Render(HDC hdc) override;				// FrameAdvanced
	void Update(float fTimeElapsed) override;	// m_pCurrScene->Update();
												// m_pCurrScene->Render(m_hdc);

	void KeyState();
	void CharacterState();
	void Nevigator();
	void AngleRender(HDC hdc);
private:
	CObject** ppObjects = NULL;
	OBJECT_Coin* CoinObject = NULL;
	int nObjects;

	CImage C_Tile[3];		//타일 이미지
	CImage C_Angle[3];
	CImage C_Numbers[10];
	CImage C_IngameLine;
	CImage WinC;
	CImage LoseC;

	HANDLE hFile;


	int windowX = ::GetSystemMetrics(SM_CXSCREEN);  //모니터 x길이
	int windowY = ::GetSystemMetrics(SM_CYSCREEN);  //모니터 y길이

	BOOL keydownList[14] = { 0 };	// 0 1 2 3 p2 이동 4 5 6 p2 스킬 공격 대시 7 8 9 10 p1 이동 11 12 13 p1 스킬 공격 대시
	BOOL keydownList_N[7] = { 0 }; // 0 1 2 3 이동, 4 5 6 스킬 공격 대시

	BOOL keydown;					// 어떤 키라도 눌렸는지 확인
	//BOOL isp1LockDown = FALSE;		// 공격 등 이동할 수 없는 상태
	//BOOL isp2LockDown = FALSE;		// 공격 등 이동할 수 없는 상태
	//BOOL isp3LockDown = FALSE;		// 공격 등 이동할 수 없는 상태
	BOOL ismyPLockDown = FALSE;
	BOOL coinLockDown = FALSE;		// 애니메이션용
	BOOL isGameEnd    = FALSE;

	RECT tmp;

	int myPlayerNum = 1;

	bool pkey = false;	//이동키
	//bool p1key = false;				// p1 이동키라면
	//bool p2key = false;				// p2 이동키라면
	//bool p3key = false;
	
	float distanceX;
	float distanceY;
	float TotalDistance;
	float rad;
	float Angle;
	float anX;
	float anY;

	int SkillCoolTime;
	int Tileindex[100][100] = { 0 };
	int RemainTime;
	int TimeTick;
	int TimerImage[2];
};