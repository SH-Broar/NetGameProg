#include "stdafx.h"
#include "OBJECT_Skill.h"

OBJECT_Skill::OBJECT_Skill()
{

}
OBJECT_Skill::OBJECT_Skill(int characterNum)
{
	WCHAR LoadText[100];
	if (characterNum == 1)
	{
		this->MaxImageNumOfSkill = 9;
	}
	else if (characterNum == 2)
	{
		this->MaxImageNumOfSkill = 6;
	}
	else
	{
		this->MaxImageNumOfSkill = 8;
	}
	for (int i = 1; i <= this->MaxImageNumOfSkill; i++)
	{
		//printf("%d", i);
		wsprintf(LoadText, L"Graphic\\Char%d\\SKILL_E\\%d.png", characterNum, i);
		this->Skill[i - 1].Load(LoadText);
	}
	this->skillNum = characterNum;
	ImageTick = 0;
	ImageTimerTick = 0;
}



OBJECT_Skill::~OBJECT_Skill()
{

}

RECT* OBJECT_Skill::GetRECT()
{
	switch (direction)
	{
	case 2:
	case 0:
		retmp.left = skillx - Skill[0].GetWidth();
		retmp.top = skilly - Skill[0].GetHeight() / 2;
		retmp.right = skillx;
		retmp.bottom = skilly + Skill[0].GetHeight() / 2;
		break;
	case 5:
		retmp.left = skillx - Skill[0].GetHeight() / 2;
		retmp.top = skilly;
		retmp.right = skillx + Skill[0].GetWidth() / 2;
		retmp.bottom = skilly + Skill[0].GetWidth();
		break;
	case 3:
		retmp.left = skillx - Skill[0].GetHeight() / 2;
		retmp.top = skilly - Skill[0].GetWidth();
		retmp.right = skillx + Skill[0].GetWidth() / 2;
		retmp.bottom = skilly;
		break;
	case 4:
	case 1:
		retmp.left = skillx;
		retmp.top = skilly - Skill[0].GetHeight() / 2;
		retmp.right = skillx + Skill[0].GetWidth();
		retmp.bottom = skilly + Skill[0].GetHeight() / 2;
		break;
	case -1:
		return NULL;
		break;
	}
	
	return &retmp;
}

void OBJECT_Skill::OnCreate(int x,int y,int Old_CharStat)
{
	this->skillx = x;
	this->skilly = y;

	//printf("*********************%d\n", Old_CharStat);
	direction = Old_CharStat;
}

void OBJECT_Skill::OnDestroy()
{
	ImageTick = 0;
	ImageTimerTick = 0;
	direction = -1;
}

bool OBJECT_Skill::Update(float fTimeElapsed)
{
	ImageTimerTick++;
	switch (this->skillNum)
	{
	case 1:
		if (ImageTimerTick > 3)
		{
			ImageTick++;
			ImageTimerTick = 0;
			if (ImageTick >= MaxImageNumOfSkill)
			{
				OnDestroy();
				return true;
			}
		}
		break;
	case 2:
		if (ImageTimerTick > 2)
		{
			ImageTick++;
			ImageTimerTick = 0;
			if (ImageTick >= MaxImageNumOfSkill)
			{
				OnDestroy();
				return true;
			}
		}
		break;
	case 3:
		if (ImageTimerTick > 1)
		{
			ImageTick++;
			ImageTimerTick = 0;
			switch (direction)
			{
			case 2:
			case 0:
				this->skillx -= 20;
				break;
			case 5:
				this->skilly += 20;
				break;
			case 3:
				this->skilly -= 20;
				break;
			case 4:
			case 1:
				this->skillx += 20;
				break;
			}
			if (ImageTick >= MaxImageNumOfSkill)
			{
				ImageTick = 0;
				return true;
			}
			if (skillx < -100 || skillx > 6500 || skilly < -100 || skilly > 6500)
			{
				OnDestroy();
				direction = -1;
			}

		}
		break;
	}
	return false;
}

