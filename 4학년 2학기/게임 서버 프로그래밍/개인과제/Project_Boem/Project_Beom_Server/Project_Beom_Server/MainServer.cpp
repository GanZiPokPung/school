#include "pch.h"
#include "MainServer.h"

map<SOCKET, SOCKETINFO> MainServer::m_mapClients;
int MainServer::m_userCount = 1;

mutex queue_process;

const char* NetTypes[] = { "Type_CanMove", "Type_AdjustMove", "Type_SetIndex",  "Type_Input", "Type_Mine", "Type_Other" };
const char* InputTypes[] = { "VK_LEFT", "VK_UP", "VK_RIGHT", "VK_DOWN" };

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
	if (0 != WSAStartup(MAKEWORD(2, 2), &m_WSAData))
		return false;

	cout << "WinSock 초기화 성공!" << endl;

	// 소켓 생성
	m_listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
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

	m_chessManager = GET_MANAGER<ChessManager>();
	m_chessManager->Initialize();

	cout << "메인 서버 초기화 성공!" << endl << endl;

	return true;
}

int MainServer::Running()
{
	cout << "서버가 클라이언트의 connect()를 기다립니다..." << endl;
	DWORD flags;

	while (true)
	{
		int addr_size = sizeof(m_clientAddr);
		m_clientSocket = accept(m_listenSocket, (sockaddr*)& m_clientAddr, &addr_size);

		m_mapClients[m_clientSocket] = SOCKETINFO{ };
		ZeroMemory(&m_mapClients[m_clientSocket], sizeof(SOCKETINFO));
		m_mapClients[m_clientSocket].socket = m_clientSocket;
		m_mapClients[m_clientSocket].dataBuffer.len = MAX_BUFFER;
		m_mapClients[m_clientSocket].dataBuffer.buf = m_mapClients[m_clientSocket].messageBuffer;
		flags = 0;
		m_mapClients[m_clientSocket].overlapped.hEvent = (HANDLE)m_mapClients[m_clientSocket].socket;

		m_mapClients[m_clientSocket].clientAddr = m_clientAddr;
		printf("클라이언트가 접속하였습니다. 포트번호 : %d, IP주소 : %s\n",
			ntohs(m_clientAddr.sin_port), inet_ntoa(m_clientAddr.sin_addr));

		WSARecv(m_mapClients[m_clientSocket].socket, &m_mapClients[m_clientSocket].dataBuffer, 1, NULL,
			&flags, &(m_mapClients[m_clientSocket].overlapped), recv_callback);
	}

	return 0;
}

void MainServer::Release()
{
	for (auto& clientSocketInfo : m_mapClients)
	{
		closesocket(clientSocketInfo.second.socket);
	}
	m_mapClients.clear();

	closesocket(m_listenSocket);
	WSACleanup();
}

