#include "stdafx.h"
#include "OBJECT_Coin.h"


OBJECT_Coin::OBJECT_Coin()
{

}


OBJECT_Coin::~OBJECT_Coin()
{

}

OBJECT_Coin::OBJECT_Coin(int Cx,int Cy)
{
	OnCreate(Cx, Cy);
}

void OBJECT_Coin::OnCreate(int Cx,int Cy)
{
	if (this->Image.IsNull())
		this->Image.Load(L"Graphic\\Item\\Coin.png");
	this->xx = Cx;
	this->yy = Cy;
	this->draw = true;
	
}

void OBJECT_Coin::Update(float fTimeElapsed)
{
	if (this->draw)
	{
		tick++;
		if ((tick / 30) % 2 == 0)
		{
			this->yy += 1;
		}
		else
		{
			this->yy -= 1;
		}
	}
	else
	{

	}
}

void OBJECT_Coin::Render(HDC * hdc)
{
	if (this->draw)
	{
		//this->Image.StretchBlt(*hdc, (this->x) - 15, (this->y) - 15, 30, 30, 0, 0, 256, 256, SRCCOPY);
		this->Image.Draw(*hdc, (this->xx) - 20, (this->yy) - 20, 40, 40, 0, 0, 256, 256);
	}
	else
	{

	}
}

void OBJECT_Coin::SetDrawFalse()
{
	this->draw = false;
}
void OBJECT_Coin::SetDrawTrue()
{
	this->draw = true;
}

int OBJECT_Coin::x()
{
	return this->xx;
}

int OBJECT_Coin::y()
{
	return this->yy;
}

void OBJECT_Coin::Setx(int cx)
{
	this->xx = cx;
}

void OBJECT_Coin::Sety(int cy)
{
	this->yy = cy;
}

bool OBJECT_Coin::GetbDraw()
{
	return this->draw;
}