void OBJECT_Skill::Render(HDC* hdc)
{
	switch (direction)
	{
	case 2:
	case 0:
		skillDC = CreateCompatibleDC(Skill[ImageTick].GetDC());
		skillBitmap = CreateCompatibleBitmap(skillDC, Skill[ImageTick].GetWidth(), Skill[ImageTick].GetHeight());
		SelectObject(skillDC, skillBitmap);

		Skill[ImageTick].Draw(skillDC, 0, 0);
		TransparentBlt(*hdc, this->skillx - Skill[ImageTick].GetWidth(), this->skilly - Skill[ImageTick].GetHeight() / 2, Skill[ImageTick].GetWidth(), Skill[ImageTick].GetHeight(),
			skillDC, 0, 0, Skill[ImageTick].GetWidth(), Skill[ImageTick].GetHeight(), RGB(0, 0, 0));
		//Skill[ImageTick].Draw(*hdc, this->skillx - Skill[ImageTick].GetWidth(), this->skilly - Skill[ImageTick].GetHeight() / 2);
		break; // 왼
	case 5:
		this->point[0].x = skillx - Skill[ImageTick].GetHeight() / 2;
		this->point[0].y = skilly + Skill[ImageTick].GetWidth();
		this->point[1].x = skillx - Skill[ImageTick].GetHeight() / 2;
		this->point[1].y = skilly;
		this->point[2].x = skillx + Skill[ImageTick].GetHeight() / 2;
		this->point[2].y = skilly + Skill[ImageTick].GetWidth();
		skillDC = CreateCompatibleDC(Skill[ImageTick].GetDC());
		skillBitmap = CreateCompatibleBitmap(skillDC, Skill[ImageTick].GetWidth(), Skill[ImageTick].GetHeight());
		SelectObject(skillDC, skillBitmap);

		Skill[ImageTick].Draw(skillDC, 0, 0);
		PlgBlt(*hdc, point, skillDC, 0, 0, Skill[ImageTick].GetWidth(), Skill[ImageTick].GetHeight(), skillBitmap, 0, 0);

		//Skill[ImageTick].ReleaseDC();
		break; // 아래
	case 3:
		this->point[0].x = skillx - Skill[ImageTick].GetHeight() / 2;
		this->point[0].y = skilly - Skill[ImageTick].GetWidth();
		this->point[1].x = skillx - Skill[ImageTick].GetHeight() / 2;
		this->point[1].y = skilly;
		this->point[2].x = skillx + Skill[ImageTick].GetHeight() / 2;
		this->point[2].y = skilly - Skill[ImageTick].GetWidth();
		skillDC = CreateCompatibleDC(Skill[ImageTick].GetDC());
		skillBitmap = CreateCompatibleBitmap(skillDC, Skill[ImageTick].GetWidth(), Skill[ImageTick].GetHeight());
		SelectObject(skillDC, skillBitmap);

		Skill[ImageTick].Draw(skillDC, 0, 0);
		PlgBlt(*hdc, point, skillDC, 0, 0, Skill[ImageTick].GetWidth(), Skill[ImageTick].GetHeight(), skillBitmap, 0, 0);
		break; // 위
	case 4:
	case 1:
		skillDC = CreateCompatibleDC(Skill[ImageTick].GetDC());
		skillBitmap = CreateCompatibleBitmap(skillDC, Skill[ImageTick].GetWidth(), Skill[ImageTick].GetHeight());
		SelectObject(skillDC, skillBitmap);

		Skill[ImageTick].Draw(skillDC, 0, 0);
		
		StretchBlt(*hdc, this->skillx + Skill[ImageTick].GetWidth(), this->skilly - Skill[ImageTick].GetHeight() / 2, -Skill[ImageTick].GetWidth(), Skill[ImageTick].GetHeight(),
			skillDC, 0, 0, Skill[ImageTick].GetWidth(), Skill[ImageTick].GetHeight(), SRCPAINT);
		break; // 오른
	}
}