int MainServer::ReceiveProcess(char* buf, bool& isSend, const DWORD& receivedByte, const SOCKET& clientsocket)
{
	// 우선 받은 패킷을 데이터로 변환 시킨다.
	ClassPacket<MOVEINFO> packet;
	//MakePacketToData(&packet, buf);

	packet.MakePacketToData(buf);

	PACKETTYPE type = packet.GetMustInfo().netType;
	if (type >= NETTYPE_END || type < 0)
		return -1;

	bool isMove;

	// 타입별로 작업을 수행한다.
	switch (type)
	{
		// 움직일 수 있는가에 대한 정보를 처리
	case NETTYPE_CANMOVE:
	{
		ClassPacket<MOVEINFO> newPacket;
		newPacket.MakePacketToData(buf);

		MUSTINFO& mustInfo = newPacket.GetMustInfo();
		MOVEINFO &info = newPacket.GetInfo();
		//
		printf("[Receive] 1.NetworkType: %s", NetTypes[type]);
		printf(" 2.Index: [%d, %d] 3.CanMove: default (%d bytes)\n", mustInfo.indexX, mustInfo.indexY,
			 receivedByte);
		//
		isMove = GET_MANAGER<ChessManager>()->CanMove(&info.others_id, mustInfo.indexX, mustInfo.indexY);
		if(true == isMove) info.canMove = 'O';
		else info.canMove = 'X';

		isSend = true;
		newPacket.MakeDataToPacket(buf);
	}
	break;
	// 인덱스에 대한 존재여부를 적용하는 처리
	case NETTYPE_ADJMOVE:
	{
		ClassPacket<MOVEINFO> newPacket;
		newPacket.MakePacketToData(buf);

		MUSTINFO& mustInfo = newPacket.GetMustInfo();
		MOVEINFO& info = newPacket.GetInfo();

		int id = mustInfo.id;
		//
		printf("[Receive] 1.NetworkType: %s", NetTypes[type]);
		printf(" 2.Id: %d 3.Index: [%d, %d] 4.CanMove: %c (%d bytes)\n\n", id, 
			mustInfo.indexX, mustInfo.indexY,
			info.canMove, receivedByte);
		//
		GET_MANAGER<ChessManager>()->SetChessExist(mustInfo.indexX, mustInfo.indexY, true, id);
		
		m_mapClients[clientsocket].curIndex.x = mustInfo.indexX;
		m_mapClients[clientsocket].curIndex.y = mustInfo.indexY;

		// 모든 클라이언트에게 정보를 전달하기 위한 큐에 등록한다.
		for (auto& client : m_mapClients)
		{
			// 바뀌는것은 자신한테 적용하지 않는다.
			if (client.first == clientsocket)
				continue;

			OTHERINFO otherInfo;
			ZeroMemory(&otherInfo, sizeof(OTHERINFO));
			otherInfo.type = CHANGE_INIT;
			otherInfo.others_id = id;
			otherInfo.others_indexX = mustInfo.indexX;
			otherInfo.others_indexY = mustInfo.indexY;
			
			m_mapClients[client.first].otherInfo.push(otherInfo);
		}

		isSend = false;
	}
	break;
	// 움직였을때 인덱스의 정보를 갱신하는 처리
	case NETTYPE_SETINDEX:
	{
		ClassPacket<INDEXINFO> newPacket;
		newPacket.MakePacketToData(buf);

		MUSTINFO& mustInfo = newPacket.GetMustInfo();
		INDEXINFO& info = newPacket.GetInfo();


		int id = mustInfo.id;

		printf("[Receive] 1.NetworkType: %s", NetTypes[type]);
		printf(" 2.Id: %d 3.NewIndex: [%d, %d] 4.OldIndex: [%d, %d] (%d bytes)\n\n",
						id, info.newIndexX, info.newIndexY, info.oldIndexX, info.oldIndexY, receivedByte);

		GET_MANAGER<ChessManager>()->SetChessExist(info.oldIndexX, info.oldIndexY, false, id);
		GET_MANAGER<ChessManager>()->SetChessExist(info.newIndexX, info.newIndexY, true, id);

		m_mapClients[clientsocket].curIndex.x = info.newIndexX;
		m_mapClients[clientsocket].curIndex.y = info.newIndexY;

		isSend = false;
	}
	break;
	// 키입력에 대한 움직임 처리
	case NETTYPE_INPUT:
	{
		ClassPacket<INPUTINFO> newPacket;
		newPacket.MakePacketToData(buf);

		MUSTINFO& mustInfo = newPacket.GetMustInfo();
		INPUTINFO& info = newPacket.GetInfo();

		int id = mustInfo.id;

		printf("[Receive] 1.NetworkType: %s", NetTypes[type]);
		printf(" 2.Id: %d 3.Index: [%d, %d] 4.Input: %d 5.CanMove: default (%d bytes)\n", 
			 id, mustInfo.indexX, mustInfo.indexY, info.inputType, receivedByte);

		int newIndexX, newIndexY;
		isMove = GET_MANAGER<ChessManager>()->ProcessKeyInput(&newIndexX, &newIndexY,
			mustInfo.indexX, mustInfo.indexY, info.inputType, id);

		mustInfo.indexX = (char)newIndexX;
		mustInfo.indexY = (char)newIndexY;

		if (true == isMove)
		{
			m_mapClients[clientsocket].curIndex.x = newIndexX;
			m_mapClients[clientsocket].curIndex.y = newIndexY;

			// 모든 클라이언트에게 정보를 전달하기 위한 큐에 등록한다.
			for (auto& client : m_mapClients)
			{
				// 바뀌는것은 자신한테 적용하지 않는다.
				if (client.first == clientsocket)
					continue;

				OTHERINFO otherInfo;
				ZeroMemory(&otherInfo, sizeof(OTHERINFO));
				otherInfo.type = CHANGE_MOVE;
				otherInfo.others_id = id;
				otherInfo.others_indexX = newIndexX;
				otherInfo.others_indexY = newIndexY;
				m_mapClients[client.first].otherInfo.push(otherInfo);
			}
		}
		
		if (true == isMove) info.canMove = 'O';
		else info.canMove = 'X';

		isSend = true;
		newPacket.MakeDataToPacket(buf);
	}
	break;
	// 자신에 대한 정보 처리 (ID)
	case NETTYPE_MINE:
	{
		ClassPacket<MOVEINFO> newPacket;
		newPacket.MakePacketToData(buf);

		MUSTINFO& mustInfo = newPacket.GetMustInfo();

		printf("[Receive] 1.NetworkType : %s\n", NetTypes[type]);
		mustInfo.id = (int)clientsocket;
		++m_userCount;

		isSend = true;
		newPacket.MakeDataToPacket(buf);
	}
	break;
	// 다른이에 대한 정보 처리 (ID)
	case NETTYPE_OTHER:
	{
		// 클라이언트 소켓내의 처리 큐에서 처리할게 있는지에 대한 여부
		if (true == m_mapClients[clientsocket].otherInfo.empty())
		{
			ZeroMemory(&packet, sizeof(packet));
		}
		else
		{
			ClassPacket<OTHERINFO> newPacket;
			newPacket.MakePacketToData(buf);

			// 있으면 그 정보를 꺼내 처리 한다.
			OTHERINFO otherInfo = m_mapClients[clientsocket].otherInfo.front();

			OTHERINFO& info = newPacket.GetInfo();

			info.type = otherInfo.type;
			info.others_id = otherInfo.others_id;
			info.others_indexX = otherInfo.others_indexX;
			info.others_indexY = otherInfo.others_indexY;

			m_mapClients[clientsocket].otherInfo.pop();
			newPacket.MakeDataToPacket(buf);
		}
		isSend = true;	
	}
	break;
	}

	return 0;
}

