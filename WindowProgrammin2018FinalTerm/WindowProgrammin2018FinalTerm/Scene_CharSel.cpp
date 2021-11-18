#include "stdafx.h"
#include "Scene_CharSel.h"
#include "Framework.h"

Scene_Charsel::Scene_Charsel()
{


}

Scene_Charsel::~Scene_Charsel()
{
}

Scene_Charsel::Scene_Charsel(SceneTag tag, CFramework * pFramework) : CScene(tag, pFramework) //�����ӿ�ũ ������ Ȱ��ȭ
{

}

void Scene_Charsel::OnDestroy()
{
	I_BG.Destroy();
	I_charL[0].Destroy();
	I_charL[1].Destroy();
	I_charL[2].Destroy();
	I_charL[3].Destroy();

	I_charR[0].Destroy();
	I_charR[1].Destroy();
	I_charR[2].Destroy();
	I_charR[3].Destroy();

	I_Ready.Destroy();

	I_list.Destroy();

	I_Select_L.Destroy();
	I_Select_R.Destroy();

	for (int i = 0; i < 10; i++) {
		num[i].Destroy();
	}
}

//�����ɶ� ���� �Ϸ��� �δ�
bool Scene_Charsel::OnCreate()
{
	//�̹����� �ε��մϴ�.
	I_BG.Load(L"Graphic\\UI\\CharSel\\BG.png");
	I_charL[0].Load(L"Graphic\\UI\\CharSel\\1L.png");
	I_charL[1].Load(L"Graphic\\UI\\CharSel\\2L.png");
	I_charL[2].Load(L"Graphic\\UI\\CharSel\\3L.png");
	I_charL[3].Load(L"Graphic\\UI\\CharSel\\RL.png");

	I_charR[0].Load(L"Graphic\\UI\\CharSel\\1R.png");
	I_charR[1].Load(L"Graphic\\UI\\CharSel\\2R.png");
	I_charR[2].Load(L"Graphic\\UI\\CharSel\\3R.png");
	I_charR[3].Load(L"Graphic\\UI\\CharSel\\RR.png");

	I_Ready.Load(L"Graphic\\UI\\CharSel\\READY.png");

	I_list.Load(L"Graphic\\UI\\CharSel\\List.png");

	I_Select_L.Load(L"Graphic\\UI\\CharSel\\L.png");
	I_Select_R.Load(L"Graphic\\UI\\CharSel\\R.png");
	I_Select_M.Load(L"Graphic\\UI\\CharSel\\M.png");

	for (int i = 0; i < 10; i++) {
		wsprintf(LoadText, L"Graphic\\UI\\CharSel\\NUM\\%d.png", i);
		num[i].Load(LoadText);
	}

	pn = m_pFramework->NetGram.getPN();

	//Ÿ�̸� �ʱ�ȭ
	timer = 60;
	count = 60;

	P1_L = false;
	P1_R = false;
	P2_L = false;
	P2_R = false;
	P3_L = false;
	P3_R = false;

	choice1 = 1;
	choice2 = 3;
	choice3 = 2;

	Finish = false;

	//�̹��� ũ��� �����Ͽ� �ۼ��մϴ�.
	//�̹��� ũ�Ⱑ ����� �� ���ۼ��� �ʿ��մϴ�.
	//��� �̹��� ũ��� CalcImage()�� ���� �����ǵ�
	//bottom�� Right �� ũ�⸦ ������ �˴ϴ�.
	{
		//10���� �̹���
		R_NUM1.top = 50;
		R_NUM1.left = 760;
		R_NUM1.bottom = 200;
		R_NUM1.right = 200;

		//1���� �̹���
		R_NUM2.top = 50;
		R_NUM2.left = 960;
		R_NUM2.bottom = 200;
		R_NUM2.right = 200;

		R_LIST.top = 500;
		R_LIST.left = 670;
		R_LIST.right = 579;
		R_LIST.bottom = 544;

		//ĳ���� L
		{
			R_CHAR_L[0].top = 0;
			R_CHAR_L[0].left = 0;
			R_CHAR_L[0].bottom = 1075;
			R_CHAR_L[0].right = 928;

			R_CHAR_L[1].top = 25;
			R_CHAR_L[1].left = 100;
			R_CHAR_L[1].bottom = 1049;
			R_CHAR_L[1].right = 596;

			R_CHAR_L[2].top = 2;
			R_CHAR_L[2].left = 0;
			R_CHAR_L[2].bottom = 1078;
			R_CHAR_L[2].right = 669;

			R_CHAR_L[3].top = 5;
			R_CHAR_L[3].left = 0;
			R_CHAR_L[3].bottom = 1075;
			R_CHAR_L[3].right = 603;

			R_READY_L.top = 800;
			R_READY_L.left = 50;
			R_READY_L.bottom = 205;
			R_READY_L.right = 633;
		}

		//ĳ���� R
		{
			R_CHAR_R[0].top = 5;
			R_CHAR_R[0].left = 1118;
			R_CHAR_R[0].bottom = 1075;
			R_CHAR_R[0].right = 802;

			R_CHAR_R[1].top = 10;
			R_CHAR_R[1].left = 1224;
			R_CHAR_R[1].bottom = 1070;
			R_CHAR_R[1].right = 596;

			R_CHAR_R[2].top = 0;
			R_CHAR_R[2].left = 1251;
			R_CHAR_R[2].bottom = 1080;
			R_CHAR_R[2].right = 669;

			R_CHAR_R[3].top = 0;
			R_CHAR_R[3].left = 1329;
			R_CHAR_R[3].bottom = 1080;
			R_CHAR_R[3].right = 591;

			R_READY_R.top = 800;
			R_READY_R.left = 1250;
			R_READY_R.bottom = 205;
			R_READY_R.right = 633;
		}

		//ĳ���� M
		{
			R_CHAR_M[0].top = 0;
			R_CHAR_M[0].left = 500;
			R_CHAR_M[0].bottom = 1075;
			R_CHAR_M[0].right = 928;
						
			R_CHAR_M[1].top = 25;
			R_CHAR_M[1].left = 600;
			R_CHAR_M[1].bottom = 1049;
			R_CHAR_M[1].right = 596;
						
			R_CHAR_M[2].top = 2;
			R_CHAR_M[2].left = 500;
			R_CHAR_M[2].bottom = 1078;
			R_CHAR_M[2].right = 669;
						
			R_CHAR_M[3].top = 5;
			R_CHAR_M[3].left = 500;
			R_CHAR_M[3].bottom = 1075;
			R_CHAR_M[3].right = 603;

			R_READY_M.top = 200;
			R_READY_M.left = 650;
			R_READY_M.bottom = 205;
			R_READY_M.right = 633;
		}

		{
			R_SELECT[0].top = 471;
			R_SELECT[0].left = 670;
			R_SELECT[0].right = 139;
			R_SELECT[0].bottom = 596;

			R_SELECT[1].top = 471;
			R_SELECT[1].left = 820;
			R_SELECT[1].right = 139;
			R_SELECT[1].bottom = 596;

			R_SELECT[2].top = 471;
			R_SELECT[2].left = 970;
			R_SELECT[2].right = 139;
			R_SELECT[2].bottom = 596;

			R_SELECT[3].top = 471;
			R_SELECT[3].left = 1120;
			R_SELECT[3].right = 139;
			R_SELECT[3].bottom = 596;
		}
	}





	return false;
}

