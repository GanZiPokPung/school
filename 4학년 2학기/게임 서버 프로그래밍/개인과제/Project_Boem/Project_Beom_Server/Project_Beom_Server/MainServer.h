#pragma once

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
	static int ReceiveProcess(char *buf, bool& isSend, const DWORD& receivedByte, const SOCKET& clientsocket);
	static int SendProcess(char* buf, DWORD& sendByte, const bool& isSend, const SOCKET& clientsocket);
	static int EndProcess(const SOCKET& clientsocket);

	// �ݹ� �Լ�
private:
	static void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD InFlags);
	static void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD InFlags);

private:
	static map<SOCKET, SOCKETINFO> m_mapClients;
	static int m_userCount;

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


};

