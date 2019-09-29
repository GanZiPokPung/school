#include "stdafx.h"
#include "ChessManager.h"


ChessManager::ChessManager()
{
	ZeroMemory(&m_ChessArr, sizeof(ChessBoardInfo) * (BOARDSIZE * BOARDSIZE));
}

ChessManager::~ChessManager()
{
}

const bool ChessManager::GetIndexByPos(int * OutputIndexX, int * OutputIndexY, const int & PosX, const int & PosY)
{
	// ������ ������ �������� ü������ �ε��� ������ �޾ƿ´�.
	// ü���� Ÿ�� �����ȿ� ������ �� ��ġ�� ���� �޾ƿ´�.

	for (int i = 0; i < BOARDSIZE; ++i)
	{
		for (int j = 0; j < BOARDSIZE; ++j)
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

void ChessManager::Initialize(const int& BoardPosX, const int& BoardPosY, const int& BoardWidth, const int& BoardHeight)
{
	// ü������ ���� �Ʒ����� 1, 1�� �����Ͽ�
	// ������ ���� 8, 8�̴�.
	// ü������ ��ġ�� ����� ���� ü������ ������ �ʱ�ȭ�Ѵ�.

	int sizeWidth = BoardWidth / BOARDSIZE;
	int sizeHeight = BoardHeight / BOARDSIZE;

	int initPosX = BoardPosX - (BoardWidth / 2) + (sizeWidth / 2);
	int initPosY = BoardPosY + (BoardHeight / 2) - (sizeHeight / 2);
	

	for (int i = 0; i < BOARDSIZE; ++i)
	{
		for (int j = 0; j < BOARDSIZE; ++j)
		{
			m_ChessArr[i][j].TileSize_Width = sizeWidth;
			m_ChessArr[i][j].TileSize_Height = sizeHeight;
			m_ChessArr[i][j].Pos_X = initPosX + (sizeWidth * (j));
			m_ChessArr[i][j].Pos_Y = initPosY - (sizeHeight * (i));
		}
	}
}
