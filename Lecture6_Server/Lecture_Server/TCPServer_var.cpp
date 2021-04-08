#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define SERVERPORT 9000
#define BUFSIZE 512

// �������� ����������
// �Լ� ���� --> ����
void err_quit(const char* msg) {
	exit(1);
}

// ���� �Լ� ���� ��� ����
void err_display(const char* msg) {
	printf("���� �޽���");
}

// �Լ� : ���� ���ۿ� �� ������ �����͸� 1byte�� �а� ������ �ִ� �Լ�
int _recv_head(SOCKET s, char* p) {
	// ���� ����
	static int nbytes = 0;	// ���� �������� ����Ʈ��
	static char buf[1024];	// ���� ����
	static char* ptr;	// ���� ���� ��ġ

	if (nbytes == 0 || nbytes == SOCKET_ERROR) {
		nbytes = recv(s, buf, sizeof(buf), 0);	// ���� ���ۿ� �ִ� ������ �о����
		if (nbytes == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		else if (nbytes == 0) {
			return 0;
		}
		ptr = buf;
	}
	--nbytes;
	*p = *ptr++;
	return 1;
}

// �Լ� : recvline() : �����͸� \n ��ŭ �о ���� ���ۿ� �ֱ�
int recvline(SOCKET s, char* buf, int maxlen) {
	int n, nbytes;
	char c, * ptr = buf;
	for (n = 1; n < maxlen; n++) {
		nbytes = _recv_head(s, &c);
		if (nbytes == 1) {
			*ptr++ = c;
			if (c == '\n') {
				break;
			}
		}
		else if (nbytes == 0) {
			*ptr = 0;
			return n - 1;
		}
		else
			return SOCKET_ERROR;
	}
	*ptr = 0;
	return n;
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


			// ���� ����(������ 2)
			retval = recvline(client_sock, buf, BUFSIZE+1);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0) {
				break;
			}


			// ���� ������ ���
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

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