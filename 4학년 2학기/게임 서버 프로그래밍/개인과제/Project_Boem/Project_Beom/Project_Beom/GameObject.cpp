#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject()
{
	ZeroMemory(&m_Info, sizeof(GAMEOBJINFO));
	ZeroMemory(&m_Rect, sizeof(RECT));
}


GameObject::~GameObject()
{
}

void GameObject::SetPosition(int posX, int posY)
{
	m_Info.Pos_X = posX;
	m_Info.Pos_Y = posY;
}

void GameObject::SetSize(int sizeWidth, int sizeHeight)
{
	m_Info.Size_Width = sizeWidth;
	m_Info.Size_Height = sizeHeight;
}

void GameObject::SetAngle(float angle)
{
	m_Angle = angle;
}

int GameObject::Update(const float & TimeDelta)
{
	// 오브젝트 정보를 바탕으로 Rect 정보를 갱신한다.
	POSITION cam = GET_MANAGER<CameraManager>()->GetPos();

	if (false == m_ScrollCheck)
	{
		cam.X = 0;
		cam.Y = 0;
	}

	m_Rect.left = m_Info.Pos_X + cam.X - m_Info.Size_Width / 2;
	m_Rect.top = m_Info.Pos_Y + cam.Y - m_Info.Size_Height / 2;
	m_Rect.right = m_Info.Pos_X + cam.X + m_Info.Size_Width / 2;
	m_Rect.bottom = m_Info.Pos_Y + cam.Y + m_Info.Size_Height / 2;

	return 0;
}
