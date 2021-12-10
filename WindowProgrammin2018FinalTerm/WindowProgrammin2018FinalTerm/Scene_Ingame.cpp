#include "stdafx.h"
#include "Scene_Ingame.h"
#include "OBJECT_Player.h"
#include "OBJECT_Coin.h"
#include "OBJECT_Skill.h"
#include "Framework.h"

#define MEMBERS 3
#define FrameDelayCnt 1

CIngameScene::CIngameScene()
{

}
CIngameScene::~CIngameScene()
{
	if (CoinObject != NULL)
	{
		free(CoinObject);
	}

}
CIngameScene::CIngameScene(SceneTag tag, CFramework* pFramework) : CScene(tag, pFramework) //프레임워크 포인터 활성화
{

}

char Inbuff[20000] = { 0 };
bool CIngameScene::OnCreate()
{
	myPlayerNum = m_pFramework->NetGram.getPN();
#ifdef _DEBUG
	printf("My Player Number : %d", myPlayerNum);
#endif

	m_pFramework->CTS.clear();
	m_pFramework->STC.clear();

	C_Tile[0].Load(L"Graphic\\Tile\\Tile1.jpg");
	C_Tile[1].Load(L"Graphic\\Tile\\Tile2.png");
	C_Tile[2].Load(L"Graphic\\Tile\\Tile3.png");

	C_Angle[0].Load(L"Graphic\\UI\\1.png");
	C_Angle[1].Load(L"Graphic\\UI\\2.png");
	C_Angle[2].Load(L"Graphic\\UI\\3.png");

	C_IngameLine.Load(L"Graphic\\UI\\INGAME UI.png");

	WinC.Load(L"Graphic\\UI\\WIN.png");
	LoseC.Load(L"Graphic\\UI\\LOSE.png");

	WCHAR LoadText[100];
	for (int i = 0; i <= 9; i++)
	{
		wsprintf(LoadText, L"Graphic\\Numbers\\%d.png", i);
		this->C_Numbers[i].Load(LoadText);
	}

	prePlayersDirections[0] = 0;
	prePlayersDirections[1] = 0;
	prePlayersDirections[2] = 0;

	DWORD read_size = 20000;
	DWORD c = 20000;

	hFile = CreateFile(L"map.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
	memset(Inbuff, 0, 99 * sizeof(WCHAR));
	ReadFile(hFile, Inbuff, c, &read_size, NULL); // hFile에서 size 만큼 읽어 InBuff에 저장
	//Inbuff[c - 1] = '\0';
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			if (Inbuff[j + i * 100] >= 48 && Inbuff[j + i * 100] <= 50)
				Tileindex[j][i] = Inbuff[j + i * 100] - 48;
			switch (Tileindex[j][i])
			{
			case 0:
				Tileindex[j][i] = 2;
				break;
			case 1:
				Tileindex[j][i] = 0;
				break;
			case 2:
				Tileindex[j][i] = 1;
				break;
			}
		}
	}

	CloseHandle(hFile);
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			C_Tile[Tileindex[j][i]].BitBlt(*m_pFramework->GetTileDC(), 64 * j, 64 * i, 64, 64, 0, 0, SRCCOPY); //타일의 가로세로크기가 64바이트
		}
	}


	RemainTime = 99;
	TimeTick = 0;
	TimerImage[0] = 9;
	TimerImage[1] = 9;
	SkillCoolTime = 0;
	BuildObjects();

	currentFDC = FrameDelayCnt;

	return true;
}


void CIngameScene::BuildObjects()
{
	nObjects = 0;
	CoinObject = new OBJECT_Coin(rand() % 6000 + 250, rand() % 6000 + 250);
	/*CObject** ppObjects;
	int nObjects; 여기다 값넣기*/

	/*nObjects = 2;
	ppObjects[0] = m_pFramework->GetPlayer(1);
	ppObjects[1] = m_pFramework->GetPlayer(2);*/
}

//

void CIngameScene::KeyState()
{
	if (keydown != TRUE)
	{
		if (ismyPLockDown != TRUE)
		{
			if (GetAsyncKeyState(0x5A) & 0x8000 && SkillCoolTime <= 0) // 스킬
			{
				keydownList_N[4] = TRUE;

				keydown = TRUE;
				ismyPLockDown = TRUE;
			}
			else if (GetAsyncKeyState(0x58) & 0x8000) // 공격
			{
				keydownList_N[5] = TRUE;

				keydown = TRUE;
				ismyPLockDown = TRUE;
			}
			else if (GetAsyncKeyState(0x43) & 0x8000) //대시
			{
				if (m_pFramework->GetPlayer(myPlayerNum)->DashCoolTimer <= 0)
				{
					keydownList_N[6] = TRUE;

					keydown = TRUE;
					ismyPLockDown = TRUE;
				}
			}
			else
			{
				if (GetAsyncKeyState(VK_UP) & 0x8000)
				{
					keydownList_N[1] = TRUE;

					keydown = TRUE;
				}
				if (GetAsyncKeyState(VK_LEFT) & 0x8000)
				{
					keydownList_N[0] = TRUE;

					keydown = TRUE;
				}
				if (GetAsyncKeyState(VK_DOWN) & 0x8000)
				{
					keydownList_N[3] = TRUE;

					keydown = TRUE;
				}
				if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
				{
					keydownList_N[2] = TRUE;

					keydown = TRUE;
				}
			}
		}

		for (int i = 1; i <= MEMBERS; ++i)
		{
			if (i != myPlayerNum)
			{
				int prev = m_pFramework->GetPlayer(i)->CharacterStatus;
				m_pFramework->GetPlayer(i)->x = m_pFramework->STC.PlayerData[i - 1].x;
				m_pFramework->GetPlayer(i)->y = m_pFramework->STC.PlayerData[i - 1].y;
				m_pFramework->GetPlayer(i)->CharacterStatus = m_pFramework->STC.PlayerData[i - 1].state;
			}
		}
	}

		FrameAdvance();
	
}

