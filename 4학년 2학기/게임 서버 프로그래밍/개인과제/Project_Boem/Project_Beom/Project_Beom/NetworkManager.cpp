#include "stdafx.h"
#include "NetworkManager.h"

NetworkManager::NetworkManager()
{
}

NetworkManager::~NetworkManager()
{
	Release();
}

bool NetworkManager::ConnectToServer(const char* ip)
{
	ZeroMemory(&m_serverAddr, sizeof(m_serverAddr));
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, ip, &m_serverAddr.sin_addr);

	int result = connect(m_serverSocket, (struct sockaddr*) & m_serverAddr, sizeof(m_serverAddr));

	if (SOCKET_ERROR == result)
	{
		MessageBox(g_NewhWnd, L"접속에 실패하였습니다.\n IP주소나 서버 상태를 확인하여 다시 접속해주세요.", L"Error", MB_OK);
		return false;
	}

	MessageBox(g_NewhWnd, L"서버에 접속하였습니다!", L"알림", MB_OK);
	return true;
}

int NetworkManager::SendAndReceiveByID(ClassPacket<MOVEINFO>* recv_data)
{
	ClassPacket<MOVEINFO> packet;
	ZeroMemory(&packet, sizeof(ClassPacket<MOVEINFO>));

	packet.GetMustInfo().netType = NETTYPE_MINE;
	packet.GetMustInfo().id = 0;

	packet.MakeDataToPacket(m_infoBuffer);

	int bufferLen = sizeof(ClassPacket<MOVEINFO>);
	int sendBytes = send(m_serverSocket, m_infoBuffer, bufferLen, 0);
	if (sendBytes > 0)
	{
		// 받는 작업
		int receiveBytes = recv(m_serverSocket, m_infoBuffer, MAX_BUFFER, 0);

		if (receiveBytes > 0)
		{
			recv_data->MakePacketToData(m_infoBuffer);
			return 1;
		}
		else if (SOCKET_ERROR == receiveBytes)
		{
			return -1;
		}
	}
	else if (SOCKET_ERROR == sendBytes)
	{
		return -1;
	}

	return 0;
}

int NetworkManager::SendAndReceiveByInput(ClassPacket<INPUTINFO>* recv_data, const int& Id, const int& IndexX, const int& IndexY, const int& keyValue)
{
	ClassPacket<INPUTINFO> packet;
	ZeroMemory(&packet, sizeof(ClassPacket<INPUTINFO>));

	packet.GetMustInfo().netType = NETTYPE_INPUT;
	packet.GetMustInfo().id = Id;
	packet.GetMustInfo().indexX = IndexX;
	packet.GetMustInfo().indexY = IndexY;

	INPUTINFO& info = packet.GetInfo();

	info.inputType = keyValue;
	info.canMove = 'X';

	packet.MakeDataToPacket(m_infoBuffer);

	int bufferLen = sizeof(ClassPacket<INPUTINFO>);
	int sendBytes = send(m_serverSocket, m_infoBuffer, bufferLen, 0);
	if (sendBytes > 0)
	{
		// 받는 작업
		int receiveBytes = recv(m_serverSocket, m_infoBuffer, MAX_BUFFER, 0);
		
		if (receiveBytes > 0)
		{
			recv_data->MakePacketToData(m_infoBuffer);
			return 1;
		}
		else if (SOCKET_ERROR == receiveBytes)
		{
			return -1;
		}
	}
	else if(SOCKET_ERROR == sendBytes)
	{
		return -1;
	}

	return 0;
}

int NetworkManager::SendAndReceiveByCanMove(ClassPacket<MOVEINFO>* recv_data, const int& IndexX, const int& IndexY)
{
	ClassPacket<MOVEINFO> packet;
	ZeroMemory(&packet, sizeof(ClassPacket<MOVEINFO>));

	packet.GetMustInfo().netType = NETTYPE_CANMOVE;
	packet.GetMustInfo().id = 0;
	packet.GetMustInfo().indexX = IndexX;
	packet.GetMustInfo().indexY = IndexY;

	MOVEINFO& info = packet.GetInfo();

	info.canMove = 'X';
	info.others_id = 0;

	packet.MakeDataToPacket(m_infoBuffer);

	int bufferLen = sizeof(ClassPacket<MOVEINFO>);
	int sendBytes = send(m_serverSocket, m_infoBuffer, bufferLen, 0);
	if (sendBytes > 0)
	{
		// 받는 작업
		int receiveBytes = recv(m_serverSocket, m_infoBuffer, MAX_BUFFER, 0);

		if (receiveBytes > 0)
		{
			recv_data->MakePacketToData(m_infoBuffer);
			return 1;
		}
		else if (SOCKET_ERROR == receiveBytes)
		{
			return -1;
		}
	}
	else if (SOCKET_ERROR == sendBytes)
	{
		return -1;
	}

	return 0;
}

