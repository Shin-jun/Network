#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define SERVERIP "127.0.0.1"
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
	const char* chatid = "Ŭ���̾�Ʈ-�ſ���1";

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
	char msg[BUFSIZE + 1];
	char* buf;
	int len;

	sprintf(msg, "[%s]", chatid);
	buf = msg + strlen(msg);

	while(1){

		// ä�ø޽��� ���
		printf("[%s]", chatid);
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;
		// ���� ����
		len = strlen(msg);
		if(msg[len-1]=='\n')
			msg[len-1] = '\0';
		if (strlen(buf) == 0)
			break;

		// ������ ������(3�ܰ�)
		// ���� ���� ������ (4byte)
		retval = send(sock, (char*)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send");
			break;
		}
		// ���� ���� ������
		retval = send(sock, msg, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send");
			break;
		}

		// �����͹ޱ�
		// ���� ���� �ޱ�
		retval = recvn(sock, (char*)&len, sizeof(int), 0);

		// ��������, ���Ź��ۿ� �ִ� ���� �����ؼ� ������ �������α׷� ����, ũ��, ��������(0)
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0) {
			break;
		}

		// ���� ���� �ޱ�
		retval = recvn(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0) {
			break;
		}
		// ���� ������ ���
		buf[retval] = '\0';
		printf("%s\n", buf);
	}

	// ���� �ݱ�(5�ܰ�)
	closesocket(sock);
	// ���� ����
	WSACleanup();

	return 0;
}