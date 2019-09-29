#pragma once
#include "GameObject.h"

class Board 
	: public GameObject
{
public:
	Board();
	virtual ~Board();

public:
	virtual bool Initialize();
	virtual int  Update(const float& TimeDelta);
	virtual void Render(HDC hdc);
	virtual void Release();
};

