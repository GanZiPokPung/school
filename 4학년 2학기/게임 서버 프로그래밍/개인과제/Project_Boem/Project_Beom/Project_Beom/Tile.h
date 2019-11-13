#pragma once
#include "GameObject.h"

class Tile
	: public GameObject
{
public:
	Tile();
	virtual ~Tile();

public:
	void SetWhite(bool check) { m_isWhite = check; }

public:
	virtual bool Initialize();
	virtual int  Update(const float& TimeDelta);
	virtual void Render(HDC hdc);
	virtual void Release();

private:
	bool m_isWhite = false;
	CameraManager* m_CamMgr = nullptr;
	bool m_isRender = false;

	const POSITION& m_CamPos;
};

