#pragma once
#include "Object.h"

class OBJECT_Skill : public CObject
{
private:
	int MaxImageNumOfSkill;

	int skillNum;
	int skillx;
	int skilly;

	int direction = -1;

	int ImageTick;
	int ImageTimerTick;

	HDC skillDC;
	HBITMAP skillBitmap;
	POINT point[3];
	RECT retmp;
public:
	CImage Skill[10];
	OBJECT_Skill();
	OBJECT_Skill(int characterNum);
	~OBJECT_Skill();

	RECT* GetRECT();
	void OnCreate(int x, int y, int Old_CharStat);
	void OnDestroy();
	bool Update(float fTimeElapsed);
	void Render(HDC* hdc);
};

