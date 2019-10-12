#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <map>
#include <thread>
using namespace std;
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#define MAX_BUFFER 1024
#define SERVER_PORT 3500

struct OVER_EX {
	WSAOVERLAPPED over;
	WSABUF wsabuf[1];
	char net_buf[MAX_BUFFER];
	bool is_recv; // send인가 recv인가 검사
};

struct SOCKETINFO {
	// 확장 구조체
	OVER_EX recv_over;
	SOCKET socket;
	int id;
};

map <SOCKET, SOCKETINFO> clients;
HANDLE g_iocp;

void do_worker()
{
	while (true){
		DWORD num_byte;
		ULONG key;
		PULONG p_key = &key;
		WSAOVERLAPPED *p_over;

		GetQueuedCompletionStatus(g_iocp, &num_byte, p_key, &p_over, INFINITE);

		OVER_EX *over_ex = reinterpret_cast<OVER_EX*>(p_over);

		if (true == over_ex->is_recv) {
			// Recv

			SOCKET client_s = static_cast<SOCKET>(key);

			if (num_byte == 0) {
				closesocket(client_s);
				clients.erase(client_s);
				continue;
			}
			over_ex->net_buf[num_byte] = '\0';
			cout << "From client : [" << client_s << "] ";
			cout << over_ex->net_buf << " (" << num_byte << ") bytes)\n";

			//오버헤드 구조체를 새로 만들어 줘야 한다.

			// 그냥 날라가 버리니까 new해줘야함
			OVER_EX *send_over = new OVER_EX;
			memset(send_over, 0x00, sizeof(OVER_EX));
			send_over->is_recv = false;
			memcpy(send_over->net_buf, over_ex->net_buf, num_byte);
			send_over->wsabuf[0].buf = send_over->net_buf;
			send_over->wsabuf[0].len = num_byte;
			WSASend(client_s, send_over->wsabuf, 1, 0, 0, &send_over->over, NULL);
			DWORD flags = 0;
			memset(&over_ex->over, 0x00, sizeof(WSAOVERLAPPED));
			WSARecv(client_s, over_ex->wsabuf, 1, 0, &flags, &over_ex->over, NULL);
		}
		else {
			// Send
			SOCKET client_s = static_cast<SOCKET>(key);
			
			if (num_byte == 0) {
				closesocket(client_s);
				clients.erase(client_s);
				delete p_over;
				continue;
			} 

			OVER_EX *over_ex = reinterpret_cast<OVER_EX*>(p_over);

			cout << "Send Message : " << client_s;
			cout << "Sent : " << over_ex->net_buf << " (" << num_byte << " bytes)\n";

			delete p_over;
		}
	}
}

int main()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0,
		WSA_FLAG_OVERLAPPED/*Overlapped IO를 쓰기 위해서는 이걸 꼭 해주어야 함*/);
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	::bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN));
	listen(listenSocket, 5);
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	memset(&clientAddr, 0, addrLen);
	SOCKET clientSocket;
	DWORD flags;

	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	thread worker_thread{ do_worker };

	while (true) {
		// 일단은 그대로
		clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &addrLen);
		clients[clientSocket] = SOCKETINFO{};
		memset(&clients[clientSocket], 0, sizeof(struct SOCKETINFO));
		clients[clientSocket].socket = clientSocket;
		//
		clients[clientSocket].recv_over.wsabuf[0].len = MAX_BUFFER;
		clients[clientSocket].recv_over.wsabuf[0].buf = clients[clientSocket].recv_over.net_buf;
		clients[clientSocket].recv_over.is_recv = true;
		flags = 0;
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), g_iocp, clientSocket, 0);
		WSARecv(clientSocket, clients[clientSocket].recv_over.wsabuf, 1, NULL, &flags,
			&(clients[clientSocket].recv_over.over), NULL);
	}
	worker_thread.join();
	closesocket(listenSocket);
	WSACleanup();
}