void Scene_Charsel::BuildObjects()
{

}


//Ű ���¸� �Է¹���.
void Scene_Charsel::KeyState()
{
	switch (pn)
	{
	case 1:
		//P1 ����
		if (GetAsyncKeyState(0x41) & 0x8000) {
			ready1 = true;
		}
		if (!ready1) {
			// f
			if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
				if (!P1_L) {
					switch (choice1) {
					case 1:
						choice1 = 4;
						break;
					case 2:
						choice1 = 1;
						break;
					case 3:
						choice1 = 2;
						break;
					case 4:
						choice1 = 3;
						break;
					}
				}
				P1_L = true;
			}
			else {
				P1_L = false;
			}
			// h
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
				if (!P1_R) {
					switch (choice1) {
					case 1:
						choice1 = 2;
						break;
					case 2:
						choice1 = 3;
						break;
					case 3:
						choice1 = 4;
						break;
					case 4:
						choice1 = 1;
						break;
					}
				}
				P1_R = true;
			}
			else {
				P1_R = false;
			}
		}
		break;
	case 2:
		//P1 ����
		if (GetAsyncKeyState(0x41) & 0x8000) {
			ready2 = true;
		}
		if (!ready2) {
			// f
			if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
				if (!P2_L) {
					switch (choice2) {
					case 1:
						choice2 = 4;
						break;
					case 2:
						choice2 = 1;
						break;
					case 3:
						choice2 = 2;
						break;
					case 4:
						choice2 = 3;
						break;
					}
				}
				P2_L = true;
			}
			else {
				P2_L = false;
			}
			// h
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
				if (!P2_R) {
					switch (choice2) {
					case 1:
						choice2 = 2;
						break;
					case 2:
						choice2 = 3;
						break;
					case 3:
						choice2 = 4;
						break;
					case 4:
						choice2 = 1;
						break;
					}
				}
				P2_R = true;
			}
			else {
				P2_R = false;
			}
		}
		break;
	case 3:
		//P1 ����
		if (GetAsyncKeyState(0x41) & 0x8000) {
			ready3 = true;
		}
		if (!ready3) {
			// f
			if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
				if (!P3_L) {
					switch (choice3) {
					case 1:
						choice3 = 4;
						break;
					case 2:
						choice3 = 1;
						break;
					case 3:
						choice3 = 2;
						break;
					case 4:
						choice3 = 3;
						break;
					}
				}
				P3_L = true;
			}
			else {
				P3_L = false;
			}
			// h
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
				if (!P3_R) {
					switch (choice3) {
					case 1:
						choice3 = 2;
						break;
					case 2:
						choice3 = 3;
						break;
					case 3:
						choice3 = 4;
						break;
					case 4:
						choice3 = 1;
						break;
					}
				}
				P3_R = true;
			}
			else {
				P3_R = false;
			}
		}
		break;
	}


}

