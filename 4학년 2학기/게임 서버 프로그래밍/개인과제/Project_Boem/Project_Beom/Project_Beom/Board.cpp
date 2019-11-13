#include "stdafx.h"
#include "Board.h"

#include "Tile.h"

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
			GameObject *tile = AbstractFactory<Tile>::CreateObj(
				(j * tileWidth) + (tileWidth / 2),
				(i * tileHeight) + (tileHeight / 2));

			int r = rand() % 2;
			if (0 == r)
				dynamic_cast<Tile*>(tile)->SetWhite(true);
			else
				dynamic_cast<Tile*>(tile)->SetWhite(false);

			m_vecTiles.emplace_back(tile);
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

	for (const auto& tile : m_vecTiles)
	{
		tile->Update(TimeDelta);
	}

	return 0;
}

void Board::Render(HDC hdc)
{
	HDC hMemDC_white = GET_MANAGER<BmpManager>()->FindBmp(L"white")->GetMemDC();
	HDC hMemDC_brown = GET_MANAGER<BmpManager>()->FindBmp(L"brown")->GetMemDC();

	for (const auto& tile : m_vecTiles)
	{
		tile->Render(hdc);
	}

	/*TransparentBlt(hdc, m_Rect.left, m_Rect.top, m_Info.Size_Width, m_Info.Size_Height
		, hMemDC, 0, 0, m_Info.Size_Width, m_Info.Size_Height, RGB(255, 255, 255));*/
}

void Board::Release()
{
	for (auto& tile : m_vecTiles)
	{
		if (nullptr != tile)
		{
			delete tile;
			tile = nullptr;
		}
	}

	m_vecTiles.clear();
}
