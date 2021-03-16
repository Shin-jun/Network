#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>
#define TESTNAME "www.naver.com"

// �Լ�����
void err_display(const char *msg) {
	printf("����");
}

// ������ �̸� -> ip�ּ� ��ȯ �Լ�
bool GetIPAddr(const char *name, IN_ADDR *addr) {
	// name : �������̸�
	// addr : �ּ�ü��
	HOSTENT *ptr = gethostbyname(name);
	if (ptr == NULL) {
		err_display("gethostbyname()");
		return false;
	}
	if (ptr->h_addrtype != AF_INET) {
		// h_addr type : �ּ�Ÿ��
		// AF_INET : IPv4
		return false;
	}

	// memcpy(���, �ҽ�) : ����Ʈ ����
	memcpy(addr, ptr->h_addr, ptr->h_length);
	// h->addr �ּ�, h->length ����
	return true;
}

// IP�ּ� --> ������ �ּҷ� ��ȯ
bool GetDomainName(IN_ADDR addr, char *name, int namelen) {
	HOSTENT *ptr = gethostbyaddr((char *)&addr, sizeof(addr), AF_INET);
	if (ptr == NULL) {
		err_display("gethostbyaddr()");
		return false;
	}
	
	if (ptr->h_addrtype != AF_INET) {
		return false;
	}

	strncpy(name, ptr->h_name, namelen);
	// strncpy(���, �ҽ�, ����)
	// h_name : ���� ������ �̸�
	return true;
}

int main() {
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	printf("������ �̸� ��ȯ�� = %s\n", TESTNAME);

	// �������̸� -> IP�ּ�
	IN_ADDR addr;	// ip�ּ� ���� ����ü ����
	if (GetIPAddr(TESTNAME, &addr)) {
		// GetIPAddr(�������̸�, �ּ�ü��)
		printf("IP�ּ� ��ȯ�� = %s\n", inet_ntoa(addr));	// inet_ntoa : ���� �ּҸ� ���ڿ��� ���, 
		// ��Ʈ��ũ ����Ʈ ���� �ּ�-> ���ڷε� ���ڿ��� ���

		// ip�ּҸ� ������ �̸�����
		char name[256];	//������ �̸��� ������ ����
		if (GetDomainName(addr, name, sizeof(name))) {
			// addr : �ּ� ü��
			// name : �������� �̸� ����
			// sizeof : ũ��
			printf("������ �̸� ��ȯ�� = %s\n", name);
		}

	}
	getchar();
	WSACleanup();
	return 0;
}