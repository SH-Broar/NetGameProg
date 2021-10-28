#pragma once

class CObject
{
public:
	CObject();
	~CObject();

	void Update(float fTimeElapsed);
	void Render(HDC hdc);

};

