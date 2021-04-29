#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <WS2tcpip.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#define SERVERIP "::1"
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
	SOCKET sock = socket(AF_INET6, SOCK_DGRAM, 0);
	// AF_INET : IPv6, SOCK_DGRAM : UDP
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// 2�ܰ� : ���� �ּұ���ü ����
	SOCKADDR_IN6 serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));	// ���� �ּ� ����ü�� �ʱ�ȭ
	serveraddr.sin6_family = AF_INET6;
	int addrlen = sizeof(serveraddr);
	WSAStringToAddress((LPSTR)SERVERIP, AF_INET6, NULL, (SOCKADDR*)&serveraddr, &addrlen);
	// ���ڿ� �ּ� ==> ���ڷ� ��ȯ
	serveraddr.sin6_port = htons(SERVERPORT);


	// ������ ��ſ� ����� ����
	SOCKADDR_IN6 peeraddr;	// �ڱ� �ڽ��� �ּ�
	//int addrlen;	// �ּұ��� ����
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
		printf("[TCPŬ���̾�Ʈ] %d����Ʈ ���½��ϴ�.\n", retval);

		// ������ �ޱ�(4�ܰ�)
		addrlen = sizeof(peeraddr);
		retval = recvfrom(sock, buf, retval, 0, (SOCKADDR*)&peeraddr, &addrlen);
		if (retval == SOCKET_ERROR) {
			err_display("recv");
			continue;
		}


		// �۽� ip�ּ� üũ
		if (memcmp(&peeraddr, &serveraddr, sizeof(peeraddr))) {
			printf("����");
			continue;
		}

		// ���� ������ ���
		buf[retval] = '\0';
		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ �޾ҽ��ϴ�.\n", retval);
		printf("[����������] %s\n", buf);
	}

	// ���� �ݱ�(5�ܰ�)
	closesocket(sock);
	// ���� ����
	WSACleanup();

	getchar();
	return 0;
}