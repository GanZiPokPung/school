#pragma once
#include "Includes.h"

class ChessManager : public Singleton<ChessManager>
{
public:
	ChessManager();
	virtual ~ChessManager();

public:
	const bool ProcessKeyInput(int* OutputIndexX, int* OutputIndexY, const int& IndexX, const int& IndexY, const char& keyValue);
	const bool CanMove(const int& IndexX, const int& IndexY);
	void SetChessExist(const int& IndexX, const int& IndexY, const bool& check);

public:
	void Initialize();

private:
	BOARDINFO m_ChessArr[BOARDSIZE][BOARDSIZE];
};

