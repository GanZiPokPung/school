#include "stdafx.h"
#include "Tile.h"

Tile::Tile()
	:m_CamPos(GET_MANAGER<CameraManager>()->GetPos())
{
}

Tile::~Tile()
{
	Release();
}

bool Tile::Initialize()
{
	m_Info = GAMEOBJINFO{ 0, 0, 80, 80 };

	m_CamMgr = GET_MANAGER<CameraManager>();

	return true;
}

int Tile::Update(const float& TimeDelta)
{
	RECT LimitRect;
	
	LimitRect.left = 0 - m_CamPos.X - (m_Info.Size_Width / 2);
	LimitRect.right = (int)WINSIZE_X - m_CamPos.X + (m_Info.Size_Width / 2);
	LimitRect.top =  0 - m_CamPos.Y - (m_Info.Size_Height / 2);
	LimitRect.bottom = (int)WINSIZE_Y - m_CamPos.Y + (m_Info.Size_Height / 2);

	if (m_Info.Pos_X < LimitRect.left ||
		m_Info.Pos_X > LimitRect.right ||
		m_Info.Pos_Y < LimitRect.top ||
		m_Info.Pos_Y > LimitRect.bottom)
	{
		m_isRender = false;
		return 0;
	}

	m_isRender = true;

	if (-1 == GameObject::Update(TimeDelta))
	{
		return -1;
	}

	return 0;
}

void Tile::Render(HDC hdc)
{
	if (false == m_isRender)
		return;

	if (true == m_isWhite)
	{
		HDC hMemDC_white = GET_MANAGER<BmpManager>()->FindBmp(L"white")->GetMemDC();
		BitBlt(hdc, m_Rect.left, m_Rect.top, m_Info.Size_Width, m_Info.Size_Height
			, hMemDC_white, 0, 0, SRCCOPY);
	}
	else
	{
		HDC hMemDC_brown = GET_MANAGER<BmpManager>()->FindBmp(L"brown")->GetMemDC();
		BitBlt(hdc, m_Rect.left, m_Rect.top, m_Info.Size_Width, m_Info.Size_Height
			, hMemDC_brown, 0, 0, SRCCOPY);
	}
}

void Tile::Release()
{
}
