#pragma once
#include "GameObject.h"

class Player
	: public GameObject
{
public:
	Player();
	virtual ~Player();

public:
	const int& GetIDInfo() { return m_ID; }
	
public:
	void SetIDInfo(const int& id) { m_ID = id; }
	void SetPlayeable(const bool& check) { m_IsPlayeable = check; }
	void MoveToIndex(const int& indexX, const int& indexY);

public:
	// Playable Player를 위한 함수 나중에 수정이 필요함
	int InitOnce();
	int Update_Input(const float& TimeDelta);
	int Update_Position(const float& TimeDelta);

public:
	virtual bool Initialize();
	virtual int  Update(const float& TimeDelta);
	virtual void Render(HDC hdc);
	virtual void Release();

private:
	bool m_IsProcess = false;
	bool m_IsPlayeable = false;

	float m_SpeedAcc = 0.f;
	int m_ID = 0;

	int m_IsReceived = 0;
	float m_Process = 0.f;
	float m_Acc = 1.f;

	// pos
	int  m_SrcX = 0;
	int  m_SrcY = 0;
	int	 m_DestX = 0;
	int  m_DestY = 0;

	ClassPacket<INPUTINFO> m_recv_info_input;
};

