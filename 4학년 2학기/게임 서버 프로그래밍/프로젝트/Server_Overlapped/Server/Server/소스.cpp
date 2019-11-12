#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <map>
using namespace std;
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#define MAX_BUFFER 1024
#define SERVER_PORT 3500

struct SOCKETINFO {
	// Window에서 사용하는 자료구조
	// 윈도우 소켓 API Overlapped 
	// 신경 쓸 필요 없다.(운영체제가 자체적으로 사용하기 때문에)
	// 초기화 한다음에 넣어줘야 한다.
	// Send, Receive할때에도 0으로 초기화 하고 넣어줘야 한다.
	// 운영체제가 사용하고 있는 도중에는 건드리면 절대 안된다.
	WSAOVERLAPPED overlapped;
	// Window에서 사용하는 자료구조
	// send, recvs 대신에...
	// WSASend, WSARecv를 사용해야 한다.
	// 거기에 들어갈 buffer 매개변수는 이것을 사용해야 한다.
	// WSARecv(socket, wsabuf, bufcount)
	// WSABUF => buf1(char*) len1(int) buf2 len2 .... 로 되어있는 배열이다.
	// 메모리를 많이 차지하고 복사할때 오버헤드가 존재한다.
	WSABUF dataBuffer;

	SOCKET socket;
	// Accept 되면 무조건 등록시켜 놓은다.
	char messageBuffer[MAX_BUFFER];
};

map <SOCKET, SOCKETINFO> clients;
void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);
void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);

// 콜백 함수의 매개변수 타입 순서는 항상 동일해야 한다.
// 어느 소켓에서 왔는지에 대한 정보가 없다? 
void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags)
{
	// 이 함수를 탔다는 것은 소켓 I/O를 완료 했다는 것이다. 
	// 데이타를 받았다는 것이다.

	// 소켓정보를 받아오는 꼼수.
	// 이것은 호환성의 문제가 있을 수도 있다.
	SOCKET client_s = reinterpret_cast<int>(overlapped->hEvent);
	// 클라이언트가 접속을 끊었네?
	if (dataBytes == 0) {
		// 나도 접속 종료
		closesocket(clients[client_s].socket);
		clients.erase(client_s);
		return;
	} // 클라이언트가 closesocket을 했을 경우
	clients[client_s].messageBuffer[dataBytes] = 0;
	cout << "From client : " << clients[client_s].messageBuffer << " (" << dataBytes << ") bytes)\n";
	// Send할때에는 받은 값 만큼의 길이만 해야한다.
	clients[client_s].dataBuffer.len = dataBytes;
	// send를 쓰기 전에 overlapped 구조체를 초기화 시킨다.
	memset(&(clients[client_s].overlapped), 0x00, sizeof(WSAOVERLAPPED));
	clients[client_s].overlapped.hEvent = (HANDLE)client_s;
	WSASend(client_s, &(clients[client_s].dataBuffer), 1, &dataBytes, 0,
		&(clients[client_s].overlapped), send_callback);
}

void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags)
{
	// 이 함수를 탔다는 것은 소켓 I/O를 완료 했다는 것이다. 
	// 데이타를 받았다는 것이다.

	DWORD receiveBytes = 0;
	DWORD flags = 0;
	SOCKET client_s = reinterpret_cast<int>(overlapped->hEvent);
	if (dataBytes == 0) {
		closesocket(clients[client_s].socket);
		clients.erase(client_s);
		return;
	} // 클라이언트가 closesocket을 했을 경우
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
	while (true) {
		clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &addrLen);
		clients[clientSocket] = SOCKETINFO{};
		memset(&clients[clientSocket], 0, sizeof(struct SOCKETINFO));
		clients[clientSocket].socket = clientSocket;
		clients[clientSocket].dataBuffer.len = MAX_BUFFER;
		// 일단은 배열에서 하나만 Setting해놓았다.
		clients[clientSocket].dataBuffer.buf = clients[clientSocket].messageBuffer;
		flags = 0;
		// 소켓정보를 recv_callback함수 내에서 알수 있도록 
		// .hEvent???
		// ==> 소켓정보를 전달하기 위해 만들어 놓은 자료구조가 아니기 때문에 
		// 이벤트 핸들용이다.
		clients[clientSocket].overlapped.hEvent = (HANDLE)clients[clientSocket].socket;
		// 클라이언트 들어올때마다 즉시 응답이 가능하다.
		// 서버로 들어오는 배열에 버퍼가 몇개 있는가? ==> 1개
		// 
		// 모든 콘텐츠에 대한 처리는 Callback에서 처리한다.
		WSARecv(clients[clientSocket].socket, &clients[clientSocket].dataBuffer, 1, 
			NULL /*등록만 시킬것이므로 NULL*/,
			&flags, &(clients[clientSocket].overlapped)/*오버랩 구조체*/, 
			recv_callback/*Data가 오면 여기서 호출하라고 운영체제에게 맡김*/);
	}
	closesocket(listenSocket);
	WSACleanup();
}