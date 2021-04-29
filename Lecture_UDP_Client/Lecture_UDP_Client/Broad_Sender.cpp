#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#define SERVERIP "255.255.255.255"
#define SERVERPORT 9000
#define BUFSIZE 512

// �Լ� ���� --> ����
void err_quit(const char* msg) {
	exit(1);
}

// ���� �Լ� ���� ��� ����
void err_display(const char* msg) {
	printf("���� �޽���");
}


int main() {
	int retval;	// ���� ������ ó���Ǵ� ����

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 1�ܰ� : ���� ���� --> UDP�� ����
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	// AF_INET : IPv4, SOCK_DGRAM : UDP
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// ���Ͽɼ� ==> ��ε� ĳ���ÿɼ� �߰�
	BOOL bEnable = TRUE;
	retval = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&bEnable, sizeof(bEnable));
	if (retval == SOCKET_ERROR) err_quit("setsockopt()");


	// 2�ܰ� : ���� �ּұ���ü ����
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));	// ���� �ּ� ����ü�� �ʱ�ȭ
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);	// ���� ���� �ּ�
	serveraddr.sin_port = htons(SERVERPORT);


	// ������ ��ſ� ����� ����
	char buf[BUFSIZE + 1];	// ���� ���α׷����� ����� ����
	int len;	// �����ų� ���� ������ ����

	// �������� ���
	while (1) {
		printf("\n[���� ������]");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;

		// TCP ���������� �������� ���� �������� �ʾƼ� ������ Ŭ���̾�Ʈ�� ���
		// \n�� ������ ���ڸ� ���� -> �ǳ��� 0���� üũ
		len = strlen(buf);	// ������ ���� ����
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;

		// ������ ������(3�ܰ�)
		retval = sendto(sock, buf, strlen(buf), 0, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) {
			err_display("send");
			continue;
		}
		printf("[UDPŬ���̾�Ʈ] %d����Ʈ ���½��ϴ�.\n", retval);

	}

	// ���� �ݱ�(5�ܰ�)
	closesocket(sock);
	// ���� ����
	WSACleanup();

	getchar();
	return 0;
}