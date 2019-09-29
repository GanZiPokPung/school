#pragma once
#include "GameObject.h"

class Player
	: public GameObject
{
public:
	Player();
	virtual ~Player();

public:
	int Update_Input(const float& TimeDelta);
	int Update_Position(const float& TimeDelta, const DIRECTION& Direction);

public:
	virtual bool Initialize();
	virtual int  Update(const float& TimeDelta);
	virtual void Render(HDC hdc);
	virtual void Release();

private:
	float m_SpeedAcc = 0.f;
	DWORD m_Dir = 0;
	DIRECTION m_Direction = DIR_END;
};

