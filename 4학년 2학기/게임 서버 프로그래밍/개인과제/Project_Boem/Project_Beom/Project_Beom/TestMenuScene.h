#pragma once
#include "Scene.h"

class TestMenuScene
	: public Scene
{
public:
	TestMenuScene();
	virtual ~TestMenuScene();

public:
	virtual bool Initialize();
	virtual int Update(const float& TimeDelta);
	virtual void Render(HDC hDC);
	virtual void Release();

};

