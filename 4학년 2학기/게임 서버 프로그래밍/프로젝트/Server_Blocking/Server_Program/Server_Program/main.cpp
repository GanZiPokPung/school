#include <stdio.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

// ������ �ּҸ� �� �ʿ䰡 ���� ������ ������ �ʴ´�.
// �׳� ���� �ּҸ� �̿��� ����ϸ� �Ǳ� �����̴�.

#define MAX_BUFFER 1024
#define SERVER_PORT 3500

int main()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	/* ��� �ּҸ� �ްڴ�. 
	�� �ּҿ��� ���°͸� �ްڴ�. (Ŭ���̾�Ʈ �ּҸ�) 
	���Ȱ�ȭ�� ���ؼ� 
	���� LANȤ�� ��ü ���ͳ� Ȥ�� �� ��ǻ�Ϳ����� ���� 
	Ŭ���̾�Ʈ ��Ŷ�� �ްڴ�. */
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	/* �ۿ��� ���� ��Ʈ ��ȣ�� ���� �ּҸ� ��� �ްڴٰ� 
	�ü���� ����ϴ� �۾��� �ϴ� �ڵ� (��Ʈ �ּ� ����) 
	::�� ���̴� ������ bind�� C++11�� Ű����(�Լ��������� �Լ����� �־��ִ� ���)
	�̱� ������ �װ��� ���Ϸ��� �ǵ��̴�. */
	::bind(listenSocket, (sockaddr *)&serverAddr, sizeof(serverAddr));
	/* ������ ��Ʈ �ּҸ� ������� �ü������ ��û�ϴ� �۾� 
	�ι�° ���� Backlog : ��⿭�� �����̴�. (��� Ŭ���̾�Ʈ���� ��⸦ �Ұ��ΰ�?) 
	MMORPG���� ��쿡�� ������ MAX���� �־��ش�. */
	listen(listenSocket, 5);
	/* ������ �Ͽ����� � Ŭ���̾�Ʈ�� �ּҷ� ������ �Ͽ������� �˾ƾ� �Ѵ�.
	�� Ŭ���̾�Ʈ �ּҸ� �޴� �����̴�. */
	SOCKADDR_IN client_addr;
	while(true)
	{
		int addr_size = sizeof(client_addr);
		/* ����� ���� ������ accept���� ������ ������ ����Ѵ�.
		listen������ ���Ӹ��� ���� �����̴�. 
		������ ������ 1�� 1�̴�. 
		���� 5000 => 5000�� 1�� �ƴ϶� 1�� 1 ����� 5000���� �迭�� �̷���� �ִ°��̴�. */
		SOCKET client_socket = accept(listenSocket, (sockaddr*)&client_addr, &addr_size);
		while (1)
		{
			// Ŭ���̾�Ʈ�� ��Ű� �����.
			char messageBuffer[MAX_BUFFER];
			int receiveBytes = recv(client_socket, messageBuffer, MAX_BUFFER, 0);
			if (receiveBytes > 0)
			{
				// ���ڿ��� ���� �� �� �ֵ���
				messageBuffer[receiveBytes] = '\0';
				printf("TRACE - Receive message : %s (%d bytes)\n",
					messageBuffer, receiveBytes);
			}
			else
			{
				break;
			}
			int sendBytes = send(client_socket, messageBuffer, receiveBytes, 0);
			if (sendBytes > 0)
			{
				printf("TRACE - Send message : %s (%d bytes)\n", messageBuffer, sendBytes);
			}
		}
		closesocket(client_socket);
	}
	closesocket(listenSocket);
	WSACleanup();
}