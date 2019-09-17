#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

//#define TCP
#define TCP_IPV6
//#define UDP
//#define UDP_IPV6

// 소켓 함수 오류 출력 후 종료
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
	// 윈속 초기화
	WSADATA wsa;

	// MAKEWORD -> 2.2에 대응하는 unsigned short형식의 값을 만듬
	// MAKEWORD(a, b) -> a : 하위 8비트 데이터, b : 상위 8비트 데이터
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}
	MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);

	// socket()
	// TCP
#ifdef TCP
	SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (tcp_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}
	MessageBox(NULL, "TCP 소켓 생성 성공", "알림", MB_OK);
#endif

	// IPv6 기반 프로토콜 TCP
#ifdef TCP_IPV6
	SOCKET tcp_sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (tcp_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}
	MessageBox(NULL, "TCP 소켓(IPv6) 생성 성공", "알림", MB_OK);
#endif

	// UDP
#ifdef UDP
	SOCKET udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}
	MessageBox(NULL, "UDP 소켓 생성 성공", "알림", MB_OK);
#endif

	// IPv6 기반 프로토콜 UDP
#ifdef UDP_IPV6
	SOCKET udp_sock = socket(AF_INET6, SOCK_DGRAM, 0);
	if (udp_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}
	MessageBox(NULL, "UDP 소켓(IPv6) 생성 성공", "알림", MB_OK);
#endif
	// closesocket()
#if defined(TCP) || defined(TCP_IPV6)
	closesocket(tcp_sock);
#endif

#if defined(UDP) || defined(UDP_IPV6)
	closesocket(udp_sock);
#endif

	// 윈속 종료
	WSACleanup();
	return 0;
}