#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>

int main() {
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성시 윈속에서 제공하는 함수 사용
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

	u_short x1 = 0x1234;
	u_long y1 = 0x12345678;

	u_short x2, x3;
	u_long y2, y3;

	printf("호스트바이트 -> 네트워크 바이트\n");
	WSAHtons(s, x1, &x2);	// htons동일한 기능
	printf("0x%x --> 0x%x\n", x1, x2);
	WSAHtonl(s, y1, &y2);
	printf("0x%x --> 0x%x\n", y1, y2);

	printf("네트워크바이트 -> 호스트 바이트\n");
	WSANtohs(s, x2, &x3);
	printf("0x%x --> 0x%x\n", x2, x3);
	WSANtohl(s, y2, &y3);
	printf("0x%x --> 0x%x\n", y2, y3);


	WSACleanup();
	getchar();
	return 0;
}