#include<stdio.h>
//#include<WinSock2.h>	// 최신 기술
#include<WS2tcpip.h>	//
#pragma comment(lib, "Ws2_32.lib")
// 32 (win NT) -> 16이 아니다.(win 95, 3.0, dos)

#define MAX_BUFFER 1024
#define SERVER_IP "127.0.0.1"	// 서버 주소(자기 자신을 뜻함)
#define SERVER_PORT 3500	// 포트 번호(통일 해야된다)

int main()
{
	// WSA : Windows Socket API
	WSADATA WSAData;
	// 윈도우 소켓의 시작 Network API를 동작하게 하는 함수
	// 왜? 윈도우 네트워크 표준은 인터넷이 아니다.
	// 깨갱 하면서 결국 지원하게 되어서 이렇게 되었다.
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	SOCKET listenSocket = WSASocket(AF_INET/*ipv4*/, SOCK_STREAM/*tcp/ip*/
		, 0, NULL, 0, 0);
	// 표준이 바뀌어 지금 이걸 사용하여야 한다.
	// char buffer[INET6_ADDRSTRLEN];
	// Server Address
	SOCKADDR_IN serverAddr;
	// ZeroMemory
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	// 자료구조 변환
	serverAddr.sin_family = AF_INET; /* protocol address? */
	serverAddr.sin_port = htons(SERVER_PORT); /* host to networt format change */
	/* inet_pton => pointer(string) to network : IP주소를 숫자로 바꾼다.
		표준이 IPv6를 강제하고 있다.*/
	//serverAddr.sin_addr.S_un.S_addr = inet_pton(AF_INET, SERVER_IP, buffer);
	inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);
	
	/* File Stream에서 File Open과 동일하다. */
	connect(listenSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

	while (1)
	{
		char messageBuffer[MAX_BUFFER];
		printf_s("Enter Message -> ");
		scanf_s("%s", messageBuffer, MAX_BUFFER);
		int bufferLen = strlen(messageBuffer);
		/* 깔끔하게 데이터만 보내기 위해 사이즈 만큼만 전송한다. */
		int sendBytes = send(listenSocket, messageBuffer, bufferLen, 0);
		if (sendBytes > 0)
		{
			/* 보낸 값 출력 */
			printf("TRACE - Send message : %s (%d bytes)\n", messageBuffer, sendBytes);
			/* 서버로 부터 다시 받는다. 
			 몇바이트 만큼 받을지 알 수 없기 때문에 MAX_BUFFER로 한다. 
			 그리고 받을때 리턴값으로 받은 바이트수가 나온다.
			 한번에 못받았을 경우 순서대로 읽혀진다. 적당한 크기로 받으면 된다. 
			 0이 나오면 서버가 어떠한 이유로 닫힘 
			 MAX_BUFFER만큼 받을때 까지 기다리는 하지만 아주 짧은 시간이다. 
			 근데 보내주지 않는다면 이쪽 코드에서 계속 멈춰있는다.
			 이것을 Blocking I/O라고 한다. *(Synchronous I/O) */
			int receiveBytes = recv(listenSocket, messageBuffer, MAX_BUFFER, 0);
			if (receiveBytes > 0)
			{
				/* 받은 값 출력 */
				printf("TRACE - Receive message : %s (%d bytes)\n* EnterMessage\n-", messageBuffer, receiveBytes);
			}
		}
	}
	closesocket(listenSocket);
	WSACleanup();
}