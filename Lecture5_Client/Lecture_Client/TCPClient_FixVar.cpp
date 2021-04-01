#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 50

// �Լ� ���� --> ����
void err_quit(const char* msg) {
	exit(1);
}

// ���� �Լ� ���� ��� ����
void err_display(const char* msg) {
	printf("���� �޽���");
}

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char* buf, int len, int flags) {
	int received;		// ���ŵ� ����
	char* ptr = buf;	// ������ ���� ��ġ
	int left = len;		// ���� �������� ����

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;	// ���� �� ���� �ϳ��� ����
		ptr += received;	// ���� ��ġ���� ����
	}

	return (len - left);

}

int main() {
	int retval;	// ���� ������ ó���Ǵ� ����

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 1�ܰ� : ���� ���� --> Ŭ���̾�Ʈ ���� ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	// AF_INET : IPv4, SOCK_STREAM : TCP
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// 2�ܰ� : connect() --> ���� �ּұ���ü
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));	// ���� �ּ� ����ü�� �ʱ�ȭ
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);	// ���� ���� �ּ�
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect");


	// ������ ��ſ� ����� ����
	char buf[BUFSIZE];	// ���� ���α׷����� ����� ����
	const char* testdata[] = {
		"�ȳ��ϼ���",
		"�ݰ�����",
		"���õ���.....",
		"���� �׷��׿�"
	};

	int len;
	// �������� ���
	for (int i = 0; i < 4; i++) {
	//	memset(buf, '#', sizeof(buf));
		len = strlen(testdata[i]);
		strncpy(buf, testdata[i], len);

		// ������ ������(3�ܰ�)
		// ���� ���� ������
		retval = send(sock, (char*)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send");
			break;
		}
		// ���� ���� ������
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send");
			break;
		}
		printf("[TCPŬ���̾�Ʈ] %d + %d����Ʈ ���½��ϴ�.\n", sizeof(int), retval);

	}

	// ���� �ݱ�(5�ܰ�)
	closesocket(sock);
	// ���� ����
	WSACleanup();

	return 0;
}