#pragma once

typedef struct GameObjectInformation
{
	int Pos_X;
	int Pos_Y;
	int Size_Width;
	int Size_Height;
}GAMEOBJINFO;

typedef struct ChessBoardInfo
{
	int Pos_X;
	int Pos_Y;
	int TileSize_Width;
	int TileSize_Height;
}BOARDINFO;

typedef struct ChessTileInfo
{
	bool isWhite;
	GAMEOBJINFO objInfo;
}TILEINFO;

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

typedef struct OtherInfo
{
	CHANGETYPE type;
	int others_id;
	int others_indexX;
	int others_indexY;
}OTHERINFO;