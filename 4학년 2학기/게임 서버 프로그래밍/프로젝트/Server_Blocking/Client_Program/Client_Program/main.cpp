#include<stdio.h>
//#include<WinSock2.h>	// �ֽ� ���
#include<WS2tcpip.h>	//
#pragma comment(lib, "Ws2_32.lib")
// 32 (win NT) -> 16�� �ƴϴ�.(win 95, 3.0, dos)

#define MAX_BUFFER 1024
#define SERVER_IP "127.0.0.1"	// ���� �ּ�(�ڱ� �ڽ��� ����)
#define SERVER_PORT 3500	// ��Ʈ ��ȣ(���� �ؾߵȴ�)

int main()
{
	// WSA : Windows Socket API
	WSADATA WSAData;
	// ������ ������ ���� Network API�� �����ϰ� �ϴ� �Լ�
	// ��? ������ ��Ʈ��ũ ǥ���� ���ͳ��� �ƴϴ�.
	// ���� �ϸ鼭 �ᱹ �����ϰ� �Ǿ �̷��� �Ǿ���.
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	SOCKET listenSocket = WSASocket(AF_INET/*ipv4*/, SOCK_STREAM/*tcp/ip*/
		, 0, NULL, 0, 0);
	// ǥ���� �ٲ�� ���� �̰� ����Ͽ��� �Ѵ�.
	// char buffer[INET6_ADDRSTRLEN];
	// Server Address
	SOCKADDR_IN serverAddr;
	// ZeroMemory
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	// �ڷᱸ�� ��ȯ
	serverAddr.sin_family = AF_INET; /* protocol address? */
	serverAddr.sin_port = htons(SERVER_PORT); /* host to networt format change */
	/* inet_pton => pointer(string) to network : IP�ּҸ� ���ڷ� �ٲ۴�.
		ǥ���� IPv6�� �����ϰ� �ִ�.*/
	//serverAddr.sin_addr.S_un.S_addr = inet_pton(AF_INET, SERVER_IP, buffer);
	inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);
	
	/* File Stream���� File Open�� �����ϴ�. */
	connect(listenSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

	while (1)
	{
		char messageBuffer[MAX_BUFFER];
		printf_s("Enter Message -> ");
		scanf_s("%s", messageBuffer, MAX_BUFFER);
		int bufferLen = strlen(messageBuffer);
		/* ����ϰ� �����͸� ������ ���� ������ ��ŭ�� �����Ѵ�. */
		int sendBytes = send(listenSocket, messageBuffer, bufferLen, 0);
		if (sendBytes > 0)
		{
			/* ���� �� ��� */
			printf("TRACE - Send message : %s (%d bytes)\n", messageBuffer, sendBytes);
			/* ������ ���� �ٽ� �޴´�. 
			 �����Ʈ ��ŭ ������ �� �� ���� ������ MAX_BUFFER�� �Ѵ�. 
			 �׸��� ������ ���ϰ����� ���� ����Ʈ���� ���´�.
			 �ѹ��� ���޾��� ��� ������� ��������. ������ ũ��� ������ �ȴ�. 
			 0�� ������ ������ ��� ������ ���� 
			 MAX_BUFFER��ŭ ������ ���� ��ٸ��� ������ ���� ª�� �ð��̴�. 
			 �ٵ� �������� �ʴ´ٸ� ���� �ڵ忡�� ��� �����ִ´�.
			 �̰��� Blocking I/O��� �Ѵ�. *(Synchronous I/O) */
			int receiveBytes = recv(listenSocket, messageBuffer, MAX_BUFFER, 0);
			if (receiveBytes > 0)
			{
				/* ���� �� ��� */
				printf("TRACE - Receive message : %s (%d bytes)\n* EnterMessage\n-", messageBuffer, receiveBytes);
			}
		}
	}
	closesocket(listenSocket);
	WSACleanup();
}