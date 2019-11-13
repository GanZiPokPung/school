#pragma once

typedef struct ChessBoardInfo
{
	bool IsExist;
	int  Id;
}BOARDINFO;

typedef struct OtherInfo
{
	CHANGETYPE type;
	int others_id;
	int others_indexX;
	int others_indexY;
}OTHERINFO;

typedef struct SocketInfo
{
	WSAOVERLAPPED overlapped;
	WSABUF dataBuffer;
	char messageBuffer[MAX_BUFFER];
	bool is_recv;
	SOCKET socket;
	SOCKADDR_IN clientAddr;
	queue<OTHERINFO> otherInfo;
	POINT curIndex;
}SOCKETINFO;

typedef struct MustInfo
{
	PACKETTYPE netType;
	int indexX, indexY;
	int id;
}MUSTINFO;

typedef struct MoveInfo
{
	int others_id;
	char canMove;
}MOVEINFO;

typedef struct InputInfo
{
	int inputType;
	char canMove;
}INPUTINFO;

typedef struct SetIndexInfo
{
	int oldIndexX, oldIndexY;
	int newIndexX, newIndexY;
}INDEXINFO;



