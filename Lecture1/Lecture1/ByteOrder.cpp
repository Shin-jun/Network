#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>

int main_byte() {
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	u_short x1 = 0x1234;
	u_long y1 = 0x12345678;

	u_short x2;
	u_long y2;

	printf("호스트바이트 -> 네트워크 바이트\n");
	printf("0x%x --> 0x%x\n", x1, x2 = htons(x1));
	printf("0x%x --> 0x%x\n", y1, y2 = htonl(y1));

	printf("네트워크바이트 -> 호스트 바이트\n");
	printf("0x%x --> 0x%x\n", x2, ntohs(x2));
	printf("0x%x --> 0x%x\n", y2, ntohl(y2));


	WSACleanup();
	getchar();
	return 0;
}