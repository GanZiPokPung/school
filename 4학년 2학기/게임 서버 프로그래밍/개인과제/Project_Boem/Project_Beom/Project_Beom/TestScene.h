#pragma once
#include "Scene.h"

class TestScene
	: public Scene
{
public:
	TestScene();
	virtual ~TestScene();

public:
	virtual bool Initialize();
	virtual int Update(const float& TimeDelta);
	virtual void Render(HDC hDC);
	virtual void Release();

private:
	bool InitChessBoardIndex();
	int UpdateChessInfo(const float& TimeDelta);

	void Do_Worker();
private:
	GameObject *m_Player = nullptr;
	GameObject *m_ChessBoard = nullptr;
	GameObject *m_Mouse = nullptr;

	vector<GameObject*> m_DummyPlayer;

	bool m_IsProcess = false;
	float m_Process = 0.f;
	float m_Acc = 1.f;
	int  m_SrcX = 0;
	int  m_SrcY = 0;
	int	 m_DestX = 0;
	int  m_DestY = 0;

	thread *m_worker = nullptr;
	float m_timeStack = 0.f;
	volatile bool  m_threadEndCheck = false;
};