int MainServer::SendProcess(char* buf, DWORD& sendByte, const bool& isSend, const SOCKET& clientsocket)
{
	ClassPacket<MOVEINFO> packet;
	//MakePacketToData(&packet, buf);

	packet.MakePacketToData(buf);

	MUSTINFO mustInfo = packet.GetMustInfo();

	PACKETTYPE type = mustInfo.netType;
	if (type >= NETTYPE_END || type < 0)
		return -1;

	if (true == isSend)
	{
		int id = 0;
		int otherId = 0;

		
		switch (type)
		{
		case NETTYPE_CANMOVE:
		{
			sendByte = sizeof(ClassPacket<MOVEINFO>);
			ClassPacket<MOVEINFO> newPacket;
			newPacket.MakePacketToData(buf);
			MOVEINFO& info = newPacket.GetInfo();
			printf("[Send] 1.NetworkType: %s", NetTypes[type]);
			printf(" 2.OtherId: %d 3.Move to Index: [%d, %d] 4.Can Move: %c (%d bytes)\n\n",
				info.others_id, mustInfo.indexX, mustInfo.indexY, info.others_id, sendByte);
		}
			break;
		case NETTYPE_INPUT:
		{
			sendByte = sizeof(ClassPacket<INPUTINFO>);
			ClassPacket<INPUTINFO> newPacket;
			newPacket.MakePacketToData(buf);
			INPUTINFO& info = newPacket.GetInfo();
			printf("[Send] 1.NetworkType: %s", NetTypes[type]);
			printf(" 2.Input: %d 3.Move to Index: [%d, %d] 4.Can Move: %c (%d bytes)\n\n",
				info.inputType, mustInfo.indexX, mustInfo.indexY, info.canMove, sendByte);
		}
			break;
		case NETTYPE_MINE:
			sendByte = sizeof(ClassPacket<MOVEINFO>);
			printf("[Send] 1.NetworkType: %s", NetTypes[type]);
			printf(" 2.ID(Socket): %d (%d bytes)\n\n", (int)clientsocket, sendByte);
			break;
		case NETTYPE_OTHER:
			sendByte = sizeof(ClassPacket<OTHERINFO>);
			break;
		}
	}
	else
	{
		sendByte = 1;
		buf[sendByte] = '\0';
	}

	return 0;
}