void CIngameScene::FrameAdvance()
{
	//상대 설정
	for (int i = 1; i <= MEMBERS; ++i)
	{
		if (i != myPlayerNum)
		{
			if (m_pFramework->GetPlayer(i)->CharacterStatus != 12 && m_pFramework->GetPlayer(i)->CharacterStatus != 13)
				m_pFramework->GetPlayer(myPlayerNum)->Old_CharStat = m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus;
			switch (m_pFramework->GetPlayer(i)->CharacterStatus)
			{
			case 0:
			case 1:
				m_pFramework->GetPlayer(i)->isWalk = FALSE;
				m_pFramework->GetPlayer(i)->AttackTimerTick = 0;
				m_pFramework->GetPlayer(i)->AttackImageTick = 0;
				m_pFramework->GetPlayer(i)->WalkingTimerTick = 0;
				break;
			case 2:
			case 3:
			case 4:
			case 5:
				m_pFramework->GetPlayer(i)->isWalk = TRUE;
				if (m_pFramework->GetPlayer(i)->WalkingTimerTick++ > 3)
				{
					m_pFramework->GetPlayer(i)->WalkingTimerTick = 0;
					m_pFramework->GetPlayer(i)->WalkingImageTick++;
				}
				//prePlayersDirections[i - 1] = m_pFramework->GetPlayer(i)->CharacterStatus;
				break;
			case 6:
			case 7:
				m_pFramework->GetPlayer(i)->isAttack = TRUE;
				if (m_pFramework->GetPlayer(i)->AttackTimerTick++ > 3)
				{
					m_pFramework->GetPlayer(i)->AttackTimerTick = 0;
					if (m_pFramework->GetPlayer(i)->AttackImageTick++ > 2)
					{
						m_pFramework->GetPlayer(i)->isAttack = FALSE;
						m_pFramework->GetPlayer(i)->AttackTimerTick = 0;
						m_pFramework->GetPlayer(i)->AttackImageTick = 0;
						//printf("%d", m_pFramework->GetPlayer(2)->CharacterStatus);
						if (m_pFramework->GetPlayer(i)->CharacterStatus == 6)
						{
							m_pFramework->GetPlayer(i)->CharacterStatus = 0;
						}
						else
						{
							m_pFramework->GetPlayer(i)->CharacterStatus = 1;
						}
					}
				}
				break;
			case 8:
			case 9:
				m_pFramework->GetPlayer(i)->isWalk = FALSE;
				if (m_pFramework->GetPlayer(i)->SkillAttackedTimer-- > 0 &&
					m_pFramework->GetPlayer(i)->AttackTimerTick > 10)
				{
					m_pFramework->GetPlayer(i)->AttackTimerTick--;
				}

				if (m_pFramework->GetPlayer(i)->AttackTimerTick++ > 20)
				{
					m_pFramework->GetPlayer(i)->AttackTimerTick = 0;
					m_pFramework->GetPlayer(i)->isAttacked = FALSE;
					//m_pFramework->GetPlayer(i)->CharacterStatus = m_pFramework->GetPlayer(i)->Old_CharStat;
					m_pFramework->GetPlayer(i)->SkillAttackedTimer = 0;
				}
				break;
			case 10:
			case 11:
				m_pFramework->GetPlayer(i)->isWalk = FALSE;
				if (m_pFramework->GetPlayer(i)->DashTimerTick++ > 10)
				{
					m_pFramework->GetPlayer(i)->DashTimerTick = 0;
					m_pFramework->GetPlayer(i)->isDash = FALSE;
				}
				break;
			case 12:
			case 13:

				//if (m_pFramework->GetPlayer(i)->isSkill == FALSE)
				{
					m_pFramework->GetPlayer(i)->SkillCast(m_pFramework->GetPlayer(i)->x, m_pFramework->GetPlayer(i)->y, prePlayersDirections[i-1]);
					m_pFramework->GetPlayer(i)->isWalk = FALSE;
					m_pFramework->GetPlayer(i)->isSkill = TRUE;
				}
				

				if (m_pFramework->GetPlayer(i)->AttackTimerTick++ > 3)
				{
					m_pFramework->GetPlayer(i)->AttackTimerTick = 0;
					if (m_pFramework->GetPlayer(i)->AttackImageTick++ > 2)
					{
						m_pFramework->GetPlayer(i)->AttackTimerTick = 0;
						m_pFramework->GetPlayer(i)->AttackImageTick = 0;
						//printf("%d", m_pFramework->GetPlayer(2)->CharacterStatus);
					}
				}
				break;
			default:
				m_pFramework->GetPlayer(i)->isWalk = FALSE;
				break;
			}
		}
	}

	//if (keydown)
	{
		if (m_pFramework->GetPlayer(myPlayerNum)->isAttack)
		{
			if (m_pFramework->GetPlayer(myPlayerNum)->AttackTimerTick++ > 3)
			{
				m_pFramework->GetPlayer(myPlayerNum)->AttackTimerTick = 0;
				if (m_pFramework->GetPlayer(myPlayerNum)->AttackImageTick++ > 2)
				{
					m_pFramework->GetPlayer(myPlayerNum)->isAttack = FALSE;
					m_pFramework->GetPlayer(myPlayerNum)->AttackTimerTick = 0;
					m_pFramework->GetPlayer(myPlayerNum)->AttackImageTick = 0;
					//printf("%d", m_pFramework->GetPlayer(2)->CharacterStatus);
					if (m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus == 6)
					{
						m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus = 0;
					}
					else
					{
						m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus = 1;
					}
					ismyPLockDown = FALSE;
					pkey = TRUE;
				}
			}
		}
		else if (m_pFramework->GetPlayer(myPlayerNum)->isSkill)
		{
			if (m_pFramework->GetPlayer(myPlayerNum)->AttackTimerTick++ > 3)
			{
				m_pFramework->GetPlayer(myPlayerNum)->AttackTimerTick = 0;
				if (m_pFramework->GetPlayer(myPlayerNum)->AttackImageTick++ > 2)
				{
					m_pFramework->GetPlayer(myPlayerNum)->AttackTimerTick = 0;
					m_pFramework->GetPlayer(myPlayerNum)->AttackImageTick = 0;
					//printf("%d", m_pFramework->GetPlayer(2)->CharacterStatus);
				}
			}
		}
		else if (m_pFramework->GetPlayer(myPlayerNum)->isSkillEnd)
		{
			if (m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus == 12)
				m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus = 0;
			else
				m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus = 1;
			m_pFramework->GetPlayer(myPlayerNum)->AttackTimerTick = 0;
			m_pFramework->GetPlayer(myPlayerNum)->AttackImageTick = 0;
			m_pFramework->GetPlayer(myPlayerNum)->isSkillEnd = FALSE;
			ismyPLockDown = FALSE;
		}
		else if (m_pFramework->GetPlayer(myPlayerNum)->isDash)
		{
			if (m_pFramework->GetPlayer(myPlayerNum)->DashTimerTick++ > 10)
			{
				m_pFramework->GetPlayer(myPlayerNum)->DashTimerTick = 0;
				m_pFramework->GetPlayer(myPlayerNum)->isDash = FALSE;
				if (m_pFramework->GetPlayer(myPlayerNum)->Old_CharStat % 2)
					m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus = 1;
				else
					m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus = 2;

				ismyPLockDown = FALSE;
				pkey = TRUE;
			}
			printf("Dash OldStat : %d\n", m_pFramework->GetPlayer(myPlayerNum)->Old_CharStat);
			switch (m_pFramework->GetPlayer(myPlayerNum)->Old_CharStat)
			{
			case 2:
			case 0:
			case 10:
				if (m_pFramework->GetPlayer(myPlayerNum)->x > 50)
					m_pFramework->GetPlayer(myPlayerNum)->x -= 25;
				break;
			case 5:
				if (m_pFramework->GetPlayer(myPlayerNum)->y < 6350)
					m_pFramework->GetPlayer(myPlayerNum)->y += 25;

				break; // 앞 볼 때
			case 3:
				if (m_pFramework->GetPlayer(myPlayerNum)->y > 50)
					m_pFramework->GetPlayer(myPlayerNum)->y -= 25;

				break;
			case 4:
			case 1:
			case 11:
				if (m_pFramework->GetPlayer(myPlayerNum)->x < 6350)
					m_pFramework->GetPlayer(myPlayerNum)->x += 25;
				break; // 뒤 볼 때
			}
		}
		else if (m_pFramework->GetPlayer(myPlayerNum)->isAttacked)
		{
			if (m_pFramework->GetPlayer(myPlayerNum)->SkillAttackedTimer-- > 0 &&
				m_pFramework->GetPlayer(myPlayerNum)->AttackTimerTick > 10)
			{
				m_pFramework->GetPlayer(myPlayerNum)->AttackTimerTick--;
			}

			if (m_pFramework->GetPlayer(myPlayerNum)->AttackTimerTick++ > 20)
			{
				m_pFramework->GetPlayer(myPlayerNum)->AttackTimerTick = 0;
				m_pFramework->GetPlayer(myPlayerNum)->isAttacked = FALSE;
				m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus = m_pFramework->GetPlayer(myPlayerNum)->Old_CharStat;
				ismyPLockDown = FALSE;
				pkey = TRUE;
				m_pFramework->GetPlayer(myPlayerNum)->SkillAttackedTimer = 0;
			}

			if (m_pFramework->GetPlayer(myPlayerNum)->AttackTimerTick < 10)
			{
				//원래 때린사람 기준으로 해야되긴 하는데
				switch (m_pFramework->GetPlayer(myPlayerNum)->Old_CharStat)
				{
				case 2:
				case 0:
					if (m_pFramework->GetPlayer(myPlayerNum)->x < 6350)
						m_pFramework->GetPlayer(myPlayerNum)->x += 15;
					if (coinLockDown)
						CoinObject->Setx(CoinObject->x() - 5);
					break;
				case 5:
					if (m_pFramework->GetPlayer(myPlayerNum)->y > 50)
						m_pFramework->GetPlayer(myPlayerNum)->y -= 15;
					if (coinLockDown)
						CoinObject->Setx(CoinObject->y() + 5);
					break; // 앞 볼 때
				case 3:

					if (m_pFramework->GetPlayer(myPlayerNum)->y < 6350)
						m_pFramework->GetPlayer(myPlayerNum)->y += 15;
					if (coinLockDown)
						CoinObject->Setx(CoinObject->y() - 5);
					break;
				case 4:
				case 1:

					if (m_pFramework->GetPlayer(myPlayerNum)->x > 50)
						m_pFramework->GetPlayer(myPlayerNum)->x -= 15;
					if (coinLockDown)
						CoinObject->Setx(CoinObject->x() + 5);
					break; // 뒤 볼 때
				}
			}
			else
			{
				coinLockDown = FALSE;
			}
		}
	}
}