void Scene_Charsel::NetCheck()
{
	 m_pFramework->NetGram.recvData(m_pFramework->STC);
	
	 choice1 = m_pFramework->STC.PlayerData[0].state;
	 choice2 = m_pFramework->STC.PlayerData[1].state;
	 choice3 = m_pFramework->STC.PlayerData[2].state;
	 printf("%d %d %d\n", choice1, choice2, choice3);

	 ready1 = m_pFramework->STC.PlayerData[0].AttackedPlayerNum[0];
	 ready2 = m_pFramework->STC.PlayerData[1].AttackedPlayerNum[0];
	 ready3 = m_pFramework->STC.PlayerData[2].AttackedPlayerNum[0];

	 timer = m_pFramework->STC.Time;
}

void Scene_Charsel::NetSend()
{
	//printf("NetSend1 : %d\n", pn);
	switch (pn)
	{
	case 1:
		//printf("NetSend2 : %d\n", choice1);
		m_pFramework->CTS.set(1,choice1,ready1);
		break;
	case 2:
		m_pFramework->CTS.set(2,choice2, ready2);
		break;
	case 3:
		m_pFramework->CTS.set(3,choice3, ready3);
		break;
	default:
		m_pFramework->CTS.set(1, choice1, ready1);
		break;
	}

	printf("NetSend : %d %d %d %d %d\n", 
		m_pFramework->CTS.PlayerNum, 
		m_pFramework->CTS.state,
		m_pFramework->CTS.x,
		m_pFramework->CTS.y,
		m_pFramework->CTS.AttackedPlayerNum[0]);
	m_pFramework->NetGram.sendData(m_pFramework->CTS);
}

void voidBuffer(SOCKET s)
{
	u_long tmpl, i;
	char tmpc; ioctlsocket(s, FIONREAD, &tmpl);
	for (i = 0; i < tmpl; i++)
		recv(s, &tmpc, sizeof(char), 0);
}

// 1/60���� ������Ʈ��
void Scene_Charsel::Update(float fTimeElapsed)
{
	NetCheck();
	KeyState();

	//voidBuffer(m_pFramework->NetGram.getSock(true));
	//������� �ʾƾ��� ��� �����Ѵ�.
	if (!Finish) {
		////�� ������ üũ
		//if (count == 0) {
		//	count = 60;
		//	timer--;
		//}
		//else {
		//	count--;
		//}

		//Ÿ�̸Ӱ� 0�̵Ǹ� ������ ���� ��Ŵ
		if (timer == 0) {
			ready1 = true;
			ready2 = true;
			ready3 = true;
		}

		//�´� ���̸� �ǴϽ�
		if (ready1&&ready2&&ready3) {
			Finish = true;
		}
		if (Finish)
		{
			if (choice1 == 4)
				choice1 = rand() % 3 + 1;
			if (choice2 == 4)
				choice2 = rand() % 3 + 1;
			if (choice3 == 4)
				choice3 = rand() % 3 + 1;


			m_pFramework->ChangeScene(CScene::SceneTag::Ingame);
			m_pFramework->curSceneCreate();
			m_pFramework->BuildPlayer(choice1, choice2, choice3);
			Scene_Charsel::OnDestroy();

		}

		
	}
	NetSend();
}