int MainServer::EndProcess(const SOCKET& clientsocket)
{
	GET_MANAGER<ChessManager>()->SetChessExist(m_mapClients[clientsocket].curIndex.x, m_mapClients[clientsocket].curIndex.y,
				false, (int)clientsocket);

	// 모든 클라이언트에게 정보를 전달하기 위한 큐에 등록한다.
	for (auto& client : m_mapClients)
	{
		// 바뀌는것은 자신한테 적용하지 않는다.
		if (client.first == clientsocket)
			continue;

		OTHERINFO otherInfo;
		ZeroMemory(&otherInfo, sizeof(OTHERINFO));
		otherInfo.type = CHANGE_EXIT;
		otherInfo.others_id = (int)clientsocket;
		otherInfo.others_indexX = 1;
		otherInfo.others_indexY = 1;
		m_mapClients[client.first].otherInfo.push(otherInfo);
	}

	--m_userCount;

	return 0;
}

void CALLBACK MainServer::recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD InFlags)
{
	SOCKET client_s = reinterpret_cast<int>(overlapped->hEvent);
	if (dataBytes == 0)
	{
		// 종료 처리
		EndProcess(client_s);
		closesocket(m_mapClients[client_s].socket);
		printf("클라이언트가 종료되었습니다.포트번호: %d, IP주소: %s\n",
			ntohs(m_mapClients[client_s].clientAddr.sin_port), inet_ntoa(m_mapClients[client_s].clientAddr.sin_addr));
		m_mapClients.erase(client_s);
		return;
	}

	m_mapClients[client_s].messageBuffer[dataBytes] = 0;

	DWORD sendByte;
	bool isSend;

	// 받은 처리
	ReceiveProcess(m_mapClients[client_s].messageBuffer, isSend, dataBytes, client_s);

	// 보낼 처리
	SendProcess(m_mapClients[client_s].messageBuffer, sendByte, isSend, client_s);	
	m_mapClients[client_s].dataBuffer.len = sendByte;
	m_mapClients[client_s].dataBuffer.buf = m_mapClients[client_s].messageBuffer;

	ZeroMemory(&(m_mapClients[client_s].overlapped), sizeof(WSAOVERLAPPED));
	m_mapClients[client_s].overlapped.hEvent = (HANDLE)client_s;
	WSASend(client_s, &(m_mapClients[client_s].dataBuffer), 1, &sendByte, 0,
		&(m_mapClients[client_s].overlapped), send_callback);
}

void CALLBACK MainServer::send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD InFlags)
{
	DWORD receiveBytes = 0;
	DWORD flags = 0;

	SOCKET client_s = reinterpret_cast<int>(overlapped->hEvent);

	if (dataBytes == 0) 
	{
		// 종료 처리
		EndProcess(client_s);
		closesocket(m_mapClients[client_s].socket);
		printf("클라이언트가 종료되었습니다.포트번호: %d, IP주소: %s\n",
			ntohs(m_mapClients[client_s].clientAddr.sin_port), inet_ntoa(m_mapClients[client_s].clientAddr.sin_addr));
		m_mapClients.erase(client_s);
		return;
	}

	m_mapClients[client_s].dataBuffer.len = MAX_BUFFER;
	m_mapClients[client_s].dataBuffer.buf = m_mapClients[client_s].messageBuffer;
	
	ZeroMemory(&(m_mapClients[client_s].overlapped), sizeof(WSAOVERLAPPED));
	m_mapClients[client_s].overlapped.hEvent = (HANDLE)client_s;
	WSARecv(client_s, &m_mapClients[client_s].dataBuffer, 1, 0, &flags, 
		&(m_mapClients[client_s].overlapped), recv_callback);
}
