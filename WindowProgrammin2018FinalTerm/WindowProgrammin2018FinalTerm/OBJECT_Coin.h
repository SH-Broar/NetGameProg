#pragma once
#include "Object.h"
class OBJECT_Coin : public CObject
{
private:
	int xx = 0;
	int yy = 0;

	int tick = 0;

	bool draw = true;
	
public:
	OBJECT_Coin();
	OBJECT_Coin(int Cx, int Cy);
	~OBJECT_Coin();
	CImage Image;

	void OnCreate(int Cx, int Cy);
	void Update(float fTimeElapsed);
	void Render(HDC* hdc);

	void SetDrawFalse();
	void SetDrawTrue();

	int x();
	int y();
	void Setx(int cx);
	void Sety(int cy);
	bool GetbDraw();
};

