#include "pch.h"
#include "MainServer.h"

map<SOCKET, SOCKETINFO> MainServer::m_mapClients;
int MainServer::m_userCount = 1;

const char* NetTypes[] = { "Type_CanMove", "Type_AdjustMove", "Type_SetIndex",  "Type_Input", "Type_Mine", "Type_Other" };
const char* InputTypes[] = { "VK_LEFT", "VK_UP", "VK_RIGHT", "VK_DOWN" };

HANDLE g_iocp;
thread worker;

void do_worker()
{
	while (true)
	{
		DWORD num_byte;
		ULONG key;
		PULONG p_key = &key;
		WSAOVERLAPPED* p_over;

		// IO �۾��� ���� ������ ������
		GetQueuedCompletionStatus(g_iocp, &num_byte, p_key, &p_over, INFINITE);

		// ���� ������
		SOCKETINFO* sock_Info = reinterpret_cast<SOCKETINFO*>(p_over);

		if (true == sock_Info->is_recv)
		{
			SOCKET client_s = static_cast<SOCKET>(key);

			// ���� ó��
			if (num_byte == 0)
			{
				// ���� ó��
				MainServer::EndProcess(client_s);
				closesocket(client_s);
				printf("Ŭ���̾�Ʈ�� ����Ǿ����ϴ�.��Ʈ��ȣ: %d, IP�ּ�: %s\n",
					ntohs(MainServer::m_mapClients[client_s].clientAddr.sin_port), 
					inet_ntoa(MainServer::m_mapClients[client_s].clientAddr.sin_addr));
				MainServer::m_mapClients.erase(client_s);
				continue;
			}

			MainServer::m_mapClients[client_s].messageBuffer[num_byte] = 0;

			DWORD sendByte;
			bool isSend;
			// ���� ó��
			MainServer::ReceiveProcess(MainServer::m_mapClients[client_s].messageBuffer, isSend, num_byte, client_s);

			// ���� ó��
			MainServer::SendProcess(MainServer::m_mapClients[client_s].messageBuffer, sendByte, isSend, client_s);

			SOCKETINFO* send_over = new SOCKETINFO;
			//ZeroMemory(&send_over, sizeof(SOCKETINFO));
			send_over->is_recv = false;
			memcpy(send_over->messageBuffer, sock_Info->messageBuffer, sendByte);
			send_over->dataBuffer.len = sendByte;
			send_over->dataBuffer.buf = send_over->messageBuffer;
			WSASend(client_s, &send_over->dataBuffer, 1, 0, 0, &send_over->overlapped, NULL);
			DWORD flags = 0;
			ZeroMemory(&(sock_Info->overlapped), sizeof(WSAOVERLAPPED));
			WSARecv(client_s, &sock_Info->dataBuffer, 1, 0, &flags, &sock_Info->overlapped, NULL);
		}
		else
		{
			// ���� ó��
			SOCKET client_s = static_cast<SOCKET>(key);

			if (num_byte == 0)
			{
				// ���� ó��
				MainServer::EndProcess(client_s);
				closesocket(client_s);
				printf("Ŭ���̾�Ʈ�� ����Ǿ����ϴ�.��Ʈ��ȣ: %d, IP�ּ�: %s\n",
					ntohs(MainServer::m_mapClients[client_s].clientAddr.sin_port), 
					inet_ntoa(MainServer::m_mapClients[client_s].clientAddr.sin_addr));
				MainServer::m_mapClients.erase(client_s);
				delete p_over;
				continue;
			}

			SOCKETINFO* sock_Info = reinterpret_cast<SOCKETINFO*>(p_over);

			// ���� �Ҵ� �߾����� ����
			delete p_over;
		}
	}
}

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
	if (0 != WSAStartup(MAKEWORD(2, 2), &m_WSAData))
		return false;

	cout << "WinSock �ʱ�ȭ ����!" << endl;

	// ���� ����
	m_listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
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

	// IOCP ��ü ����
	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	// �����忡�� ó��


	cout << "listen socket ���� listen���� ���� ����!" << endl;

	m_chessManager = GET_MANAGER<ChessManager>();
	m_chessManager->Initialize();

	// thread
	

	cout << "���� ���� �ʱ�ȭ ����!" << endl << endl;

	return true;
}

