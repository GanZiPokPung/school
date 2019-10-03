#include <stdio.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

// 서버는 주소를 알 필요가 없기 때문에 가지지 않는다.
// 그냥 받은 주소를 이용해 통신하면 되기 때문이다.

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
	/* 모든 주소를 받겠다. 
	이 주소에서 오는것만 받겠다. (클라이언트 주소만) 
	보안강화를 위해서 
	같은 LAN혹은 전체 인터넷 혹은 내 컴퓨터에서만 오는 
	클라이언트 패킷만 받겠다. */
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	/* 밖에서 오는 포트 번호로 오는 주소를 모두 받겠다고 
	운영체제에 등록하는 작업을 하는 코드 (포트 주소 선점) 
	::을 붙이는 이유는 bind가 C++11의 키워드(함수포인터의 함수값을 넣어주는 기능)
	이기 때문에 그것을 피하려는 의도이다. */
	::bind(listenSocket, (sockaddr *)&serverAddr, sizeof(serverAddr));
	/* 선점한 포트 주소를 받으라고 운영체제에게 요청하는 작업 
	두번째 인자 Backlog : 대기열의 길이이다. (몇개의 클라이언트까지 대기를 할것인가?) 
	MMORPG같은 경우에는 보통은 MAX값을 넣어준다. */
	listen(listenSocket, 5);
	/* 접속을 하였을때 어떤 클라이언트의 주소로 접속을 하였는지를 알아야 한다.
	그 클라이언트 주소를 받는 변수이다. */
	SOCKADDR_IN client_addr;
	while(true)
	{
		int addr_size = sizeof(client_addr);
		/* 통신을 위한 소켓은 accept에서 리턴한 소켓을 사용한다.
		listen소켓은 접속만을 위한 소켓이다. 
		소켓은 무조건 1대 1이다. 
		동접 5000 => 5000대 1이 아니라 1대 1 통신이 5000개의 배열로 이루어져 있는것이다. */
		SOCKET client_socket = accept(listenSocket, (sockaddr*)&client_addr, &addr_size);
		while (1)
		{
			// 클라이언트쪽 통신과 비슷함.
			char messageBuffer[MAX_BUFFER];
			int receiveBytes = recv(client_socket, messageBuffer, MAX_BUFFER, 0);
			if (receiveBytes > 0)
			{
				// 문자열의 끝을 알 수 있도록
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