//
void CIngameScene::CharacterState()
{
	if (keydown)
	{
		// 0 1 2 3 p2 이동 4 5 6 p2 스킬 공격 대시
		if (keydownList_N[1])
		{
			if (Tileindex[m_pFramework->GetPlayer(myPlayerNum)->x / 64][(m_pFramework->GetPlayer(myPlayerNum)->y + 60) / 64] == 1)
			{
				if (m_pFramework->GetPlayer(myPlayerNum)->y > 50)
					m_pFramework->GetPlayer(myPlayerNum)->y -= (15 - m_pFramework->GetPlayer(myPlayerNum)->iHaveCoin * 4);
				m_pFramework->GetPlayer(myPlayerNum)->WalkingTimerTick++;
			}
			else if (Tileindex[m_pFramework->GetPlayer(myPlayerNum)->x / 64][(m_pFramework->GetPlayer(myPlayerNum)->y + 60 - 20) / 64] == 2)
			{

			}
			else
			{
				if (m_pFramework->GetPlayer(myPlayerNum)->y > 50)
					m_pFramework->GetPlayer(myPlayerNum)->y -= (10 - m_pFramework->GetPlayer(myPlayerNum)->iHaveCoin * 2);
			}
			pkey = true;
			m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus = 3;

		}
		if (keydownList_N[0])
		{
			if (Tileindex[m_pFramework->GetPlayer(myPlayerNum)->x / 64][(m_pFramework->GetPlayer(myPlayerNum)->y + 60) / 64] == 1)
			{
				if (m_pFramework->GetPlayer(myPlayerNum)->x > 50)
					m_pFramework->GetPlayer(myPlayerNum)->x -= (15 - m_pFramework->GetPlayer(myPlayerNum)->iHaveCoin * 4);
				m_pFramework->GetPlayer(myPlayerNum)->WalkingTimerTick++;
			}
			else if (Tileindex[((m_pFramework->GetPlayer(myPlayerNum)->x) - 30) / 64][(m_pFramework->GetPlayer(myPlayerNum)->y + 60 - 10) / 64] == 2)
			{

			}
			else
			{
				if (m_pFramework->GetPlayer(myPlayerNum)->x > 50)
					m_pFramework->GetPlayer(myPlayerNum)->x -= (10 - m_pFramework->GetPlayer(myPlayerNum)->iHaveCoin * 2);
			}
			pkey = true;
			m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus = 2;

		}
		if (keydownList_N[3])
		{
			if (Tileindex[m_pFramework->GetPlayer(myPlayerNum)->x / 64][(m_pFramework->GetPlayer(myPlayerNum)->y + 60) / 64] == 1)
			{
				if (m_pFramework->GetPlayer(myPlayerNum)->y < 6350)
					m_pFramework->GetPlayer(myPlayerNum)->y += (15 - m_pFramework->GetPlayer(myPlayerNum)->iHaveCoin * 4);
				m_pFramework->GetPlayer(myPlayerNum)->WalkingTimerTick++;
			}
			else if (Tileindex[m_pFramework->GetPlayer(myPlayerNum)->x / 64][(m_pFramework->GetPlayer(myPlayerNum)->y + 60 + 20) / 64] == 2)
			{

			}
			else
			{
				if (m_pFramework->GetPlayer(myPlayerNum)->y < 6350)
					m_pFramework->GetPlayer(myPlayerNum)->y += (10 - m_pFramework->GetPlayer(myPlayerNum)->iHaveCoin * 2);
			}
			pkey = true;
			m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus = 5;

		}
		if (keydownList_N[2])
		{
			if (Tileindex[m_pFramework->GetPlayer(myPlayerNum)->x / 64][(m_pFramework->GetPlayer(myPlayerNum)->y + 60) / 64] == 1)
			{
				if (m_pFramework->GetPlayer(myPlayerNum)->x < 6350)
					m_pFramework->GetPlayer(myPlayerNum)->x += (15 - m_pFramework->GetPlayer(myPlayerNum)->iHaveCoin * 4);
				m_pFramework->GetPlayer(myPlayerNum)->WalkingTimerTick++;
			}
			else if (Tileindex[(m_pFramework->GetPlayer(myPlayerNum)->x + 30) / 64][(m_pFramework->GetPlayer(myPlayerNum)->y + 60) / 64] == 2)
			{

			}
			else
			{
				if (m_pFramework->GetPlayer(myPlayerNum)->x < 6350)
					m_pFramework->GetPlayer(myPlayerNum)->x += (10 - m_pFramework->GetPlayer(myPlayerNum)->iHaveCoin * 2);
			}
			pkey = true;
			m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus = 4;

		}

		if (keydownList_N[4]) // p2 스킬
		{
			switch (m_pFramework->GetPlayer(myPlayerNum)->charNum)
			{
			case 1:
				SkillCoolTime = 8;
				break;
			case 2:
				SkillCoolTime = 5;
				break;
			case 3:
				SkillCoolTime = 12;
				break;
			}
			switch (m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus)
			{
			case 2:
			case 5:
			case 6:
			case 0:
				m_pFramework->GetPlayer(myPlayerNum)->Old_CharStat = m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus;
				m_pFramework->GetPlayer(myPlayerNum)->isSkill = TRUE;
				m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus = 12;
				m_pFramework->GetPlayer(myPlayerNum)->SkillCast(m_pFramework->GetPlayer(myPlayerNum)->x, m_pFramework->GetPlayer(myPlayerNum)->y, m_pFramework->GetPlayer(myPlayerNum)->Old_CharStat);
				break; // 앞 볼 때
			case 3:
			case 4:
			case 7:
			case 1:
				m_pFramework->GetPlayer(myPlayerNum)->Old_CharStat = m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus;
				m_pFramework->GetPlayer(myPlayerNum)->isSkill = TRUE;
				m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus = 13;
				m_pFramework->GetPlayer(myPlayerNum)->SkillCast(m_pFramework->GetPlayer(myPlayerNum)->x, m_pFramework->GetPlayer(myPlayerNum)->y, m_pFramework->GetPlayer(myPlayerNum)->Old_CharStat);
				break; // 뒤 볼 때
			}
		}
		//printf("Character Status 3_1 : %d\n", m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus);
		if (keydownList_N[5]) // p2 공격
		{
			switch (m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus)
			{
			case 2:
			case 5:
			case 6:
			case 0:
				m_pFramework->GetPlayer(myPlayerNum)->Old_CharStat = m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus;
				m_pFramework->GetPlayer(myPlayerNum)->isAttack = TRUE;
				m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus = 6;

				for (int i = 1; i <= MEMBERS; ++i)
				{
					if (i != myPlayerNum)
					{
						//상대 피격 처리(수정필)
						if ((abs(m_pFramework->GetPlayer(myPlayerNum)->x - m_pFramework->GetPlayer(i)->x) < 70) &&
							(abs(m_pFramework->GetPlayer(myPlayerNum)->y - m_pFramework->GetPlayer(i)->y) < 50))
						{
							//수정필 : 때린 애 서버에 실어서 보내야함
							m_pFramework->CTS.AttackedPlayerNum[i - 1] = true;
							m_pFramework->GetPlayer(i)->Old_CharStat = m_pFramework->GetPlayer(i)->CharacterStatus;
							m_pFramework->GetPlayer(i)->isAttacked = TRUE;
							m_pFramework->GetPlayer(i)->CharacterStatus = 9;
							//isp1LockDown = TRUE;
							if (m_pFramework->GetPlayer(i)->iHaveCoin)
							{
								m_pFramework->GetPlayer(i)->iHaveCoin = FALSE;
								CoinObject->OnCreate(m_pFramework->GetPlayer(i)->x, m_pFramework->GetPlayer(i)->y);
								coinLockDown = FALSE;
							}
						}
					}
				}

				break; // 앞 볼 때
			case 3:
			case 4:
			case 7:
			case 1:
				m_pFramework->GetPlayer(myPlayerNum)->Old_CharStat = m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus;
				m_pFramework->GetPlayer(myPlayerNum)->isAttack = TRUE;
				m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus = 7;

				//상대 피격 처리
				for (int i = 1; i <= MEMBERS; ++i)
				{
					if (i != myPlayerNum)
					{
						if ((abs(m_pFramework->GetPlayer(myPlayerNum)->x - m_pFramework->GetPlayer(i)->x) < 70) &&
							(abs(m_pFramework->GetPlayer(myPlayerNum)->y - m_pFramework->GetPlayer(i)->y) < 50))
						{
							m_pFramework->CTS.AttackedPlayerNum[i - 1] = true;
							m_pFramework->GetPlayer(i)->Old_CharStat = m_pFramework->GetPlayer(i)->CharacterStatus;
							m_pFramework->GetPlayer(i)->isAttacked = TRUE;
							m_pFramework->GetPlayer(i)->CharacterStatus = 8;
							//isp1LockDown = TRUE;
							if (m_pFramework->GetPlayer(i)->iHaveCoin)
							{
								m_pFramework->GetPlayer(i)->iHaveCoin = FALSE;
								CoinObject->OnCreate(m_pFramework->GetPlayer(i)->x, m_pFramework->GetPlayer(i)->y);
								coinLockDown = FALSE;
							}
						}
					}
				}
				break; // 뒤 볼 때
			}
		}
		//printf("Character Status 3_2 : %d\n", m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus);
		if (keydownList_N[6]) // p2 대시
		{
			if (m_pFramework->GetPlayer(myPlayerNum)->DashCoolTimer <= 0)
			{
				switch (m_pFramework->GetPlayer(myPlayerNum)->charNum)
				{
				case 1:
					m_pFramework->GetPlayer(myPlayerNum)->DashCoolTimer = 410;
					break;
				case 2:
					m_pFramework->GetPlayer(myPlayerNum)->DashCoolTimer = 300;
					break;
				case 3:
					m_pFramework->GetPlayer(myPlayerNum)->DashCoolTimer = 600;
					break;
				}
				m_pFramework->GetPlayer(myPlayerNum)->Old_CharStat = m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus;
				switch (m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus)
				{
				case 2:
				case 5:
				case 0:
					m_pFramework->GetPlayer(myPlayerNum)->isDash = TRUE;
					m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus = 10;
					break; // 앞 볼 때
				case 3:
				case 4:
				case 1:
					m_pFramework->GetPlayer(myPlayerNum)->isDash = TRUE;
					m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus = 11;
					break; // 뒤 볼 때
				}
			}
		}
		//printf("Character Status 3_3 : %d\n", m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus);

		//printf("Character Status 3_4 : %d\n", m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus);
	}

	if (pkey)
	{
		if (m_pFramework->GetPlayer(myPlayerNum)->isAttack != TRUE)
		{
			m_pFramework->GetPlayer(myPlayerNum)->isWalk = TRUE;
			if (m_pFramework->GetPlayer(myPlayerNum)->WalkingTimerTick++ > 3)
			{
				m_pFramework->GetPlayer(myPlayerNum)->WalkingTimerTick = 0;
				m_pFramework->GetPlayer(myPlayerNum)->WalkingImageTick++;
			}
		}
	}
	else
	{
		if (m_pFramework->GetPlayer(myPlayerNum)->isAttack != TRUE)
		{
			m_pFramework->GetPlayer(myPlayerNum)->isWalk = FALSE;
			switch (m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus)
			{
			case 2:
			case 5:
			case 6:
				m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus = 0;
				break;
			case 3:
			case 4:
			case 7:
				m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus = 1;
				break;
			}
		}
	}
}