int MainServer::Running()
{
	cout << "������ Ŭ���̾�Ʈ�� connect()�� ��ٸ��ϴ�..." << endl;
	DWORD flags;

	thread worker{ do_worker };

	while (true)
	{
		int addr_size = sizeof(m_clientAddr);
		m_clientSocket = accept(m_listenSocket, (sockaddr*)& m_clientAddr, &addr_size);
		m_mapClients[m_clientSocket] = SOCKETINFO{ };
		//ZeroMemory(&m_mapClients[m_clientSocket], sizeof(SOCKETINFO));
		m_mapClients[m_clientSocket].socket = m_clientSocket;
		m_mapClients[m_clientSocket].dataBuffer.len = MAX_BUFFER;
		m_mapClients[m_clientSocket].dataBuffer.buf = m_mapClients[m_clientSocket].messageBuffer;
		m_mapClients[m_clientSocket].is_recv = true;
		flags = 0;

		m_mapClients[m_clientSocket].clientAddr = m_clientAddr;
		printf("Ŭ���̾�Ʈ�� �����Ͽ����ϴ�. ��Ʈ��ȣ : %d, IP�ּ� : %s\n",
			ntohs(m_clientAddr.sin_port), inet_ntoa(m_clientAddr.sin_addr));

		//Socket�� IOCP�� ����
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_clientSocket), g_iocp, m_clientSocket, 0);
		WSARecv(m_clientSocket, &m_mapClients[m_clientSocket].dataBuffer, 1, NULL, &flags,
			&(m_mapClients[m_clientSocket].overlapped), NULL);
	}
	worker.join();

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
	// �켱 ���� ��Ŷ�� �����ͷ� ��ȯ ��Ų��.
	ClassPacket<MOVEINFO> packet;
	//MakePacketToData(&packet, buf);

	packet.MakePacketToData(buf);

	PACKETTYPE type = packet.GetMustInfo().netType;
	if (type >= NETTYPE_END || type < 0)
		return -1;

	bool isMove;

	// Ÿ�Ժ��� �۾��� �����Ѵ�.
	switch (type)
	{
		// ������ �� �ִ°��� ���� ������ ó��
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
	// �ε����� ���� ���翩�θ� �����ϴ� ó��
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

		// ��� Ŭ���̾�Ʈ���� ������ �����ϱ� ���� ť�� ����Ѵ�.
		for (auto& client : m_mapClients)
		{
			// �ٲ�°��� �ڽ����� �������� �ʴ´�.
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
	// ���������� �ε����� ������ �����ϴ� ó��
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
	// Ű�Է¿� ���� ������ ó��
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

			// ��� Ŭ���̾�Ʈ���� ������ �����ϱ� ���� ť�� ����Ѵ�.
			for (auto& client : m_mapClients)
			{
				// �ٲ�°��� �ڽ����� �������� �ʴ´�.
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
	// �ڽſ� ���� ���� ó�� (ID)
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
	// �ٸ��̿� ���� ���� ó�� (ID)
	case NETTYPE_OTHER:
	{
		// Ŭ���̾�Ʈ ���ϳ��� ó�� ť���� ó���Ұ� �ִ����� ���� ����
		if (true == m_mapClients[clientsocket].otherInfo.empty())
		{
			ZeroMemory(&packet, sizeof(packet));
		}
		else
		{
			ClassPacket<OTHERINFO> newPacket;
			newPacket.MakePacketToData(buf);

			// ������ �� ������ ���� ó�� �Ѵ�.
			const OTHERINFO& otherInfo = m_mapClients[clientsocket].otherInfo.front();

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

	// ��� Ŭ���̾�Ʈ���� ������ �����ϱ� ���� ť�� ����Ѵ�.
	for (auto& client : m_mapClients)
	{
		// �ٲ�°��� �ڽ����� �������� �ʴ´�.
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
		// ���� ó��
		EndProcess(client_s);
		closesocket(m_mapClients[client_s].socket);
		printf("Ŭ���̾�Ʈ�� ����Ǿ����ϴ�.��Ʈ��ȣ: %d, IP�ּ�: %s\n",
			ntohs(m_mapClients[client_s].clientAddr.sin_port), inet_ntoa(m_mapClients[client_s].clientAddr.sin_addr));
		m_mapClients.erase(client_s);
		return;
	}

	m_mapClients[client_s].messageBuffer[dataBytes] = 0;

	DWORD sendByte;
	bool isSend;

	// ���� ó��
	ReceiveProcess(m_mapClients[client_s].messageBuffer, isSend, dataBytes, client_s);

	// ���� ó��
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
		// ���� ó��
		EndProcess(client_s);
		closesocket(m_mapClients[client_s].socket);
		printf("Ŭ���̾�Ʈ�� ����Ǿ����ϴ�.��Ʈ��ȣ: %d, IP�ּ�: %s\n",
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

