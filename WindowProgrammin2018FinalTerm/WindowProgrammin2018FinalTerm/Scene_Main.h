#pragma once
#include "Scene.h"

class CMainScene : public CScene
{
public:
	CMainScene();
	~CMainScene();
	CMainScene(SceneTag tag, CFramework * pFramework);
	void OnDestroy();
	bool OnCreate() override;
	void BuildObjects() override;
	void Render(HDC hdc) override;				// FrameAdvanced
	void Update(float fTimeElapsed) override;	// m_pCurrScene->Update();
												// m_pCurrScene->Render(m_hdc);
	void KeyState();

private:
	int windowX = ::GetSystemMetrics(SM_CXSCREEN);  //모니터 x길이
	int windowY = ::GetSystemMetrics(SM_CYSCREEN);  //모니터 y길이


	CImage MainTitle;
	CImage Manual[2];

	bool CheckKey;
	
	int SceneNum;

	bool finish;


};