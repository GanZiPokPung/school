#include "ChessManager.h"

ChessManager::ChessManager()
{
}

ChessManager::~ChessManager()
{
}

const bool ChessManager::CanMove(const int& IndexX, const int& IndexY)
{
	// 체스판 범위 바깥
	if (0 >= IndexX ||
		0 >= IndexY ||
		BOARDSIZE < IndexX ||
		BOARDSIZE < IndexY)
	{
		return false;
	}

	// 이미 체스말이 있음
	if (true == m_ChessArr[IndexY - 1][IndexX - 1].IsExist)
		return false;

	return true;
}

void ChessManager::SetChessExist(const int& IndexX, const int& IndexY, const bool& check)
{
	m_ChessArr[IndexY - 1][IndexX - 1].IsExist = check;
}

const bool ChessManager::ProcessKeyInput(int* OutputIndexX, int* OutputIndexY, const int& IndexX, const int& IndexY, const char& keyValue)
{
	*OutputIndexX = IndexX;
	*OutputIndexY = IndexY;

	switch (keyValue)
	{
	case VK_UP:
		*OutputIndexY += 1;
		break;
	case VK_DOWN:
		*OutputIndexY -= 1;
		break;
	case VK_LEFT:
		*OutputIndexX -= 1;
		break;
	case VK_RIGHT:
		*OutputIndexX += 1;
		break;
	default:
		break;
	}

	if (false == CanMove(*OutputIndexX, *OutputIndexY))
	{
		return false;
	}
	else
	{
		// 만약 움직일 수 있다면
		// 체스판 인덱스에 존재하는지에 대한 여부를 갱신된 위치의 인덱스로 변경한다.
		m_ChessArr[*OutputIndexY - 1][*OutputIndexX - 1].IsExist = true;
		m_ChessArr[IndexY - 1][IndexX - 1].IsExist = false;
		
	}

	return true;
}

void ChessManager::Initialize()
{
	for (int i = 0; i < BOARDSIZE; ++i)
	{
		for (int j = 0; j < BOARDSIZE; ++j)
		{
			m_ChessArr[i][j].IsExist = false;
		}
	}
}
