#include "stdafx.h"
#include "Board.h"


Board::Board()
{
}


Board::~Board()
{
	Release();
}

bool Board::Initialize()
{
	m_Info = GAMEOBJINFO{ 512, 512, 800, 800 };

	return true;
}

int Board::Update(const float & TimeDelta)
{
	if (-1 == GameObject::Update(TimeDelta))
	{
		return -1;
	}

	return 0;
}

void Board::Render(HDC hdc)
{
	HDC hMemDC = GET_MANAGER<BmpManager>()->FindBmp(L"board")->GetMemDC();

	TransparentBlt(hdc, m_Rect.left, m_Rect.top, m_Info.Size_Width, m_Info.Size_Height
		, hMemDC, 0, 0, m_Info.Size_Width, m_Info.Size_Height, RGB(255, 255, 255));
}

void Board::Release()
{
}
