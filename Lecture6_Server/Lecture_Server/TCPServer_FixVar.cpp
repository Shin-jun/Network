#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define SERVERPORT 9000
#define BUFSIZE 512

// ����+����
// �Լ� ���� --> ����
void err_quit(const char* msg) {
	exit(1);
}

// ���� �Լ� ���� ��� ����
void err_display(const char* msg) {
	printf("���� �޽���");
}

// ����� ���� --> recvn
int recvn(SOCKET s, char* buf, int len, int flags) {
	int received; // ���� ������
	char* ptr = buf; // ������ ������ġ
	int left = len; // ���� ������

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		else if (received == 0) {
			break;
		}
		left -= received;
		ptr += received;
	}
	return(len - left);
}


int main() {
	int retval;		// ���Ͽ� ������ ������ ����

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ���� --> ���� ���� ����
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	// AF_INET : IPv4, SOCK_STREAM : TCP
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

	// 3�ܰ� : listen() : ���� ������ ���¸� "���"���·� ��ȯ
	retval = listen(listen_sock, SOMAXCONN);	// SOMAXCONN : �ִ밪
	if (retval == SOCKET_ERROR) err_quit("listen()");


	// ����� ���� �غ� �۾�
	SOCKET client_sock;			// ����ɶ� ������ ���� ����
	SOCKADDR_IN clientaddr;		// Ŭ���̾�Ʈ ip, ��Ʈ (����)
	int addrlen;		// �ּ� ����
	char buf[BUFSIZE + 1];		// ���� ���α׷����� �۽�, ���� ���ۿ� �ִ� ���� �����ؼ� ���� ����

	int len;	// �������̸� ������ ����� ����

	while (1) {
		// 4�ܰ� : accept()�Լ� --> Ŭ���̾�Ʈ�� connect�Լ��� ����
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP����] Ŭ���̾�Ʈ ���� : IP�ּ�=%s, ��Ʈ��ȣ=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// Ŭ���̾�Ʈ���� ������ ���
		while (1) {
			// 5�ܰ� : ����
			
			// ���� ����(������ 1)
			retval = recvn(client_sock, (char*)&len, sizeof(int), 0);
			// ��������, ���Ź��ۿ� �ִ� ���� �����ؼ� ������ �������α׷� ����, ũ��, ��������(0)
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0) {
				break;
			}

			// ���� ����(������ 2)
			retval = recvn(client_sock, buf, len, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0) {
				break;
			}


			// ���� ������ ���
			buf[retval] = '\0';
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

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
		printf("[TCP����] Ŭ���̾�Ʈ ����:IP�ּ� =%s, ��Ʈ��ȣ = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();

	getchar();
	return 0;


}