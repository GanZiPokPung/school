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
	m_Info = GAMEOBJINFO{ 0, 0, 0, 0 };

	int tileWidth = 80;
	int tileHeight = 80;

	for (int i = 0; i < (int)BOARDSIZE_Y; ++i)
	{
		int count = 0;
		for (int j = 0; j < (int)BOARDSIZE_X; ++j)
		{
			TILEINFO info;
			info.objInfo.Pos_X = (j * tileWidth) + (tileWidth / 2);
			info.objInfo.Pos_Y = (i * tileHeight) + (tileHeight / 2);
			info.objInfo.Size_Width = tileWidth;
			info.objInfo.Size_Height = tileHeight;

			int r = rand() % 2;
			if (0 == r)
				info.isWhite = true;
			else
				info.isWhite = false;

			m_vecTiles.emplace_back(info);
		}
	}

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
	HDC hMemDC_white = GET_MANAGER<BmpManager>()->FindBmp(L"white")->GetMemDC();
	HDC hMemDC_brown = GET_MANAGER<BmpManager>()->FindBmp(L"brown")->GetMemDC();

	for (const auto& tile : m_vecTiles)
	{
		RECT rc = RECT{ 0, 0, 0, 0 };
		rc.left = tile.objInfo.Pos_X - tile.objInfo.Size_Width / 2;
		rc.top = tile.objInfo.Pos_Y - tile.objInfo.Size_Height / 2;
		rc.right = tile.objInfo.Pos_X + tile.objInfo.Size_Width / 2;
		rc.bottom = tile.objInfo.Pos_Y + tile.objInfo.Size_Height / 2;

		if (true == tile.isWhite)
		{
			BitBlt(hdc, rc.left, rc.top, tile.objInfo.Size_Width, tile.objInfo.Size_Height
				, hMemDC_white, 0, 0, NULL);
		}
		else
		{
			BitBlt(hdc, rc.left, rc.top, tile.objInfo.Size_Width, tile.objInfo.Size_Height
				, hMemDC_brown, 0, 0, NULL);
		}
	}

	/*TransparentBlt(hdc, m_Rect.left, m_Rect.top, m_Info.Size_Width, m_Info.Size_Height
		, hMemDC, 0, 0, m_Info.Size_Width, m_Info.Size_Height, RGB(255, 255, 255));*/
}

void Board::Release()
{
	m_vecTiles.clear();
}
