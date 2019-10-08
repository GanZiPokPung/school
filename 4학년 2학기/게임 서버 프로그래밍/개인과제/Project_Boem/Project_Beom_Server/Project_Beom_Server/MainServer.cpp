#include "MainServer.h"

MainServer::MainServer()
{
}

MainServer::~MainServer()
{
	Release();
}

bool MainServer::Initialize()
{
	// ���� �ʱ�ȭ
	if (0 != WSAStartup(MAKEWORD(2, 0), &m_WSAData))
		return false;

	cout << "WinSock �ʱ�ȭ ����!" << endl;

	// ���� ����
	m_listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
	if (INVALID_SOCKET == m_listenSocket)
		return false;

	cout << "listen socket ���� ����!" << endl;

	// �ּ� ����ü 
	ZeroMemory(&m_serverAddr, sizeof(m_serverAddr));
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons(SERVER_PORT);
 	m_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// listen������ ���� IP �ּҿ� ���� ��Ʈ ��ȣ�� ����
	if (SOCKET_ERROR == ::bind(m_listenSocket, (sockaddr*)& m_serverAddr, sizeof(m_serverAddr)))
		return false;

	cout << "listen socket bind() ����!" << endl;

	// ������ ���¸� listen ���·� �����.
	if (SOCKET_ERROR == listen(m_listenSocket, SOMAXCONN))
		return false;

	cout << "listen socket ���� listen���� ���� ����!" << endl;
	cout << "���� ���� �ʱ�ȭ ����!" << endl << endl;


	m_chessManager = GET_MANAGER<ChessManager>();
	m_chessManager->Initialize();

	return true;
}

int MainServer::Running()
{
	cout << "������ Ŭ���̾�Ʈ�� connect()�� ��ٸ��ϴ�..." << endl;

	while (true)
	{
		int addr_size = sizeof(m_clientAddr);
		m_clientSocket = accept(m_listenSocket, (sockaddr*)& m_clientAddr, &addr_size);

		cout << "Ŭ���̾�Ʈ�� �����Ͽ����ϴ�." << endl;
		cout << "IP : " << inet_ntoa(m_clientAddr.sin_addr)
			<< " Port��ȣ : " << ntohs(m_clientAddr.sin_port) << endl;

		
		while (true)
		{
			int result;

			result = ReceiveProcess();
			// ���� ó��
			if (-1 == result)
				break;

			result = SendProcess();
			// ���� ó��
			if (-1 == result)
				break;
			else if (1 == result)
				continue;		
		}
		closesocket(m_clientSocket);
		cout << "Ŭ���̾�Ʈ�� ����Ǿ����ϴ�." << endl;
		cout << "IP : " << inet_ntoa(m_clientAddr.sin_addr)
			<< " Port��ȣ : " << ntohs(m_clientAddr.sin_port) << endl;
	}

	return 0;
}

void MainServer::Release()
{
	closesocket(m_listenSocket);
	WSACleanup();
}

int MainServer::ReceiveProcess()
{
	const char* NetTypes[] = { "Type_CanMove", "Type_AdjustMove", "Type_Input" };
	const char* InputTypes[] = { "VK_LEFT", "VK_UP", "VK_RIGHT", "VK_DOWN" };

	// �޾ƿ�
	int receiveBytes = recv(m_clientSocket, m_infoBuffer, MAX_BUFFER, 0);
	if (receiveBytes > 0)
	{
		// ���μ��� ó��
		m_infoBuffer[receiveBytes] = '\0';

		// ��Ʈ��ũ ������� ó���� Ÿ���� ���� �޴´�.
		NETWORKTYPE type = (NETWORKTYPE)m_infoBuffer[TYPE_NETWORKTYPE];

		if (type >= NETTYPE_END)
			return -1;

		cout << "Receive 1. NetworkType : " << NetTypes[type - 1];

		int indexX = (int)m_infoBuffer[TYPE_INDEX_X];
		int indexY = (int)m_infoBuffer[TYPE_INDEX_Y];

		// Ÿ�Ժ��� �۾��� �����Ѵ�.
		switch (type)
		{
			// ������ �� �ִ°��� ���� ������ ó��
		case NETTYPE_CANMOVE:
		{
			cout << " 2. Index : [" << indexX << ", " << indexY << "] "
				<< "(" << receiveBytes << " bytes)" << endl;

			m_canMove = m_chessManager->CanMove(indexX, indexY);

			m_isSend = true;
		}
		break;
		// �ε����� ���� ���翩�θ� �����ϴ� ó��
		case NETTYPE_ADJMOVE:
		{
			cout << " 2. Index : [" << indexX << ", " << indexY << "] "
				<< "(" << receiveBytes << " bytes)" << endl << endl;

			m_chessManager->SetChessExist(indexX, indexY, true);

			m_isSend = false;
		}
		break;
		// Ű�Է¿� ���� ������ ó��
		case NETTYPE_INPUT:
		{
			cout << " 2. Input : " << InputTypes[m_infoBuffer[TYPE_INPUT] - 0x25]
				<< " 3. Index : [" << indexX << ", " << indexY << "] "
				<< "(" << receiveBytes << " bytes)" << endl;

			m_canMove = m_chessManager->ProcessKeyInput(&m_newIndexX, &m_newIndexY,
				indexX, indexY, m_infoBuffer[TYPE_INPUT]);

			// ���ο� ��ǥ ����
			m_infoBuffer[TYPE_INDEX_X] = (char)m_newIndexX;
			m_infoBuffer[TYPE_INDEX_Y] = (char)m_newIndexY;

			m_isSend = true;
		}
		break;
		}

		// �� �� �ִ°� ���°��� �˷��� ����
		if (true == m_canMove)
			m_infoBuffer[TYPE_CANMOVE] = 'O';
		else
			m_infoBuffer[TYPE_CANMOVE] = 'X';

		m_infoBuffer[TYPE_INFO_END] = '\0';
	}
	else
	{
		return -1;
	}

	return 0;
}

int MainServer::SendProcess()
{
	if (false == m_isSend)
		return 1; // continue

	//����
	const char* NetTypes[] = { "Type_CanMove", "Type_AdjustMove", "Type_Input" };
	int sendBytes = send(m_clientSocket, m_infoBuffer, (int)strlen(m_infoBuffer) + 1, 0);
	if (sendBytes > 0)
	{
		NETWORKTYPE type = (NETWORKTYPE)m_infoBuffer[TYPE_NETWORKTYPE];

		if (type >= NETTYPE_END)
			return -1;

		cout << "Send    1. NetworkType : " << NetTypes[type - 1];

		// Ÿ�Ժ��� �۾��� �����Ѵ�.
		switch (type)
		{
			// ������ �� �ִ°��� ���� ������ ó��
		case NETTYPE_CANMOVE:
		{
			cout << " 2. Can Move : " << m_infoBuffer[TYPE_CANMOVE]
				<< "(" << sendBytes << " bytes)" << endl << endl;
		}
		break;
		case NETTYPE_INPUT:
		{
			cout << " 2. Move to Index : [" << (int)m_infoBuffer[TYPE_INDEX_X] << ", " << (int)m_infoBuffer[TYPE_INDEX_Y] << "] "
				<< " 3. Can Move : " << m_infoBuffer[TYPE_CANMOVE]
				<< "(" << sendBytes << " bytes)" << endl << endl;
		}
		break;
		}
	}
	else
	{
		return -1;
	}

	return 0;
}
