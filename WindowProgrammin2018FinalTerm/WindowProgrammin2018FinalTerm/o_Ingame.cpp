#include <Windows.h>
#include "Ingame.h"
void IngameGetKey(HDC hDC, WPARAM wParam)
{
	switch (wParam)
	{

	}

}

void IngameGetKey(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_BACK:
		PostQuitMessage(0);
		break;
	}

}

void IngameGetChar(HDC hDC, WPARAM wParam)
{
	switch (wParam)
	{

	}

}

void IngameGetChar(WPARAM wParam)
{
	switch (wParam)
	{

	}

}
//
//void PlayerImageLoad(PlayerData* player, int characterNum)
//{
//	WCHAR LoadText[100];
//
//	for (int i = 1; i <= 4; i++)
//	{
//		wsprintf(LoadText, L"Graphic\\Char%d\\ATTACK\\%d.png",characterNum, i);
//		player->Attack[i-1].Load(LoadText);
//		wsprintf(LoadText, L"Graphic\\Char%d\\ATTACK_B\\%d.png", characterNum, i);
//		player->Attack_B[i-1].Load(LoadText);
//	}
//	for (int i = 1; i <= 6; i++)
//	{
//		wsprintf(LoadText, L"Graphic\\Char%d\\WALK\\%d.png", characterNum, i);
//		player->Walk[i-1].Load(LoadText);
//		wsprintf(LoadText, L"Graphic\\Char%d\\WALK_B\\%d.png", characterNum, i);
//		player->Walk_B[i-1].Load(LoadText);
//	}
//	for (int i = 1; i <= 2; i++)
//	{
//		wsprintf(LoadText, L"Graphic\\Char%d\\ATTACKED\\%d.png", characterNum, i);
//		player->Attacked[i - 1].Load(LoadText);
//		wsprintf(LoadText, L"Graphic\\Char%d\\ATTACKED_B\\%d.png", characterNum, i);
//		player->Attacked_B[i - 1].Load(LoadText);
//	}
//
//	wsprintf(LoadText, L"Graphic\\Char%d\\IDLE\\1.png", characterNum);
//	player->Idle.Load(LoadText);
//	wsprintf(LoadText, L"Graphic\\Char%d\\IDLE_B\\1.png", characterNum);
//	player->Idle_B.Load(LoadText);
//	wsprintf(LoadText, L"Graphic\\Char%d\\WIN\\1.png", characterNum);
//	player->Win.Load(LoadText);
//	wsprintf(LoadText, L"Graphic\\Char%d\\LOSE\\1.png", characterNum);
//	player->Lose.Load(LoadText);
//}

//void PlayerDraw(PlayerData* player, HDC PlayerMemDC)
//{
//	switch (player->CharacterStatus) // 0~1 = Idle  // 2~5 = Walk // 6~7 = Attack // 8~9 Attacked // 10 = Win // 11 = Lose
//	{
//	case 0:
//		player->Idle.Draw(PlayerMemDC, player->x - player->Idle.GetWidth() / 2, player->y - 5 - player->Idle.GetHeight() / 2, player->Idle.GetWidth(), player->Idle.GetHeight());
//		break;
//	case 1:
//		player->Idle_B.Draw(PlayerMemDC, player->x - player->Idle_B.GetWidth() / 2, player->y - 5 - player->Idle_B.GetHeight() / 2, player->Idle_B.GetWidth(), player->Idle_B.GetHeight());
//		break;
//	case 2:
//	case 5:
//		player->Walk[player->WalkingImageTick % 6].Draw(PlayerMemDC, player->x - player->Walk[player->WalkingImageTick % 6].GetWidth() / 2, player->y - 5 - player->Walk[player->WalkingImageTick % 6].GetHeight() / 2, player->Walk[player->WalkingImageTick % 6].GetWidth(), player->Walk[player->WalkingImageTick % 6].GetHeight());
//		break;
//	case 3:
//	case 4:
//		player->Walk_B[player->WalkingImageTick % 6].Draw(PlayerMemDC, player->x - player->Walk[player->WalkingImageTick % 6].GetWidth() / 2, player->y - 5 - player->Walk[player->WalkingImageTick % 6].GetHeight() / 2, player->Walk[player->WalkingImageTick % 6].GetWidth(), player->Walk[player->WalkingImageTick % 6].GetHeight());
//		break;
//	case 6:
//
//		break;
//	case 7:
//
//		break;
//	case 8:
//
//		break;
//	case 9:
//
//		break;
//	case 10:
//		player->Win.Draw(PlayerMemDC, player->x - player->Idle_B.GetWidth() / 2, player->y - 5 - player->Idle_B.GetHeight() / 2, player->Idle_B.GetWidth(), player->Idle_B.GetHeight());
//		break;
//	case 11:
//		player->Lose.Draw(PlayerMemDC, player->x - player->Idle_B.GetWidth() / 2, player->y - 5 - player->Idle_B.GetHeight() / 2, player->Idle_B.GetWidth(), player->Idle_B.GetHeight());
//		break;
//	}
//
//	
//}

//switch (player2->.CharacterStatus)  // 0~1 = Idle  // 2~5 = Walk // 6~7 = Attack // 8~9 Attacked // 10 = Win // 11 = Lose
//{
//case 0:
//	player2->.Idle.Draw(PlayerMemDC, player2->.x - player2->.Idle.GetWidth() / 2, player2->.y - 5 - player2->.Idle.GetHeight() / 2, player2->.Idle.GetWidth(), player2->.Idle.GetHeight());
//	break;
//case 1:
//	player2->.Idle_B.Draw(PlayerMemDC, player2->.x - player2->.Idle_B.GetWidth() / 2, player2->.y - 5 - player2->.Idle_B.GetHeight() / 2, player2->.Idle_B.GetWidth(), player2->.Idle_B.GetHeight());
//	break;
//case 2:
//case 5:
//	player2->.Walk[player2->.WalkingImageTick % 6].Draw(PlayerMemDC, player2->.x - player2->.Walk[player2->.WalkingImageTick % 6].GetWidth() / 2, player2->.y - 5 - player2->.Walk[player2->.WalkingImageTick % 6].GetHeight() / 2, player2->.Walk[player2->.WalkingImageTick % 6].GetWidth(), player2->.Walk[player2->.WalkingImageTick % 6].GetHeight());
//	break;
//case 3:
//case 4:
//	player2->.Walk_B[player2->.WalkingImageTick % 6].Draw(PlayerMemDC, player2->.x - player2->.Walk[player2->.WalkingImageTick % 6].GetWidth() / 2, player2->.y - 5 - player2->.Walk[player2->.WalkingImageTick % 6].GetHeight() / 2, player2->.Walk[player2->.WalkingImageTick % 6].GetWidth(), player2->.Walk[player2->.WalkingImageTick % 6].GetHeight());
//	break;
//case 6:
//	break;
//case 7:
//	break;
//case 8:
//	break;
//case 9:
//	break;
//case 10:
//	player2->.Win.Draw(PlayerMemDC, player2->.x - player2->.Idle_B.GetWidth() / 2, player2->.y - 5 - player2->.Idle_B.GetHeight() / 2, player2->.Idle_B.GetWidth(), player2->.Idle_B.GetHeight());
//	break;
//case 11:
//	player2->.Lose.Draw(PlayerMemDC, player2->.x - player2->.Idle_B.GetWidth() / 2, player2->.y - 5 - player2->.Idle_B.GetHeight() / 2, player2->.Idle_B.GetWidth(), player2->.Idle_B.GetHeight());
//	break;
//}