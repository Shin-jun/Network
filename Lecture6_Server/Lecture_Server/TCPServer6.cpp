#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ws2tcpip.h> // WSAStringtoAddress �Լ�

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

	// ���� ���� --> ���� ���� ����
	SOCKET listen_sock = socket(AF_INET6, SOCK_STREAM, 0);
	// AF_INET6 : IPv6, SOCK_STREAM : TCP
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// 2�ܰ� : bind() : ���� ���� ip, ��Ʈ ����
	SOCKADDR_IN6 serveraddr;		// ���� �ּ� ����ü ����
	ZeroMemory(&serveraddr, sizeof(serveraddr));	// ���� �ּ� ������ 0���� �ʱ�ȭ
	serveraddr.sin6_family = AF_INET6;				// �ּ� ü��
	serveraddr.sin6_addr = in6addr_any;
	// in6addr_any : Ŭ���̾�Ʈ ip�ּҸ� ����̵� ����
	serveraddr.sin6_port = htons(SERVERPORT);

	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// 3�ܰ� : listen() : ���� ������ ���¸� "���"���·� ��ȯ
	retval = listen(listen_sock, SOMAXCONN);	// SOMAXCONN : �ִ밪
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// ����� ���� �غ� �۾�
	SOCKET client_sock;			// ����ɶ� ������ ���� ����
	SOCKADDR_IN6 clientaddr;		// Ŭ���̾�Ʈ ip, ��Ʈ (����)
	int addrlen;		// �ּ� ����
	char buf[BUFSIZE + 1];		// ���� ���α׷����� �۽�, ���� ���ۿ� �ִ� ���� �����ؼ� ���� ����

	while (1) {

		// 4�ܰ� : accept()�Լ� --> Ŭ���̾�Ʈ�� connect�Լ��� ����
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}


		// ������ Ŭ���̾�Ʈ ���� ���
		char ipaddr[50];
		DWORD ipaddrlen = sizeof(ipaddr);
		WSAAddressToString((SOCKADDR*)&clientaddr, sizeof(clientaddr), NULL, ipaddr, &ipaddrlen);
		// ���ڸ� ���ڷ� ��ȯ
		printf("\n[TCP����] Ŭ���̾�Ʈ ���� : IP�ּ�=%s, ��Ʈ��ȣ=%d\n", ipaddr, ntohs(clientaddr.sin6_port));


		// Ŭ���̾�Ʈ���� ������ ���
		while (1) {
			// 5�ܰ� : ����
			retval = recv(client_sock, buf, BUFSIZE, 0);
			// ��������, ���Ź��ۿ� �ִ� ���� �����ؼ� ������ �������α׷� ����, ũ��, ��������(0)
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;

			// ���� ������ ���
			buf[retval] = '\0';
			printf("[TCP/%s:%d] %s\n", ipaddr, ntohs(clientaddr.sin6_port), buf);

			// 6�ܰ� : ������ ������(�۽�)
			retval = send(client_sock, buf, retval, 0);
			// �������α׷��ȿ� buf������ �۽� ���ۿ� �����ؼ� ����
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}// ������ ����� ������

		// closesocket 7�ܰ� : ���� ����
		closesocket(client_sock);
		printf("[TCP����] Ŭ���̾�Ʈ ����:IP�ּ� =%s, ��Ʈ��ȣ = %d\n", ipaddr, ntohs(clientaddr.sin6_port));
	}

	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();

	
	return 0;


}