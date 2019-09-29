#pragma once

class NetworkManager : public Singleton<NetworkManager>
{
public:
	NetworkManager();
	virtual ~NetworkManager();

public:
	const char* GetInfoBuffer() { return m_infoBuffer; }

public:
	bool ConnectToServer(const char* ip);
	int SendAndReceiveByInput(const int& IndexX, const int& IndexY, const char& keyValue);
	int SendAndReceiveByCanMove(const int& IndexX, const int& IndexY);
	int SendByAdjustMove(const int& IndexX, const int& IndexY);
public:
	bool Initialize();
	void Release();

private:
	WSADATA m_WSAData;
	SOCKET m_serverSocket;
	sockaddr_in m_serverAddr;

	char m_infoBuffer[MAX_BUFFER];
};

