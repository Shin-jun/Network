#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>

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
	SOCKET listen_sock = socket(AF_INET, SOCK_DGRAM, 0);
	// AF_INET : IPv4, SOCK_DGRAM : UDP
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// 2�ܰ� : bind() : ���� ���� ip, ��Ʈ ����
	SOCKADDR_IN serveraddr;		// ���� �ּ� ����ü ����
	ZeroMemory(&serveraddr, sizeof(serveraddr));	// ���� �ּ� ������ 0���� �ʱ�ȭ
	serveraddr.sin_family = AF_INET;				// �ּ� ü��
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// ip�ּ� , hton --> host -> network, INADDR_ANY : Ŭ���̾�Ʈ ip�ּҸ� ����̵� ����
	serveraddr.sin_port = htons(SERVERPORT);

	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");


	// ����� ���� �غ� �۾�
	SOCKADDR_IN clientaddr;		// Ŭ���̾�Ʈ ip, ��Ʈ (����)
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


		// ���� ������ ���
		buf[retval] = '\0';
		printf("[UDP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

		
	}// ������ ����� ������


// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();

	getchar();
	return 0;


}