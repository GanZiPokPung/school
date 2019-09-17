#define _CRT_SECURE_NO_WARNINGS         // 최신 VC++ 컴파일 시 경고 방지
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지

#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	// WINSOCK 초기화
	WSADATA wsa;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		return 1;
	}

	// WinSock ver
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == SOCKET_ERROR)
	{
		return 1;
	}
	
	u_short x1 = 0x1234;
	u_long  y1 = 0x12345678;
	u_short x2, x3, x4;
	u_long  y2, y3, y4;

	printf("[호스트 바이트 -> 네트워크 바이트]\n");
	WSAHtons(s, x1, &x2);
	printf("0x%x -> 0x%x\n", x1, x2);
	WSAHtonl(s, y1, &y2);
	printf("0x%x -> 0x%x\n\n", y1, y2);

	printf("[네트워크 바이트 -> 호스트 바이트]\n");
	WSANtohs(s, x2, &x3);
	printf("0x%x -> 0x%x\n", x2, x3);
	WSANtohl(s, y2, &y3);
	printf("0x%x -> 0x%x\n\n", y2, y3);

	// 쓰는게 불가능.
	//printf("[잘못된 사용 예]\n");
	//WSAHtonl(s, x1, &x4);
	//printf("0x%x -> 0x%x\n", x1, x4);

	// UNIX ver
	/*u_short x1 = 0x1234;
	u_long  y1 = 0x12345678;
	u_short x2;
	u_long  y2;

	// 리틀 엔디안
	printf("[호스트 바이트 -> 네트워크 바이트]\n");
	printf("0x%x -> 0x%x\n", x1, x2 = htons(x1));
	printf("0x%x -> 0x%x\n\n", y1, y2 = htonl(y1));

	// 빅 엔디안
	printf("[네트워크 바이트 -> 호스트 바이트]\n");
	printf("0x%x -> 0x%x\n", x2, ntohs(x2));
	printf("0x%x -> 0x%x\n\n", y2, ntohl(y2));

	// 잘못된 사용 예
	printf("[잘못된 사용 예]\n");
	printf("0x%x -> 0x%x\n", x1, htonl(x1));*/

	WSACleanup();
	return 0;
}