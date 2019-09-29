#pragma once
#include "Includes.h"

class MainServer
{
public:
	MainServer();
	~MainServer();

public:
	bool Initialize();
	int Running();
	void Release();

private:
	int ReceiveProcess();
	int SendProcess();

private:
	// 윈속 데이터 변수
	WSADATA m_WSAData;
	// 접속용 소켓
	SOCKET m_listenSocket;
	// 통신용 소켓 
	SOCKET m_clientSocket;

	// 서버 
	SOCKADDR_IN m_serverAddr;
	// 클라이언트 소켓주소
	SOCKADDR_IN m_clientAddr;

	ChessManager* m_chessManager;

	char m_infoBuffer[MAX_BUFFER];
	// 갱신된 인덱스
	int m_newIndexX, m_newIndexY;
	// 움직일 수 있는지에 대한 여부
	bool m_canMove = false;
	// 클라이언트로 부터 정보를 받고 보낼지에 대한 여부
	bool m_isSend = false;
};

