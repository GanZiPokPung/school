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
	// ���� ������ ����
	WSADATA m_WSAData;
	// ���ӿ� ����
	SOCKET m_listenSocket;
	// ��ſ� ���� 
	SOCKET m_clientSocket;

	// ���� 
	SOCKADDR_IN m_serverAddr;
	// Ŭ���̾�Ʈ �����ּ�
	SOCKADDR_IN m_clientAddr;

	ChessManager* m_chessManager;

	char m_infoBuffer[MAX_BUFFER];
	// ���ŵ� �ε���
	int m_newIndexX, m_newIndexY;
	// ������ �� �ִ����� ���� ����
	bool m_canMove = false;
	// Ŭ���̾�Ʈ�� ���� ������ �ް� �������� ���� ����
	bool m_isSend = false;
};

