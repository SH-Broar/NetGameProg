#include "Block.h"

Block::Block()
{
	isVisible = TRUE;
	xPos = 0;
	yPos = 0;
	Speed = 5;
	HowManyCount = 0;
	End = FALSE;
	canScore = TRUE;
	SetObjectNumber(0);
}

void Block::Loadimage(BOOL isPupple)
{
	Image.Load(TEXT("Resource/배경/블록.png"));
}

void Block::SetObjectNumber(int num)
{
	Object_Num = num;
}

void Block::Draw(HDC hDC)
{
	if (isVisible)
	{
		Image.Draw(hDC, Box.left, Box.top, Block_Size_Lenght, Block_Size_Raw, 0, 0, Block_IMG_Size, Block_IMG_Size);
	}
}

void Block::SetPosition(int Set_xPos, int Set_yPos)
{
	xPos = Set_xPos;
	yPos = Set_yPos;
	Reposition();
}

void Block::Reposition()
{
	Box = { xPos, yPos, xPos + Block_Size_Lenght, yPos + Block_Size_Raw };
}

int Block::Collision(RECT PlayerBox, BOOL isPupple)
{
	if (isVisible)
	{
		RECT CrushCheck;
		if (IntersectRect(&CrushCheck, &Box, &PlayerBox))
		{
			if (CrushCheck.right == PlayerBox.right && CrushCheck.top == Box.top && (PlayerBox.left + PlayerBox.right) / 2 < Box.left)
			{
				CrushLeft = TRUE;
			}
			if (CrushCheck.left == PlayerBox.left && CrushCheck.bottom == Box.bottom && (PlayerBox.left + PlayerBox.right) / 2 > Box.right)
			{
				CrushRight = TRUE;
			}
			if ((CrushCheck.left == PlayerBox.left && CrushCheck.right == PlayerBox.right && PlayerBox.top > Box.top && PlayerBox.top < Box.bottom)||
				(CrushCheck.left == Box.left && CrushCheck.right == PlayerBox.right && PlayerBox.top > Box.top && PlayerBox.top < Box.bottom)||
				(CrushCheck.left == PlayerBox.left && CrushCheck.right == Box.right && PlayerBox.top > Box.top && PlayerBox.top < Box.bottom))
			{
				CrushDown = TRUE;
			}
			if ((CrushCheck.right == PlayerBox.right && CrushCheck.top == Box.top && (PlayerBox.left + PlayerBox.right) / 2 > Box.left) 
				&& (PlayerBox.top + PlayerBox.bottom) / 2 > Box.top && (PlayerBox.top + PlayerBox.bottom) / 2 < Box.bottom ||
				(CrushCheck.left == PlayerBox.left && CrushCheck.bottom == Box.bottom && (PlayerBox.left + PlayerBox.right) / 2 < Box.right)
				&& (PlayerBox.top + PlayerBox.bottom) / 2 > Box.top && (PlayerBox.top + PlayerBox.bottom) / 2 < Box.bottom ||
				(CrushCheck.left == PlayerBox.left && CrushCheck.right == PlayerBox.right && (PlayerBox.top + PlayerBox.bottom) / 2 > Box.top
					&& (PlayerBox.top + PlayerBox.bottom) / 2 < Box.bottom))
			{
				FallOut = TRUE;
			}
			OnCheck();
			return Object_Num;
		}
	}
	return 0;
}

void Block::OnCheck()
{
	if (HowManyCount == 0)
	{
		HowManyCount++;
	}
	isOn = TRUE;
}

void Block::OffCheck()
{
	isOn = FALSE;
}

void Block::Update()
{
	yPos -= Speed;
	Speed += 0.001f;
	if (yPos + Block_Size_Raw < 0)
	{
		Destroy();
	}
	Reposition();
}

void Block::Push_Space()
{
}

void Block::Create(int xPos)
{
	isVisible = TRUE;

	SetPosition(xPos * 90 + 30, Window_Clinet_Bottom);
}

void Block::Destroy()
{
	isVisible = FALSE;
	canScore = TRUE;
}

//==============================================================================================
ColorBlock::ColorBlock(BOOL isPupple)
{
	if (isPupple)
	{
		CheckPupple = TRUE;
	}
	else
	{
		CheckPupple = FALSE;
	}
	canScore = TRUE;
}

void ColorBlock::Loadimage(BOOL isPupple)
{
	if (isPupple)
	{
		Image.Load(TEXT("Resource/배경/보라 블록.png"));
	}
	else
	{
		Image.Load(TEXT("Resource/배경/노란 블록.png"));
	}
}

