#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>

int main() {
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ������ ���ӿ��� �����ϴ� �Լ� ���
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

	u_short x1 = 0x1234;
	u_long y1 = 0x12345678;

	u_short x2, x3;
	u_long y2, y3;

	printf("ȣ��Ʈ����Ʈ -> ��Ʈ��ũ ����Ʈ\n");
	WSAHtons(s, x1, &x2);	// htons������ ���
	printf("0x%x --> 0x%x\n", x1, x2);
	WSAHtonl(s, y1, &y2);
	printf("0x%x --> 0x%x\n", y1, y2);

	printf("��Ʈ��ũ����Ʈ -> ȣ��Ʈ ����Ʈ\n");
	WSANtohs(s, x2, &x3);
	printf("0x%x --> 0x%x\n", x2, x3);
	WSANtohl(s, y2, &y3);
	printf("0x%x --> 0x%x\n", y2, y3);


	WSACleanup();
	getchar();
	return 0;
}