void CIngameScene::Nevigator()
{
	//안 씀 VER3
	//this->distanceX = (m_pFramework->GetPlayer(2)->x - m_pFramework->GetPlayer(1)->x);
	//this->distanceY = (m_pFramework->GetPlayer(2)->y - m_pFramework->GetPlayer(1)->y);
	//if (distanceX < m_pFramework->GetRect().right / 4 && distanceX > -m_pFramework->GetRect().right / 4 &&
	//	distanceY < m_pFramework->GetRect().bottom / 2 && distanceY > -m_pFramework->GetRect().bottom / 2)
	//{
	//	this->TotalDistance = 0;
	//}
	//else
	//{
	//	this->TotalDistance = sqrt(distanceX * distanceX + distanceY * distanceY);				//거리 구하기
	//}
	//this->rad = atan2(distanceY, distanceX);
	//this->Angle = rad * 180 / 3.14;
	//this->Angle += 180;
}

void CIngameScene::Update(float fTimeElapsed)
{
	//승리 처리
	if (RemainTime <= 0)
	{
		isGameEnd = TRUE;
#ifdef _DEBUG
		printf("End\n");
#endif

		for (int i = 1; i <= MEMBERS; ++i)
		{
			m_pFramework->GetPlayer(i)->CharacterStatus = 15;
			if (m_pFramework->GetPlayer(i)->iHaveCoin == TRUE)
			{
				m_pFramework->GetPlayer(i)->CharacterStatus = 14;
			}
		}
	}

	if (isGameEnd == FALSE)
	{
		currentFDC--;
		if (currentFDC == 0)
		{
			//printf("Character Status 1 : %d\n", m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus);
			m_pFramework->NetGram.recvData(m_pFramework->STC);
			//printf("Character Status 2 : %d\n", m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus);
#ifdef _DEBUG
			m_pFramework->STC.explain();
#endif
			m_pFramework->CTS.APNclear();
		}

		//Nevigator();

		printf("STATE : %d\n", m_pFramework->STC.PlayerData[1].state);

		if (m_pFramework->STC.CoinState != 0)
		{
			CoinObject->SetDrawFalse();
			CoinObject->Setx(m_pFramework->STC.CoinX);
			CoinObject->Sety(m_pFramework->STC.CoinY);
			coinLockDown = TRUE;

			m_pFramework->GetPlayer(m_pFramework->STC.CoinState)->iHaveCoin = TRUE;
		}
		else
		{
			CoinObject->SetDrawTrue();


			CoinObject->Setx(m_pFramework->STC.CoinX);
			CoinObject->Sety(m_pFramework->STC.CoinY);


			for (int i = 1; i <= MEMBERS; ++i)
				m_pFramework->GetPlayer(i)->iHaveCoin = FALSE;

			//코인 Idle 애니메이션
			CoinObject->Update(fTimeElapsed);
			coinLockDown = FALSE;

		}

		//여기서 네트워크 처리


		KeyState();
		//printf("Character Status 3 : %d\n", m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus);
		CharacterState();
		//printf("Character Status 4 : %d\n", m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus);
		

		//printf("Character Status 5 : %d\n", m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus);
		if (m_pFramework->GetPlayer(myPlayerNum)->DashCoolTimer > 0)
		{
			m_pFramework->GetPlayer(myPlayerNum)->DashCoolTimer--;
		}

		//여기서 스킬을 사용했는지 판별(from네트워크)
		for (int i = 0; i < MEMBERS; i++)
		{
			if (m_pFramework->STC.PlayerData[i].AttackedPlayerNum[myPlayerNum - 1] && i + 1 != myPlayerNum)
			{
				//피격
				switch (m_pFramework->GetPlayer(i + 1)->charNum)
				{
				case 1:
					m_pFramework->GetPlayer(myPlayerNum)->SkillAttackedTimer = 120;
					break;
				case 2:
					m_pFramework->GetPlayer(myPlayerNum)->SkillAttackedTimer = 60;
					break;
				case 3:
					m_pFramework->GetPlayer(myPlayerNum)->SkillAttackedTimer = 180;
					break;
				}

				m_pFramework->GetPlayer(myPlayerNum)->isAttacked = TRUE;
				m_pFramework->GetPlayer(myPlayerNum)->CharacterStatus = 9;
				ismyPLockDown = TRUE;
				//if (m_pFramework->GetPlayer(myPlayerNum)->iHaveCoin)
				//{
				//	//m_pFramework->GetPlayer(myPlayerNum)->iHaveCoin = FALSE;
				//	//CoinObject->OnCreate(m_pFramework->GetPlayer(myPlayerNum)->x, m_pFramework->GetPlayer(myPlayerNum)->y);
				//	//coinLockDown = FALSE;	//이거 FALSE같은데
				//}
			}
		}

		//스킬 사용시 업데이트
		for (int i = 1; i <= MEMBERS; i++)
		{
			m_pFramework->GetPlayer(i)->Update(fTimeElapsed);
		}

		for (int i = 0; i < MEMBERS; i++)
		{
			//초기화
			m_pFramework->CTS.AttackedPlayerNum[i] = false;
			if (i != myPlayerNum - 1)
			{
				if (IntersectRect(&tmp, m_pFramework->GetPlayer(i + 1)->getRECT(), m_pFramework->GetPlayer(myPlayerNum)->CSkill->GetRECT())) // 스킬 공격 성공
				{
					////공격
					//switch (m_pFramework->GetPlayer(myPlayerNum)->charNum)
					//{
					//case 1:
					//	m_pFramework->GetPlayer(i+1)->SkillAttackedTimer = 120;
					//	break;
					//case 2:
					//	m_pFramework->GetPlayer(i + 1)->SkillAttackedTimer = 60;
					//	break;
					//case 3:
					//	m_pFramework->GetPlayer(i + 1)->SkillAttackedTimer = 180;
					//	break;
					//}

					//m_pFramework->GetPlayer(i + 1)->isAttacked = TRUE;
					//m_pFramework->GetPlayer(i + 1)->CharacterStatus = 9;

					m_pFramework->CTS.AttackedPlayerNum[i] = true;
					//if (m_pFramework->GetPlayer(myPlayerNum)->iHaveCoin)
					//{
					//	//m_pFramework->GetPlayer(myPlayerNum)->iHaveCoin = FALSE;
					//	CoinObject->OnCreate(m_pFramework->GetPlayer(myPlayerNum)->x, m_pFramework->GetPlayer(myPlayerNum)->y);
					//	coinLockDown = TRUE;	//이거 FALSE같은데
					//}
				}
			}

		}

		//네트워크 시간으로 시간 체크
		if (RemainTime != m_pFramework->STC.Time)
		{
			RemainTime = m_pFramework->STC.Time;

			TimerImage[0] = RemainTime / 10;
			TimerImage[1] = RemainTime % 10;

			if (SkillCoolTime > 0)
			{
				SkillCoolTime--;
			}
		}

		//이거는 서버쪽에 있어야 되겠는데?
		//if (CoinObject->GetbDraw() && coinLockDown == FALSE)
		//{
		//	if (abs(CoinObject->x() - m_pFramework->GetPlayer(myPlayerNum)->x) < 30 && abs(CoinObject->y() - m_pFramework->GetPlayer(myPlayerNum)->y) < 30)
		//	{
		//		//m_pFramework->GetPlayer(myPlayerNum)->iHaveCoin = TRUE;
		//		//CoinObject->SetDrawFalse();
		//	}
		//}

		//VER3
		//CObject_Player 이거 수정하고 CTS.set에서 수정한걸로 send하면 될거같음

		if (currentFDC == 0)
		{
			m_pFramework->CTS.set(m_pFramework->GetPlayer(myPlayerNum));
			m_pFramework->NetGram.sendData(m_pFramework->CTS);
			currentFDC = FrameDelayCnt;
		}
	}
	//for (int i = 0; i < nObjects; ++i)
		//ppObjects[i]->Update(fTimeElapsed);
}