int ColorBlock::Collision(RECT PlayerBox, BOOL isPupple)
{
	if (isVisible)
	{
		RECT CrushCheck;
		if (CheckPupple && isPupple || !CheckPupple && !isPupple)
		{
			if (IntersectRect(&CrushCheck, &Box, &PlayerBox))
			{
				if (CrushCheck.right == PlayerBox.right && CrushCheck.top == Box.top && (PlayerBox.left + PlayerBox.right) / 2 < Box.left)
				{
					CrushLeft = TRUE;
				}
				if (CrushCheck.left == PlayerBox.left && CrushCheck.bottom == Box.bottom && (PlayerBox.left + PlayerBox.right) / 2 > Box.right)
				{
					CrushRight = TRUE;
				}
				if ((CrushCheck.left == PlayerBox.left && CrushCheck.right == PlayerBox.right && PlayerBox.top > Box.top && PlayerBox.top < Box.bottom) ||
					(CrushCheck.left == Box.left && CrushCheck.right == PlayerBox.right && PlayerBox.top > Box.top && PlayerBox.top < Box.bottom) ||
					(CrushCheck.left == PlayerBox.left && CrushCheck.right == Box.right && PlayerBox.top > Box.top && PlayerBox.top < Box.bottom))
				{
					CrushDown = TRUE;
				}
				if ((CrushCheck.right == PlayerBox.right && CrushCheck.top == Box.top && (PlayerBox.left + PlayerBox.right) / 2 > Box.left)
					&& (PlayerBox.top + PlayerBox.bottom) / 2 > Box.top && (PlayerBox.top + PlayerBox.bottom) / 2 < Box.bottom ||
					(CrushCheck.left == PlayerBox.left && CrushCheck.bottom == Box.bottom && (PlayerBox.left + PlayerBox.right) / 2 < Box.right)
					&& (PlayerBox.top + PlayerBox.bottom) / 2 > Box.top && (PlayerBox.top + PlayerBox.bottom) / 2 < Box.bottom ||
					(CrushCheck.left == PlayerBox.left && CrushCheck.right == PlayerBox.right && (PlayerBox.top + PlayerBox.bottom) / 2 > Box.top
						&& (PlayerBox.top + PlayerBox.bottom) / 2 < Box.bottom))
				{
					FallOut = TRUE;
				}
				OnCheck();
				return Object_Num;
			}
		}
	}
	return 0;
}
//==============================================================================================

void BreakBlock::Loadimage(BOOL isPupple)
{
	Image.Load(TEXT("Resource/배경/깨진 블록.png"));
}

void BreakBlock::OffCheck()
{
	if (HowManyCount == 1)
	{
		HowManyCount = 0;
		Destroy();
	}
	Reposition();

	isOn = FALSE;
}
//==============================================================================================

ThornBlock::ThornBlock(BOOL isPupple)
{
	if (isPupple)
	{
		CheckPupple = TRUE;
	}
	else
	{
		CheckPupple = FALSE;
	}
	canScore = FALSE;
	Speed = 10;
}

void ThornBlock::Loadimage(BOOL isPupple)
{
	Image.Load(TEXT("Resource/배경/보라 가시.png"));
	Image_2.Load(TEXT("Resource/배경/노란 가시.png"));
}

void ThornBlock::Draw(HDC hDC)
{
	if (isVisible)
	{
		if (CheckPupple)
		{
			Image.Draw(hDC, Box.left, Box.top, Thorn_Size, Thorn_Size, 0, 0, Thorn_Size, Thorn_Size);
		}
		else
		{
			Image_2.Draw(hDC, Box.left, Box.top, Thorn_Size, Thorn_Size, 0, 0, Thorn_Size, Thorn_Size);
		}
	}
}

void ThornBlock::Reposition()
{
	Box = { xPos, yPos, xPos + Thorn_Size, yPos + Thorn_Size };
}

int ThornBlock::Collision(RECT PlayerBox, BOOL isPupple)
{
	if (isVisible)
	{
		RECT CrushCheck;
		if (CheckPupple && isPupple || !CheckPupple && !isPupple)
		{
			if (IntersectRect(&CrushCheck, &Box, &PlayerBox))
			{
				End = TRUE;
				return Object_Num;
			}
		}
	}
	return 0;
}

void ThornBlock::Update()
{
	yPos -= Speed;
	if (ThornLeft)
	{
		xPos -= Speed;
		if (xPos < Window_Clinet_Left)
		{
			xPos += Speed;
			ThornLeft = FALSE;
		}
	}
	else
	{
		xPos += Speed;
		if (xPos + Thorn_Size > Window_Clinet_Right)
		{
			xPos -= Speed;
			ThornLeft = TRUE;
		}
	}
	if (yPos + Thorn_Size < 0)
	{
		Destroy();
	}
	Reposition();
}

