#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

//#define TCP
#define TCP_IPV6
//#define UDP
//#define UDP_IPV6

// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

int main(int argc, char *argv[])
{
	// ���� �ʱ�ȭ
	WSADATA wsa;

	// MAKEWORD -> 2.2�� �����ϴ� unsigned short������ ���� ����
	// MAKEWORD(a, b) -> a : ���� 8��Ʈ ������, b : ���� 8��Ʈ ������
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}
	MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);

	// socket()
	// TCP
#ifdef TCP
	SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (tcp_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}
	MessageBox(NULL, "TCP ���� ���� ����", "�˸�", MB_OK);
#endif

	// IPv6 ��� �������� TCP
#ifdef TCP_IPV6
	SOCKET tcp_sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (tcp_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}
	MessageBox(NULL, "TCP ����(IPv6) ���� ����", "�˸�", MB_OK);
#endif

	// UDP
#ifdef UDP
	SOCKET udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}
	MessageBox(NULL, "UDP ���� ���� ����", "�˸�", MB_OK);
#endif

	// IPv6 ��� �������� UDP
#ifdef UDP_IPV6
	SOCKET udp_sock = socket(AF_INET6, SOCK_DGRAM, 0);
	if (udp_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}
	MessageBox(NULL, "UDP ����(IPv6) ���� ����", "�˸�", MB_OK);
#endif
	// closesocket()
#if defined(TCP) || defined(TCP_IPV6)
	closesocket(tcp_sock);
#endif

#if defined(UDP) || defined(UDP_IPV6)
	closesocket(udp_sock);
#endif

	// ���� ����
	WSACleanup();
	return 0;
}