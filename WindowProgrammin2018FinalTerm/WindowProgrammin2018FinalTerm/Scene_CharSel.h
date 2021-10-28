#pragma once
#include "Scene.h"

class Scene_Charsel : public CScene
{
public:
	Scene_Charsel();
	~Scene_Charsel();
	Scene_Charsel(SceneTag tag, CFramework * pFramework);

	void OnDestroy();
	bool OnCreate() override;
	void BuildObjects() override;
	void Update(float fTimeElapsed) override;	// m_pCurrScene->Update();
	void Render(HDC hdc) override;				// FrameAdvanced
	
												// m_pCurrScene->Render(m_hdc);
	void KeyState();
	RECT CalcImage(RECT input);


private:
	int windowX = ::GetSystemMetrics(SM_CXSCREEN);  //����� x����
	int windowY = ::GetSystemMetrics(SM_CYSCREEN);  //����� y����


	//����̾�
	CImage I_BG;
	//���ʿ� ǥ�õ� ĳ���� �Դϴ�
	CImage I_charL[4];
	//�����ʿ� ǥ�õ� ĳ���� �Դϴ�.
	CImage I_charR[4];

	//���� �Ϸ�Ǿ����� ǥ���� �̹����Դϴ�.
	CImage I_Ready;

	CImage I_list;

	CImage I_Select_L, I_Select_R;

	CImage num[10];

	//���õ� ĳ�����Դϴ�.
	int choice1,choice2;
	//���� �Ǿ����� üũ�մϴ�.
	bool ready1 = false, ready2 = false;

	int timer;
	int count;

	bool Finish = false;

	//�ڵ����� �Ѿ�� ���� �����ϱ� ���� ����
	bool P2_L, P2_R;
	bool P1_L, P1_R;
	WCHAR LoadText[100];

	//���� UI ��ġ���Դϴ�. 1920 1080 �������� �ۼ��Ͽ����ϴ�.
	RECT R_NUM1, R_NUM2;
	RECT R_CHAR_L[4], R_CHAR_R[4];
	RECT R_SELECT[4];
	RECT R_LIST;
	RECT R_READY_L, R_READY_R;



};