void CIngameScene::AngleRender(HDC hdc)
{
	//printf("%lf\n", Angle);
	if (TotalDistance > 0)
	{
		if (Angle > 135 && Angle <= 225) // p1가 왼쪽
		{
			if ((sin(rad) + 0.5) * m_pFramework->GetRect().right / 2 > m_pFramework->GetRect().bottom * 0.93)
				anY = m_pFramework->GetRect().bottom * 0.93;
			else if ((sin(rad) + 0.5) * m_pFramework->GetRect().right / 2 < 0)
				anY = 0;
			else
				anY = (sin(rad) + 0.5) * m_pFramework->GetRect().right / 2;
			C_Angle[m_pFramework->GetPlayer(2)->charNum - 1].Draw(hdc, m_pFramework->GetRect().right / 2 - 50, anY, 50, 50);
			//
			C_Angle[m_pFramework->GetPlayer(1)->charNum - 1].Draw(hdc, 10 + m_pFramework->GetRect().right / 2, m_pFramework->GetRect().bottom - 50 - anY, 50, 50);
		}
		else if (Angle > 225 && Angle <= 315) // p1가 위쪽
		{
			if ((cos(rad) + 0.5) * m_pFramework->GetRect().right / 2 > m_pFramework->GetRect().right / 2 * 0.93)
				anX = m_pFramework->GetRect().right / 2 * 0.93;
			else if ((cos(rad) + 0.5) * m_pFramework->GetRect().right / 2 < 0)
				anX = 0;
			else
				anX = (cos(rad) + 0.5) * m_pFramework->GetRect().right / 2;
			C_Angle[m_pFramework->GetPlayer(2)->charNum - 1].Draw(hdc, anX, m_pFramework->GetRect().bottom - 55, 50, 50);
			//Ellipse(hdc, m_pFramework->GetRect().right / 4 + distanceX / 2 - 10, m_pFramework->GetRect().bottom - 65, m_pFramework->GetRect().right / 4 + distanceX/2 + 10, m_pFramework->GetRect().bottom - 45);
			C_Angle[m_pFramework->GetPlayer(1)->charNum - 1].Draw(hdc, -anX + m_pFramework->GetRect().right - 50, 20, 50, 50);
		}
		else if (Angle > 45 && Angle <= 135) // p1가 아래쪽
		{
			if ((cos(rad) + 0.5) * m_pFramework->GetRect().right / 2 > m_pFramework->GetRect().right / 2 * 0.93)
				anX = m_pFramework->GetRect().right / 2 * 0.93;
			else if ((cos(rad) + 0.5) * m_pFramework->GetRect().right / 2 < 0)
				anX = 0;
			else
				anX = (cos(rad) + 0.5) * m_pFramework->GetRect().right / 2;
			C_Angle[m_pFramework->GetPlayer(2)->charNum - 1].Draw(hdc, anX, 20, 50, 50);
			//Ellipse(hdc, m_pFramework->GetRect().right / 4 + distanceX / 2 - 10, 45, m_pFramework->GetRect().right / 4 + distanceX / 2 + 10, 65);
			C_Angle[m_pFramework->GetPlayer(1)->charNum - 1].Draw(hdc, -anX + m_pFramework->GetRect().right - 20, m_pFramework->GetRect().bottom - 50, 50, 50);
		}
		else								// p1가 오른쪽
		{
			if ((sin(rad) + 0.5) * m_pFramework->GetRect().right / 2 > m_pFramework->GetRect().bottom * 0.93)
				anY = m_pFramework->GetRect().bottom * 0.93;
			else if ((sin(rad) + 0.5) * m_pFramework->GetRect().right / 2 < 0)
				anY = 0;
			else
				anY = (sin(rad) + 0.5) * m_pFramework->GetRect().right / 2;
			C_Angle[m_pFramework->GetPlayer(2)->charNum - 1].Draw(hdc, 10, anY, 50, 50);
			//Ellipse(hdc, 10, m_pFramework->GetRect().bottom / 2 + distanceY / 2 - 10, 30, m_pFramework->GetRect().bottom / 2 + distanceY / 2 + 10);
			C_Angle[m_pFramework->GetPlayer(1)->charNum - 1].Draw(hdc, m_pFramework->GetRect().right - 30, m_pFramework->GetRect().bottom - 50 - anY, 50, 50);
		}
	}
}

