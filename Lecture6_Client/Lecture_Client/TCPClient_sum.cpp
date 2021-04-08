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
	char buf[BUFSIZE+1];	// ���� ���α׷����� ����� ����
	int len;
	struct Mydata {
		int n1;
		int n2;
	}data;

	int result;
	int dummy;

	while(1){

		printf("\n���� 2���� ����� �Է� : ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;

		// ���� ����
		len = strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;

		if (sscanf(buf, "%d%d%d", &data.n1, &data.n2, &dummy) != 2) {
			fprintf(stderr, "[����]���� 2���� �Է��ϼ���!\n");
			continue;
		}

		// ������ ������(3�ܰ�)
		// ���� ���� ������
		retval = send(sock, (char *)&data, sizeof(data), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send");
			break;
		}
		// ������ �ޱ�
		retval = recvn(sock, (char*)&result, sizeof(result), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send");
			break;
		}
		else if (retval == 9) {
			break;
		}
		
		// ���������� ���
		printf("����� %d + %d = %d\n", data.n1, data.n2, result);
	}

	// ���� �ݱ�(5�ܰ�)
	closesocket(sock);
	// ���� ����
	WSACleanup();

	return 0;
}