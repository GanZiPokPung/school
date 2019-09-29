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
	// 윈속 초기화
	if (0 != WSAStartup(MAKEWORD(2, 0), &m_WSAData))
		return false;

	cout << "WinSock 초기화 성공!" << endl;

	// 소켓 생성
	m_listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
	if (INVALID_SOCKET == m_listenSocket)
		return false;

	cout << "listen socket 생성 성공!" << endl;

	// 주소 구조체 
	ZeroMemory(&m_serverAddr, sizeof(m_serverAddr));
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons(SERVER_PORT);
 	m_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// listen소켓의 지역 IP 주소와 지역 포트 번호를 결정
	if (SOCKET_ERROR == ::bind(m_listenSocket, (sockaddr*)& m_serverAddr, sizeof(m_serverAddr)))
		return false;

	cout << "listen socket bind() 성공!" << endl;

	// 소켓의 상태를 listen 상태로 만든다.
	if (SOCKET_ERROR == listen(m_listenSocket, SOMAXCONN))
		return false;

	cout << "listen socket 상태 listen으로 변경 성공!" << endl;
	cout << "메인 서버 초기화 성공!" << endl << endl;


	m_chessManager = GET_MANAGER<ChessManager>();
	m_chessManager->Initialize();

	return true;
}

int MainServer::Running()
{
	cout << "서버가 클라이언트의 connect()를 기다립니다..." << endl;

	while (true)
	{
		int addr_size = sizeof(m_clientAddr);
		m_clientSocket = accept(m_listenSocket, (sockaddr*)& m_clientAddr, &addr_size);

		cout << "클라이언트가 접속하였습니다." << endl;
		cout << "IP : " << inet_ntoa(m_clientAddr.sin_addr)
			<< " Port번호 : " << ntohs(m_clientAddr.sin_port) << endl;

		
		while (true)
		{
			int result;

			result = ReceiveProcess();
			// 받은 처리
			if (-1 == result)
				break;

			result = SendProcess();
			// 보낸 처리
			if (-1 == result)
				break;
			else if (1 == result)
				continue;		
		}
		closesocket(m_clientSocket);
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

	// 받아옴
	int receiveBytes = recv(m_clientSocket, m_infoBuffer, MAX_BUFFER, 0);
	if (receiveBytes > 0)
	{
		// 프로세스 처리
		m_infoBuffer[receiveBytes] = '\0';

		// 네트워크 통신으로 처리할 타입을 먼저 받는다.
		NETWORKTYPE type = (NETWORKTYPE)m_infoBuffer[TYPE_NETWORKTYPE];

		if (type >= NETTYPE_END)
			return -1;

		cout << "Receive 1. NetworkType : " << NetTypes[type - 1];

		int indexX = (int)m_infoBuffer[TYPE_INDEX_X];
		int indexY = (int)m_infoBuffer[TYPE_INDEX_Y];

		// 타입별로 작업을 수행한다.
		switch (type)
		{
			// 움직일 수 있는가에 대한 정보를 처리
		case NETTYPE_CANMOVE:
		{
			cout << " 2. Index : [" << indexX << ", " << indexY << "] "
				<< "(" << receiveBytes << " bytes)" << endl;

			m_canMove = m_chessManager->CanMove(indexX, indexY);

			m_isSend = true;
		}
		break;
		// 인덱스에 대한 존재여부를 적용하는 처리
		case NETTYPE_ADJMOVE:
		{
			cout << " 2. Index : [" << indexX << ", " << indexY << "] "
				<< "(" << receiveBytes << " bytes)" << endl << endl;

			m_chessManager->SetChessExist(indexX, indexY, true);

			m_isSend = false;
		}
		break;
		// 키입력에 대한 움직임 처리
		case NETTYPE_INPUT:
		{
			cout << " 2. Input : " << InputTypes[m_infoBuffer[TYPE_INPUT] - 0x25]
				<< " 3. Index : [" << indexX << ", " << indexY << "] "
				<< "(" << receiveBytes << " bytes)" << endl;

			m_canMove = m_chessManager->ProcessKeyInput(&m_newIndexX, &m_newIndexY,
				indexX, indexY, m_infoBuffer[TYPE_INPUT]);

			// 새로운 좌표 갱신
			m_infoBuffer[TYPE_INDEX_X] = (char)m_newIndexX;
			m_infoBuffer[TYPE_INDEX_Y] = (char)m_newIndexY;

			m_isSend = true;
		}
		break;
		}

		// 갈 수 있는가 없는가를 알려줄 변수
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

	//보냄
	const char* NetTypes[] = { "Type_CanMove", "Type_AdjustMove", "Type_Input" };
	int sendBytes = send(m_clientSocket, m_infoBuffer, (int)strlen(m_infoBuffer) + 1, 0);
	if (sendBytes > 0)
	{
		NETWORKTYPE type = (NETWORKTYPE)m_infoBuffer[TYPE_NETWORKTYPE];

		if (type >= NETTYPE_END)
			return -1;

		cout << "Send    1. NetworkType : " << NetTypes[type - 1];

		// 타입별로 작업을 수행한다.
		switch (type)
		{
			// 움직일 수 있는가에 대한 정보를 처리
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