//Network Accepted
void CIngameScene::Render(HDC hdc)
{
	keydown = FALSE;
	for (int i = 0; i < 14; i++)
	{
		keydownList[i] = FALSE;
		keydownList_N[(i % 7)] = FALSE;
	}
	pkey = false;

	//플레이어별 바닥타일
	BitBlt(*m_pFramework->GetPlayerDC(), m_pFramework->GetPlayer(myPlayerNum)->x - m_pFramework->GetPlayerRect(myPlayerNum).right / 2,
		m_pFramework->GetPlayer(myPlayerNum)->y - m_pFramework->GetPlayerRect(myPlayerNum).bottom / 2,
		m_pFramework->GetPlayerRect(myPlayerNum).right, m_pFramework->GetPlayerRect(myPlayerNum).bottom,
		*m_pFramework->GetTileDC(), m_pFramework->GetPlayer(myPlayerNum)->x - m_pFramework->GetPlayerRect(myPlayerNum).right / 2,
		m_pFramework->GetPlayer(myPlayerNum)->y - m_pFramework->GetPlayerRect(myPlayerNum).bottom / 2, SRCCOPY);
	//BitBlt(*m_pFramework->GetPlayerDC(), m_pFramework->GetPlayer(2)->x - m_pFramework->p1.right / 2, m_pFramework->GetPlayer(2)->y - m_pFramework->p1.bottom / 2, m_pFramework->p1.right, m_pFramework->p2.bottom,
	//	*m_pFramework->GetTileDC(), m_pFramework->GetPlayer(2)->x - m_pFramework->p1.right / 2, m_pFramework->GetPlayer(2)->y - m_pFramework->p2.bottom / 2, SRCCOPY);


	//플레이어
	m_pFramework->GetPlayer(myPlayerNum)->Render(m_pFramework->GetPlayerDC());
	for (int i = 1; i <= MEMBERS; ++i)
	{
		if (i != myPlayerNum)
			m_pFramework->GetPlayer(i)->Render(m_pFramework->GetPlayerDC());
	}

	//코인
	CoinObject->Render(&*m_pFramework->GetPlayerDC());

	//Ellipse(*m_pFramework->GetPlayerDC(), m_pFramework->GetPlayer(1)->x - 5, m_pFramework->GetPlayer(1)->y - 5, m_pFramework->GetPlayer(1)->x + 5, m_pFramework->GetPlayer(1)->y + 5);
	//Ellipse(*m_pFramework->GetPlayerDC(), m_pFramework->GetPlayer(2)->x - 5, m_pFramework->GetPlayer(2)->y - 5, m_pFramework->GetPlayer(2)->x + 5, m_pFramework->GetPlayer(2)->y + 5);

	//토탈로 옮기기
	BitBlt(hdc, 0, 0, m_pFramework->GetPlayerRect(myPlayerNum).right, m_pFramework->GetPlayerRect(myPlayerNum).bottom,
		*m_pFramework->GetPlayerDC(),
		m_pFramework->GetPlayer(myPlayerNum)->x - m_pFramework->GetPlayerRect(myPlayerNum).right / 2,
		m_pFramework->GetPlayer(myPlayerNum)->y - m_pFramework->GetPlayerRect(myPlayerNum).bottom / 2, SRCCOPY);

	//승패그림
	if (isGameEnd)
	{
		if (m_pFramework->GetPlayer(myPlayerNum)->iHaveCoin == TRUE)
		{
			WinC.Draw(hdc, m_pFramework->GetPlayerRect(myPlayerNum).right / 2 - 100, m_pFramework->GetPlayerRect(myPlayerNum).bottom / 2 - 300, 200, 200);
		}
		else
		{
			LoseC.Draw(hdc, m_pFramework->GetPlayerRect(myPlayerNum).right / 2 - 100, m_pFramework->GetPlayerRect(myPlayerNum).bottom / 2 - 300, 200, 200);
		}
	}


	//스킬 / 쿨타임
	for (int i = 0; i < 3; i++)
	{
		m_pFramework->GetPlayer(myPlayerNum)->Image.Skill_I[i].Draw(hdc, 30 + 80 * i, windowY - 120, 64, 64);
		//m_pFramework->GetPlayer(2)->Image.Skill_I[i].Draw(hdc,windowX - 280 +( 30 + 80 * i), windowY - 120, 64, 64);
	}

	if (SkillCoolTime > 0)
	{
		Rectangle(hdc, 30, windowY - 120, 30 + SkillCoolTime * 4, windowY - 120 + 64);
	}
	if (m_pFramework->GetPlayer(myPlayerNum)->DashCoolTimer > 0)
	{
		Rectangle(hdc, 30 + 160, windowY - 120, 30 + 160 + (m_pFramework->GetPlayer(myPlayerNum)->DashCoolTimer / 10), windowY - 120 + 64);
	}

	//UI
	C_IngameLine.Draw(hdc, 0, 0, m_pFramework->GetRect().right, m_pFramework->GetRect().bottom);
	C_Numbers[TimerImage[0]].Draw(hdc, m_pFramework->GetRect().right / 2 - 90, m_pFramework->GetRect().bottom / 15, 80, 80);
	C_Numbers[TimerImage[1]].Draw(hdc, m_pFramework->GetRect().right / 2 + 20, m_pFramework->GetRect().bottom / 15, 80, 80);


	//Coin 소지 표현'
	for (int i = 0; i < MEMBERS; i++)
	{
		if (m_pFramework->GetPlayer(i + 1)->iHaveCoin)
		{
			switch (i + 1)
			{
			case 1:
				CoinObject->Image.Draw(hdc, m_pFramework->GetRect().right / 2 - 90 - 60, windowY / 15, 50, 50);
				break;
			case 2:
				CoinObject->Image.Draw(hdc, m_pFramework->GetRect().right / 2 + 110, windowY / 15, 50, 50);
				break;
			case 3:

				CoinObject->Image.Draw(hdc, m_pFramework->GetRect().right / 2, windowY / 15 - 20, 50, 50);
				break;
			}
		}

		//오브젝트 렌더링
		for (int i = 0; i < nObjects; ++i)
			ppObjects[i]->Render(*m_pFramework->GetPlayerDC());

	}
}