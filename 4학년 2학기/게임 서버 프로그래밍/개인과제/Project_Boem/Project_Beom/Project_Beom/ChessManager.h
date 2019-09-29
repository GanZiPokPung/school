#pragma once
class ChessManager : public Singleton<ChessManager>
{
public:
	ChessManager();
	virtual ~ChessManager();

public:
	const bool GetIndexByPos(int* OutputIndexX, int *OutputIndexY, const int& PosX, const int& PosY);
	const bool GetPosByIndex(int* OutputPosX, int *OutputPosY, const int& IndexX, const int& IndexY);

public:
	void Initialize(const int& BoardPosX, const int& BoardPosY, const int& BoardWidth, const int& BoardHeight);

private:
	BOARDINFO m_ChessArr[BOARDSIZE][BOARDSIZE];
};

