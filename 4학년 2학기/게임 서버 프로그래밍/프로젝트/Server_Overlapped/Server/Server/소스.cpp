#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <map>
using namespace std;
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#define MAX_BUFFER 1024
#define SERVER_PORT 3500
struct SOCKETINFO {
	// Window���� ����ϴ� �ڷᱸ��
	// ������ ���� API Overlapped 
	// �Ű� �� �ʿ� ����.(�ü���� ��ü������ ����ϱ� ������)
	// �ʱ�ȭ �Ѵ����� �־���� �Ѵ�.
	// Send, Receive�Ҷ����� 0���� �ʱ�ȭ �ϰ� �־���� �Ѵ�.
	WSAOVERLAPPED overlapped;
	// Window���� ����ϴ� �ڷᱸ��
	// send, recvs ��ſ�...
	// WSASend, WSARecv�� ����ؾ� �Ѵ�.
	// �ű⿡ �� buffer �Ű������� �̰��� ����ؾ� �Ѵ�.
	// WSARecv(socket, wsabuf, bufcount)
	// WSABUF => buf1(char*) len1(int) buf2 len2 .... �� �Ǿ��ִ� �迭�̴�.
	// �޸𸮸� ���� �����ϰ� �����Ҷ� ������尡 �����Ѵ�.
	WSABUF dataBuffer;

	SOCKET socket;
	// Accept �Ǹ� ������ ��Ͻ��� ������.
	char messageBuffer[MAX_BUFFER];
};
map <SOCKET, SOCKETINFO> clients;
void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);
void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);

// �ݹ� �Լ��� �Ű����� Ÿ�� ������ �׻� �����ؾ� �Ѵ�.
// ��� ���Ͽ��� �Դ����� ���� ������ ����? 
void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags)
{
	// ���������� �޾ƿ��� �ļ�.
	// �̰��� ȣȯ���� ������ ���� ���� �ִ�.
	SOCKET client_s = reinterpret_cast<int>(overlapped->hEvent);
	// Ŭ���̾�Ʈ�� ������ ������?
	if (dataBytes == 0) {
		// ���� ���� ����
		closesocket(clients[client_s].socket);
		clients.erase(client_s);
		return;
	} // Ŭ���̾�Ʈ�� closesocket�� ���� ���
	clients[client_s].messageBuffer[dataBytes] = 0;
	cout << "From client : " << clients[client_s].messageBuffer << " (" << dataBytes << ") bytes)\n";
	// Send�Ҷ����� ���� �� ��ŭ�� ���̸� �ؾ��Ѵ�.
	clients[client_s].dataBuffer.len = dataBytes;
	memset(&(clients[client_s].overlapped), 0x00, sizeof(WSAOVERLAPPED));
	clients[client_s].overlapped.hEvent = (HANDLE)client_s;
	WSASend(client_s, &(clients[client_s].dataBuffer), 1, &dataBytes, 0,
		&(clients[client_s].overlapped), send_callback);
}

void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags)
{
	DWORD receiveBytes = 0;
	DWORD flags = 0;
	SOCKET client_s = reinterpret_cast<int>(overlapped->hEvent);
	if (dataBytes == 0) {
		closesocket(clients[client_s].socket);
		clients.erase(client_s);
		return;
	} // Ŭ���̾�Ʈ�� closesocket�� ���� ���
	clients[client_s].dataBuffer.len = MAX_BUFFER;
	clients[client_s].dataBuffer.buf = clients[client_s].messageBuffer;
	cout << "Sent : " << clients[client_s].messageBuffer << " (" << dataBytes << " bytes)\n";
	memset(&(clients[client_s].overlapped), 0x00, sizeof(WSAOVERLAPPED));
	clients[client_s].overlapped.hEvent = (HANDLE)client_s;
	WSARecv(client_s, &clients[client_s].dataBuffer, 1, 0, &flags,
		&(clients[client_s].overlapped), recv_callback);
}

int main()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0,
		WSA_FLAG_OVERLAPPED/*Overlapped IO�� ���� ���ؼ��� �̰� �� ���־�� ��*/);
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
	while (true) {
		clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &addrLen);
		clients[clientSocket] = SOCKETINFO{};
		memset(&clients[clientSocket], 0, sizeof(struct SOCKETINFO));
		clients[clientSocket].socket = clientSocket;
		clients[clientSocket].dataBuffer.len = MAX_BUFFER;
		// �ϴ��� �迭���� �ϳ��� Setting�س��Ҵ�.
		clients[clientSocket].dataBuffer.buf = clients[clientSocket].messageBuffer;
		flags = 0;
		// ���������� recv_callback�Լ� ������ �˼� �ֵ��� 
		// .hEvent???
		// ==> ���������� �����ϱ� ���� ����� ���� �ڷᱸ���� �ƴϱ� ������ 
		// �̺�Ʈ �ڵ���̴�.
		clients[clientSocket].overlapped.hEvent = (HANDLE)clients[clientSocket].socket;
		// Ŭ���̾�Ʈ ���ö����� ��� ������ �����ϴ�.
		// ������ ������ �迭�� ���۰� � �ִ°�? ==> 1��
		// 
		// ��� �������� ���� ó���� Callback���� ó���Ѵ�.
		WSARecv(clients[clientSocket].socket, &clients[clientSocket].dataBuffer, 1, 
			NULL /*��ϸ� ��ų���̹Ƿ� NULL*/,
			&flags, &(clients[clientSocket].overlapped)/*������ ����ü*/, 
			recv_callback/*Data�� ���� ���⼭ ȣ���϶�� �ü������ �ñ�*/);
	}
	closesocket(listenSocket);
	WSACleanup();
}