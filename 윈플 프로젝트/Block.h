#pragma once

#include "system.h"
class Block
{
public:
	int Object_Num;
	CImage Image;

	int xPos;
	int yPos;
	RECT Box;

	BOOL isOn;
	int HowManyCount;

	BOOL isVisible;

	float Speed;

	BOOL CrushLeft;
	BOOL CrushRight;
	BOOL CrushDown;
	BOOL FallOut;

	BOOL End;
	BOOL ThornLeft;

	BOOL canScore;

	BOOL isLeft;

	Block();
	virtual void Loadimage(BOOL);
	void SetObjectNumber(int);
	virtual void Draw(HDC);
	void SetPosition(int, int);
	virtual void Reposition();
	virtual int Collision(RECT, BOOL);
	void OnCheck();
	virtual void OffCheck();
	virtual void Update();
	virtual void Push_Space();
	virtual void Create(int);
	virtual void Destroy();
};

class ColorBlock :public Block
{
public:
	BOOL CheckPupple;

	ColorBlock(BOOL);
	virtual void Loadimage(BOOL);
	virtual int Collision(RECT, BOOL);
};

class BreakBlock :public Block
{
public:
	BreakBlock() = default;

	virtual void Loadimage(BOOL);
	virtual void OffCheck();
};

class ThornBlock : public Block
{
public:
	CImage Image_2;
	BOOL CheckPupple;

	ThornBlock(BOOL);
	virtual void Loadimage(BOOL);
	virtual void Draw(HDC);
	virtual void Reposition();
	virtual int Collision(RECT, BOOL);
	virtual void Update();
	virtual void Push_Space();
	virtual void Destroy();
};

class DaggarBlock : public Block
{
public:
	CImage Image[10];

	int ImageCount;

	DaggarBlock();
	virtual void Loadimage(BOOL);
	virtual void Draw(HDC);
	virtual void Reposition();
	virtual int Collision(RECT, BOOL);
	virtual void Update();
	virtual void Create(int);
	virtual void Destroy();
};

class WallBlock : public Block
{
public:

	WallBlock();
	virtual void Loadimage(BOOL);
	virtual void Draw(HDC);
	virtual void Reposition();
	virtual int Collision(RECT, BOOL);
	virtual void Create(int);
	virtual void Update();
};