#include "stdafx.h"
#include "ChessManager.h"


ChessManager::ChessManager()
{
	ZeroMemory(&m_ChessArr, sizeof(ChessBoardInfo) * (BOARDSIZE_X * BOARDSIZE_Y));
}

ChessManager::~ChessManager()
{
}

const bool ChessManager::GetIndexByPos(int * OutputIndexX, int * OutputIndexY, const int & PosX, const int & PosY)
{
	// ������ ������ �������� ü������ �ε��� ������ �޾ƿ´�.
	// ü���� Ÿ�� �����ȿ� ������ �� ��ġ�� ���� �޾ƿ´�.

	for (int i = 0; i < BOARDSIZE_Y; ++i)
	{
		for (int j = 0; j < BOARDSIZE_X; ++j)
		{
			BOARDINFO TileInfo = m_ChessArr[i][j];

			int TileMinX = TileInfo.Pos_X - TileInfo.TileSize_Width / 2;
			int TileMaxX = TileInfo.Pos_X + TileInfo.TileSize_Width / 2;

			int TileMinY = TileInfo.Pos_Y - TileInfo.TileSize_Height / 2;
			int TileMaxY = TileInfo.Pos_Y + TileInfo.TileSize_Height / 2;
			
			if ((TileMinX <= PosX && TileMaxX >= PosX) &&
				(TileMinY <= PosY && TileMaxY >= PosY))
			{
				*OutputIndexX = j + 1;
				*OutputIndexY = i + 1;
				return true;
			}
		}
	}

	return false;
}

const bool ChessManager::GetPosByIndex(int * OutputPosX, int * OutputPosY, const int & IndexX, const int & IndexY)
{
	// �ε��� ������ �������� ü������ ��ġ ������ �޾ƿ´�.
	*OutputPosX = m_ChessArr[IndexY - 1][IndexX - 1].Pos_X;
	*OutputPosY = m_ChessArr[IndexY - 1][IndexX - 1].Pos_Y;

	return true;
}

void ChessManager::Initialize(const int& TileWidth, const int& TileHeight)
{
	// ü������ ��ġ�� ����� ���� ü������ ������ �ʱ�ȭ�Ѵ�.
	for (int i = 0; i < BOARDSIZE_Y; ++i)
	{
		for (int j = 0; j < BOARDSIZE_X; ++j)
		{
			m_ChessArr[i][j].TileSize_Width = TileWidth;
			m_ChessArr[i][j].TileSize_Height = TileHeight;
			m_ChessArr[i][j].Pos_X = (j * TileWidth) + (TileWidth / 2);
			m_ChessArr[i][j].Pos_Y = (i * TileHeight) + (TileHeight / 2);
		}
	}
}
