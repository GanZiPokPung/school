#pragma once
#include "pch.h"

// Chess Manager에서 플레이어들을 관리 해줘야 할 것이다..
// 추가 필요

class ChessManager : public Singleton<ChessManager>
{
public:
	ChessManager();
	virtual ~ChessManager();

public:
	const bool ProcessKeyInput(int* OutputIndexX, int* OutputIndexY, const int& IndexX, const int& IndexY, const char& keyValue, const int& Id);
	const bool CanMove(int* OutputId, const int& IndexX, const int& IndexY);
	void SetChessExist(const int& IndexX, const int& IndexY, const bool& check, const int& Id);
public:
	void Initialize();

private:
	BOARDINFO m_ChessArr[BOARDSIZE][BOARDSIZE];
};