void Scene_Charsel::Render(HDC hdc)
{

	//�׷��ִ� ����
	//���
	//���� �Ϸ���Ʈ
	//ĳ���� �̴� �Ϸ���Ʈ
	//Ÿ�̸�
	//���� ����
	//�غ�

	I_BG.Draw(hdc, 0, 0, windowX, windowY);

	//�̹��� ũ�Ⱑ �� �ٸ��ϱ� �׳� ����ġ ���̽� ����...
	//���� �̹���


	switch (choice1) {
	case 1:
		I_charL[0].Draw(hdc, CalcImage(R_CHAR_L[0]));
		break;
	case 2:
		I_charL[1].Draw(hdc, CalcImage(R_CHAR_L[1]));
		break;
	case 3:
		I_charL[2].Draw(hdc, CalcImage(R_CHAR_L[2]));
		break;
	case 4: //�����϶���
		I_charL[3].Draw(hdc, CalcImage(R_CHAR_L[3]));
		break;
	}


	//������ �̹���

	switch (choice2)
	{
	case 1:
		I_charR[0].Draw(hdc, CalcImage(R_CHAR_R[0]));
		break;
	case 2:
		I_charR[1].Draw(hdc, CalcImage(R_CHAR_R[1]));
		break;
	case 3:
		I_charR[2].Draw(hdc, CalcImage(R_CHAR_R[2]));
		break;
	case 4:
		I_charR[3].Draw(hdc, CalcImage(R_CHAR_R[3]));
		break;
	}

	switch (choice3)
	{
	case 1:
		I_charL[0].Draw(hdc, CalcImage(R_CHAR_M[0]));
		break;
	case 2:
		I_charL[1].Draw(hdc, CalcImage(R_CHAR_M[1]));
		break;
	case 3:
		I_charL[2].Draw(hdc, CalcImage(R_CHAR_M[2]));
		break;
	case 4:
		I_charL[3].Draw(hdc, CalcImage(R_CHAR_M[3]));
		break;
	}



	I_list.Draw(hdc, CalcImage(R_LIST));

	I_Select_L.Draw(hdc, CalcImage(R_SELECT[choice1 - 1]));
	I_Select_R.Draw(hdc, CalcImage(R_SELECT[choice2 - 1]));
	I_Select_M.Draw(hdc, CalcImage(R_SELECT[choice3 - 1]));

	//���� ���� �ߴٸ�
	if (ready1) {
		I_Ready.Draw(hdc, CalcImage(R_READY_L));
	}
	if (ready2) {
		I_Ready.Draw(hdc, CalcImage(R_READY_R));
	}
	if (ready3) {
		I_Ready.Draw(hdc, CalcImage(R_READY_M));
	}




	num[timer / 10].Draw(hdc, CalcImage(R_NUM1));
	num[timer % 10].Draw(hdc, CalcImage(R_NUM2));


}


//�̹����� ũ�⸦ ����մϴ�.
RECT Scene_Charsel::CalcImage(RECT input) {
	//=======================================
	//        ��� �ȳ�
	// top�� left�� ȭ�� ������ �°� ����Ͽ� ���ϰ�
	// bottom�� right�� ȭ�� ũ�⿡ �°� ����Ͽ� ���Ѵ�.
	//===========================================

	RECT calc;

	float X = 1920.0, Y = 1080.0;

	//1080�϶� �������� �����ϰ�, �������� ũ��� ������ ��ġ�� �����.
	calc.top = input.top*(windowY / Y);
	//1920�϶� �������� �����ϰ�, �������� ũ��� ������ ��ġ�� ���սô�.
	calc.left = input.left*(windowX / X);

	//���ο� ������ � ������ �� ������ ���ϰ� �� ���� ������ ���Ͽ� ���� �����Ͽ� �׸� ũ�⸦ ���Ѵ�.
	//���ΰ� �� ũ�Ƿ� ���� �������� �����.
	if ((windowY / Y) >= (windowX / X)) {
		calc.bottom = calc.top + input.bottom * (windowX / X);
		calc.right = calc.left + input.right * (windowX / X);
	}
	//���ΰ� �� ũ�Ƿ� ���� �������� �����.
	else {
		calc.bottom = calc.top + input.bottom*(windowY / Y);
		calc.right = calc.left + input.right*(windowY / Y);
	}

	//printf("top : %d, bottom : %d, left : %d, right : %d\n", calc.top, calc.bottom, calc.left, calc.right);
	//���� ����� �����Ѵ�.
	return calc;
}

