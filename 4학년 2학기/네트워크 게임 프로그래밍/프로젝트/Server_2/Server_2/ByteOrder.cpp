#define _CRT_SECURE_NO_WARNINGS         // �ֽ� VC++ ������ �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����

#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	// WINSOCK �ʱ�ȭ
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

	printf("[ȣ��Ʈ ����Ʈ -> ��Ʈ��ũ ����Ʈ]\n");
	WSAHtons(s, x1, &x2);
	printf("0x%x -> 0x%x\n", x1, x2);
	WSAHtonl(s, y1, &y2);
	printf("0x%x -> 0x%x\n\n", y1, y2);

	printf("[��Ʈ��ũ ����Ʈ -> ȣ��Ʈ ����Ʈ]\n");
	WSANtohs(s, x2, &x3);
	printf("0x%x -> 0x%x\n", x2, x3);
	WSANtohl(s, y2, &y3);
	printf("0x%x -> 0x%x\n\n", y2, y3);

	// ���°� �Ұ���.
	//printf("[�߸��� ��� ��]\n");
	//WSAHtonl(s, x1, &x4);
	//printf("0x%x -> 0x%x\n", x1, x4);

	// UNIX ver
	/*u_short x1 = 0x1234;
	u_long  y1 = 0x12345678;
	u_short x2;
	u_long  y2;

	// ��Ʋ �����
	printf("[ȣ��Ʈ ����Ʈ -> ��Ʈ��ũ ����Ʈ]\n");
	printf("0x%x -> 0x%x\n", x1, x2 = htons(x1));
	printf("0x%x -> 0x%x\n\n", y1, y2 = htonl(y1));

	// �� �����
	printf("[��Ʈ��ũ ����Ʈ -> ȣ��Ʈ ����Ʈ]\n");
	printf("0x%x -> 0x%x\n", x2, ntohs(x2));
	printf("0x%x -> 0x%x\n\n", y2, ntohl(y2));

	// �߸��� ��� ��
	printf("[�߸��� ��� ��]\n");
	printf("0x%x -> 0x%x\n", x1, htonl(x1));*/

	WSACleanup();
	return 0;
}