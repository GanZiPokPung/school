#include "stdafx.h"
#include "CameraManager.h"

#include "GameObject.h"

CameraManager::CameraManager()
{
	ZeroMemory(&m_Pos, sizeof(POSITION));
	ZeroMemory(&m_Pos, sizeof(RESOLUTION));
	m_Offset.X = (WINSIZE_X / 2);
	m_Offset.Y = (WINSIZE_Y / 2);
}

CameraManager::~CameraManager()
{
	Release();
}

void CameraManager::SetPos(int x, int y)
{
	if (nullptr == m_Target)
		return;

	m_Pos.X = -(x - (int)((int)WINSIZE_X / 2));
	m_Pos.Y = -(y - (int)((int)WINSIZE_Y / 2));
}

void CameraManager::SetTarget(GameObject * target)
{
	m_Target = target;

	GAMEOBJINFO targetInfo = m_Target->GetInfo();
	SetPos(targetInfo.Pos_X, targetInfo.Pos_Y);
}

void CameraManager::SetResolution(int x, int y)
{
	m_Resolution.X = x;
	m_Resolution.Y = y;
}

void CameraManager::SetOffset(int x, int y)
{
	m_Offset.X = (WINSIZE_X / 2) - x;
	m_Offset.Y = (WINSIZE_Y / 2) - y;
}

int CameraManager::Update(const float & TimeDelta)
{
	if (nullptr != m_Target)
	{
		GAMEOBJINFO targetInfo = m_Target->GetInfo();

		POSITION temp = m_Pos;

		if ((targetInfo.Pos_X + m_Pos.X + m_Offset.X) >= (int)WINSIZE_X ||
			(targetInfo.Pos_X + m_Pos.X - m_Offset.X) <= 0 )
		{
			temp.X = -(targetInfo.Pos_X - (int)((int)WINSIZE_X / 2));
		}

		if ((targetInfo.Pos_Y + m_Pos.Y + m_Offset.Y) >= (int)WINSIZE_Y ||
			(targetInfo.Pos_Y + m_Pos.Y - m_Offset.Y) <= 0 )
		{
			temp.Y = -(targetInfo.Pos_Y - (int)((int)WINSIZE_Y / 2));
		}

		m_Pos = temp;

		if (0 <= m_Pos.X) m_Pos.X = 0;
		if (0 <= m_Pos.Y) m_Pos.Y = 0;
		if (-m_Resolution.X > m_Pos.X - (int)WINSIZE_X) m_Pos.X = -(m_Resolution.X - (int)WINSIZE_X);
		if (-m_Resolution.Y > m_Pos.Y - (int)WINSIZE_Y) m_Pos.Y = -(m_Resolution.Y - (int)WINSIZE_Y);
	}

	return 0;
}

void CameraManager::Release()
{
	m_Target = nullptr;
}