int NetworkManager::SendByAdjustMove(const int& Id, const int& IndexX, const int& IndexY)
{
	ClassPacket<MOVEINFO> packet;
	ZeroMemory(&packet, sizeof(ClassPacket<MOVEINFO>));

	packet.GetMustInfo().netType = NETTYPE_ADJMOVE;
	packet.GetMustInfo().id = Id;
	packet.GetMustInfo().indexX = IndexX;
	packet.GetMustInfo().indexY = IndexY;

	MOVEINFO& info = packet.GetInfo();

	info.canMove = 'X';
	info.others_id = 0;

	packet.MakeDataToPacket(m_infoBuffer);

	int bufferLen = sizeof(ClassPacket<MOVEINFO>);
	int sendBytes = send(m_serverSocket, m_infoBuffer, bufferLen, 0);
	if (sendBytes > 0)
	{
		// 받는 작업
		int receiveBytes = recv(m_serverSocket, m_infoBuffer, MAX_BUFFER, 0);

		if (receiveBytes > 0)
		{
			return 1;
		}
		else if (SOCKET_ERROR == receiveBytes)
		{
			return -1;
		}
	}
	else if (SOCKET_ERROR == sendBytes)
	{
		return -1;
	}

	return 0;
}

int NetworkManager::SendBySetIndex(const int& Id, const int& NewIndexX, const int& NewIndexY, const int& OldIndexX, const int& OldIndexY)
{
	ClassPacket<INDEXINFO> packet;
	ZeroMemory(&packet, sizeof(ClassPacket<INDEXINFO>));

	packet.GetMustInfo().netType = NETTYPE_ADJMOVE;
	packet.GetMustInfo().id = Id;
	packet.GetMustInfo().indexX = OldIndexX;
	packet.GetMustInfo().indexY = OldIndexY;

	INDEXINFO& info = packet.GetInfo();

	info.newIndexX = NewIndexX;
	info.newIndexY = NewIndexY;
	info.oldIndexX = OldIndexX;
	info.oldIndexY = OldIndexY;

	packet.MakeDataToPacket(m_infoBuffer);

	int bufferLen = sizeof(ClassPacket<INDEXINFO>);
	int sendBytes = send(m_serverSocket, m_infoBuffer, bufferLen, 0);
	if (sendBytes > 0)
	{
		// 받는 작업
		int receiveBytes = recv(m_serverSocket, m_infoBuffer, MAX_BUFFER, 0);

		if (receiveBytes > 0)
		{
			return 1;
		}
		else if (SOCKET_ERROR == receiveBytes)
		{
			return -1;
		}
	}
	else if (SOCKET_ERROR == sendBytes)
	{
		return -1;
	}

	return 0;
}

int NetworkManager::AlwaysSendAndReceiveByServerInfo(ClassPacket<OTHERINFO>* recv_data)
{
	ClassPacket<OTHERINFO> packet;
	ZeroMemory(&packet, sizeof(ClassPacket<OTHERINFO>));

	packet.GetMustInfo().netType = NETTYPE_OTHER;
	packet.GetMustInfo().id = 0;
	packet.GetMustInfo().indexX = 1;
	packet.GetMustInfo().indexY = 1;

	OTHERINFO& info = packet.GetInfo();

	info.type = CHANGE_END;
	info.others_indexX = 1;
	info.others_indexY = 1;
	info.others_id = 0;

	packet.MakeDataToPacket(m_infoBuffer);

	int bufferLen = sizeof(ClassPacket<OTHERINFO>);
	int sendBytes = send(m_serverSocket, m_infoBuffer, bufferLen, 0);
	if (sendBytes > 0)
	{
		// 받는 작업
		int receiveBytes = recv(m_serverSocket, m_infoBuffer, MAX_BUFFER, 0);

		if (receiveBytes > 0)
		{
			recv_data->MakePacketToData(m_infoBuffer);
			return (int)recv_data->GetInfo().type;
		}
		else if (SOCKET_ERROR == receiveBytes)
		{
			return -1;
		}
	}
	else if (SOCKET_ERROR == sendBytes)
	{
		return -1;
	}

	return 0;
}

bool NetworkManager::Initialize()
{
	// 윈속 초기화
	if (0 != WSAStartup(MAKEWORD(2, 2), &m_WSAData))
		return false;

	// server socket을 만든다.
	m_serverSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
	if (INVALID_SOCKET == m_serverSocket)
		return false;

	return true;
}

void NetworkManager::Release()
{
	closesocket(m_serverSocket);
	WSACleanup();
}