void ThornBlock::Push_Space()
{
	if (CheckPupple)
	{
		CheckPupple = FALSE;
	}
	else
	{
		CheckPupple = TRUE;
	}
}
void ThornBlock::Destroy()
{
	isVisible = FALSE;
}
//==============================================================================================
DaggarBlock::DaggarBlock()
{
	ImageCount = 0;
	canScore = FALSE;
}

void DaggarBlock::Loadimage(BOOL)
{
	Image[0].Load(TEXT("Resource/배경/표창 블럭_1.png"));
	Image[1].Load(TEXT("Resource/배경/표창 블럭_2.png"));
	Image[2].Load(TEXT("Resource/배경/표창 블럭_3.png"));
	Image[3].Load(TEXT("Resource/배경/표창 블럭_4.png"));
	Image[4].Load(TEXT("Resource/배경/표창 블럭_5.png"));
	Image[5].Load(TEXT("Resource/배경/표창 블럭_6.png"));
	Image[6].Load(TEXT("Resource/배경/표창 블럭_7.png"));
	Image[7].Load(TEXT("Resource/배경/표창 블럭_8.png"));
	Image[8].Load(TEXT("Resource/배경/표창 블럭_9.png"));
	Image[9].Load(TEXT("Resource/배경/표창 블럭_10.png"));
}

void DaggarBlock::Draw(HDC hDC)
{
	if (isVisible)
	{
		for (int i = 0; i < 10; i++)
		{
			if (ImageCount == i)
			{
				Image[i].Draw(hDC, Box.left, Box.top, Block_IMG_Size, Block_IMG_Size, 0, 0, Block_IMG_Size, Block_IMG_Size);
			}
		}
	}
}

void DaggarBlock::Reposition()
{
	Box = { xPos + 10, yPos + 10, xPos + Block_IMG_Size - 10, yPos + Block_IMG_Size - 10};
}

int DaggarBlock::Collision(RECT PlayerBox, BOOL isPupple)
{
	if (isVisible)
	{
		RECT CrushCheck;
		if (IntersectRect(&CrushCheck, &Box, &PlayerBox))
		{
			End = TRUE;
			return Object_Num;
		}

	}
	return 0;
}

void DaggarBlock::Update()
{
	ImageCount++;
	if (ImageCount > 9)
	{
		ImageCount = 0;
	}
	xPos -= DaggerSpeed;
	if (xPos < Window_Clinet_Left - Image_Size)
	{
		Destroy();
	}
	Reposition();
}

void DaggarBlock::Create(int yPos)
{
	isVisible = TRUE;

	SetPosition(Window_Clinet_Right, yPos);
}

void DaggarBlock::Destroy()
{
	isVisible = FALSE;
}
//==============================================================================================
WallBlock::WallBlock()
{
	canScore = FALSE;
	isVisible = TRUE;
	isLeft = TRUE;
}
void WallBlock::Loadimage(BOOL)
{
	Image.Load(TEXT("Resource/배경/벽.png"));
}

void WallBlock::Draw(HDC hDC)
{
	if (isVisible)
	{
		Image.Draw(hDC, Box.left, Box.top, WallBlock_Size, WallBlock_Size, 0, 0, Block_IMG_Size, Block_IMG_Size);
	}
}

void WallBlock::Reposition()
{
	Box = { xPos, yPos, xPos + WallBlock_Size, yPos + WallBlock_Size };
}

int WallBlock::Collision(RECT PlayerBox, BOOL)
{
	if (isVisible)
	{
		RECT CrushCheck;
		if (IntersectRect(&CrushCheck, &Box, &PlayerBox))
		{
			if (CrushCheck.right == PlayerBox.right && CrushCheck.top == Box.top && (PlayerBox.left + PlayerBox.right) / 2 < Box.left)
			{
				CrushLeft = TRUE;
			}
			if (CrushCheck.left == PlayerBox.left && CrushCheck.bottom == Box.bottom && (PlayerBox.left + PlayerBox.right) / 2 > Box.right)
			{
				CrushRight = TRUE;
			}
			return Object_Num;
		}
	}
	return 0;
}

void WallBlock::Create(int xPos)
{
	isVisible = TRUE;

	if (xPos > 0)
	{
		SetPosition(Window_Clinet_Left, Window_Clinet_Bottom);
	}
	else if (xPos < 0)
	{
		SetPosition(Window_Clinet_Right - WallBlock_Size, Window_Clinet_Bottom);
	}
}

void WallBlock::Update()
{
	yPos -= Speed;
	Speed += 0.001f;
	if (yPos + WallBlock_Size < 0)
	{
		Destroy();
		if (isLeft)
		{
			Create(1);
		}
		else
		{
			Create(-1);
		}
	}
	Reposition();
}