#include "ChessManager.h"

ChessManager::ChessManager()
{
}

ChessManager::~ChessManager()
{
}

const bool ChessManager::CanMove(const int& IndexX, const int& IndexY)
{
	// ü���� ���� �ٱ�
	if (0 >= IndexX ||
		0 >= IndexY ||
		BOARDSIZE < IndexX ||
		BOARDSIZE < IndexY)
	{
		return false;
	}

	// �̹� ü������ ����
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
		// ���� ������ �� �ִٸ�
		// ü���� �ε����� �����ϴ����� ���� ���θ� ���ŵ� ��ġ�� �ε����� �����Ѵ�.
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
