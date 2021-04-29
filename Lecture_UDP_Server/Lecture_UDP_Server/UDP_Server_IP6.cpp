#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <WS2tcpip.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

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
	int retval;		// ���Ͽ� ������ ������ ����

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ���� --> ���� ���� ����(UDP)
	SOCKET listen_sock = socket(AF_INET6, SOCK_DGRAM, 0);
	// AF_INET6 : IPv6, SOCK_DGRAM : UDP
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// 2�ܰ� : bind() : ���� ���� ip, ��Ʈ ����
	SOCKADDR_IN6 serveraddr;		// ���� �ּ� ����ü ����
	ZeroMemory(&serveraddr, sizeof(serveraddr));	// ���� �ּ� ������ 0���� �ʱ�ȭ
	serveraddr.sin6_family = AF_INET6;				// �ּ� ü��
	serveraddr.sin6_addr = in6addr_any;
	// ip�ּ� , hton --> host -> network, INADDR_ANY : Ŭ���̾�Ʈ ip�ּҸ� ����̵� ����
	serveraddr.sin6_port = htons(SERVERPORT);

	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");


	// ����� ���� �غ� �۾�
	SOCKADDR_IN6 clientaddr;		// Ŭ���̾�Ʈ ip, ��Ʈ (����)
	int addrlen;		// �ּ� ����
	char buf[BUFSIZE + 1];		// ���� ���α׷����� �۽�, ���� ���ۿ� �ִ� ���� �����ؼ� ���� ����

	while (1) {
		// 4�ܰ� : accept()�Լ� --> Ŭ���̾�Ʈ�� connect�Լ��� ����==> UDP�� ����
		addrlen = sizeof(clientaddr);


		// Ŭ���̾�Ʈ���� ������ ���
		retval = recvfrom(listen_sock, buf, BUFSIZE, 0, (SOCKADDR*)&clientaddr, &addrlen);
		// recvfrom(����, ����, ����ũ��, 0, �ּ�, �ּұ���)
		// ��������, ���Ź��ۿ� �ִ� ���� �����ؼ� ������ �������α׷� ����, ũ��, ��������(0)
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			continue;
		}

		// IP�ּ� ��ȯ �Լ�
		char ipaddr[50];
		DWORD ipaddrlen = sizeof(ipaddr);
		WSAAddressToString((SOCKADDR*)&clientaddr, sizeof(clientaddr), NULL, ipaddr, &ipaddrlen);
		// IPv6 ���� -> ���ڿ���
		
		// ���� ������ ���
		buf[retval] = '\0';
		printf("[UDP/%s] %s\n", ipaddr, buf);

		// 6�ܰ� : ������ ������(�۽�)
		retval = sendto(listen_sock, buf, retval, 0, (SOCKADDR*)&clientaddr, sizeof(clientaddr));
		// �������α׷��ȿ� buf������ �۽� ���ۿ� �����ؼ� ����
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			continue;
		}
	}// ������ ����� ������


// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();

	getchar();
	return 0;


}