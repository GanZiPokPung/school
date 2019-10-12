#pragma once

class NetworkManager : public Singleton<NetworkManager>
{
public:
	NetworkManager();
	virtual ~NetworkManager();

public:
	bool ConnectToServer(const char* ip);
	int SendAndReceiveByID(ClassPacket<MOVEINFO>* recv_data);
	int SendAndReceiveByInput(ClassPacket<INPUTINFO>* recv_data, 
		const int& Id, const int& IndexX, const int& IndexY, const int& keyValue);
	int SendAndReceiveByCanMove(ClassPacket<MOVEINFO>* recv_data, 
		const int& IndexX, const int& IndexY);
	int SendByAdjustMove(const int& Id, const int& IndexX, const int& IndexY);
	int SendBySetIndex(const int& Id, const int& NewIndexX, const int& NewIndexY,
								const int& OldIndexX, const int& OldIndexY);
	
	int AlwaysSendAndReceiveByServerInfo(ClassPacket<OTHERINFO>* recv_data);

public:
	bool Initialize();
	void Release();


private:
	WSADATA m_WSAData;
	SOCKET m_serverSocket;
	sockaddr_in m_serverAddr;

	char m_infoBuffer[MAX_BUFFER];
};

