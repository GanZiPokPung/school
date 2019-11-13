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
public:
	static int ReceiveProcess(char *buf, bool& isSend, const DWORD& receivedByte, const SOCKET& clientsocket);
	static int SendProcess(char* buf, DWORD& sendByte, const bool& isSend, const SOCKET& clientsocket);
	static int EndProcess(const SOCKET& clientsocket);

	// 콜백 함수
private:
	static void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD InFlags);
	static void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD InFlags);

public :
	static map<SOCKET, SOCKETINFO> m_mapClients;
	static int m_userCount;

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

};

