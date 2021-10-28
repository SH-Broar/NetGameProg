#pragma once
#include <atlimage.h>
//=====================================
// Ingame �Լ� ���
//=====================================

typedef struct _PlayerData
{
	CImage Attack[4];
	CImage Attack_B[4];
	CImage Attacked[2];
	CImage Attacked_B[2];
	CImage Idle;
	CImage Idle_B;
	CImage Walk[6];
	CImage Walk_B[6];
	CImage Win;
	CImage Lose;

	BOOL isWalk = FALSE;
	int WalkingTimerTick = 0; //Ÿ�̸ӵ����� 10�Ǹ� Idle��
	int WalkingImageTick = 0;

	int AttackTimerTick = 0; //
	int AttackImageTick = 0;

	int x;
	int y;

	int CharacterStatus = 0; // 0~1 = Idle  // 2~5 = Walk // 6~7 = Attack // 8 = Win // 9 = Lose

}PlayerData;

void IngameGetKey(HDC hDC, WPARAM wParam);
void IngameGetKey(WPARAM wParam);

void IngameGetChar(HDC hDC, WPARAM wParam);
void IngameGetChar(WPARAM wParam);

void PlayerImageLoad(PlayerData* player,int characterNum);
void PlayerDraw(PlayerData* player, HDC PlayerMemDC);