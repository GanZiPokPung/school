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
		MessageBox(g_NewhWnd, L"���ӿ� �����Ͽ����ϴ�.\n IP�ּҳ� ���� ���¸� Ȯ���Ͽ� �ٽ� �������ּ���.", L"Error", MB_OK);
		return false;
	}
	
	MessageBox(g_NewhWnd, L"������ �����Ͽ����ϴ�!", L"�˸�", MB_OK);
	return true;
}

int NetworkManager::SendAndReceiveByInput(const int& IndexX, const int& IndexY, const char& keyValue)
{
	m_infoBuffer[TYPE_NETWORKTYPE] = (char)NETTYPE_INPUT;
	m_infoBuffer[TYPE_INPUT] = keyValue;
	m_infoBuffer[TYPE_INDEX_X] = (char)IndexX;
	m_infoBuffer[TYPE_INDEX_Y] = (char)IndexY;
	m_infoBuffer[TYPE_CANMOVE] = 'X';
	m_infoBuffer[TYPE_INFO_END] = '\0';

	int bufferLen = (int)strlen(m_infoBuffer);
	int sendBytes = send(m_serverSocket, m_infoBuffer, bufferLen, 0);
	if (sendBytes > 0)
	{
		// �޴� �۾�
		int receiveBytes = recv(m_serverSocket, m_infoBuffer, MAX_BUFFER, 0);
		
		if (receiveBytes > 0)
		{
			m_infoBuffer[receiveBytes] = '\0';
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

int NetworkManager::SendAndReceiveByCanMove(const int& IndexX, const int& IndexY)
{
	m_infoBuffer[TYPE_NETWORKTYPE] = (char)NETTYPE_CANMOVE;
	m_infoBuffer[TYPE_INPUT] = 1;
	m_infoBuffer[TYPE_INDEX_X] = (char)IndexX;
	m_infoBuffer[TYPE_INDEX_Y] = (char)IndexY;
	m_infoBuffer[TYPE_CANMOVE] = 'X';
	m_infoBuffer[TYPE_INFO_END] = '\0';

	int bufferLen = (int)strlen(m_infoBuffer);
	int sendBytes = send(m_serverSocket, m_infoBuffer, bufferLen, 0);
	if (sendBytes > 0)
	{
		// �޴� �۾�
		int receiveBytes = recv(m_serverSocket, m_infoBuffer, MAX_BUFFER, 0);

		if (receiveBytes > 0)
		{
			m_infoBuffer[receiveBytes] = '\0';
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

int NetworkManager::SendByAdjustMove(const int& IndexX, const int& IndexY)
{
	m_infoBuffer[TYPE_NETWORKTYPE] = (char)NETTYPE_ADJMOVE;
	m_infoBuffer[TYPE_INPUT] = 1;
	m_infoBuffer[TYPE_INDEX_X] = (char)IndexX;
	m_infoBuffer[TYPE_INDEX_Y] = (char)IndexY;
	m_infoBuffer[TYPE_CANMOVE] = 'X';
	m_infoBuffer[TYPE_INFO_END] = '\0';

	int bufferLen = (int)strlen(m_infoBuffer);
	int sendBytes = send(m_serverSocket, m_infoBuffer, bufferLen, 0);
	if (sendBytes > 0)
	{
		return 1;
	}
	else if (SOCKET_ERROR == sendBytes)
	{
		return -1;
	}

	return 0;
}

bool NetworkManager::Initialize()
{
	// ���� �ʱ�ȭ
	if (0 != WSAStartup(MAKEWORD(2, 0), &m_WSAData))
		return false;

	// server socket�� �����.
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
