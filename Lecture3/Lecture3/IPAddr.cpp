#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdio.h>

int main_IPAddr() {
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ip�ּ� ��ȯ
	const char *ipv4test = "147.46.114.70";
	printf("IP�ּ� ��ȯ�� : =%s\n", ipv4test);
	
	// inet_addr() : ���ڿ� -> ����
	printf("ipv4�ּ� ��ȯ ��= 0x%x\n", inet_addr(ipv4test));

	// inet_ntoa() : ���� -> ���ڿ�
	IN_ADDR ipv4num;
	ipv4num.s_addr = inet_addr(ipv4test);	// ���ڿ� -> ����
	printf("IP�ּ� �ٽ� ��ȯ = %s\n", inet_ntoa(ipv4num));
	printf("\n");

	// ipv6
	const char *ipv6test = "2001:0230:abcd:ffab:0023:eb00:ffff:1111";
	printf("IPv6�ּ� ��ȯ �� = %s\n", ipv6test);

	// WSAStringtoAddress() : ���� �����Լ� ���ڿ�->����
	SOCKADDR_IN6 ipv6num;	// IPv6�ּ� ����ü ����
	int addrlen = sizeof(ipv6num);	// �ּ�ü���� ũ��
	WSAStringToAddress((LPSTR)ipv6test, AF_INET6, NULL, (SOCKADDR *)&ipv6num, &addrlen);	// ���ڿ� �ּ�, �ּ�ü��, null, ������ ����, ������ �����ּ� ����
	printf("IPV6 �ּ� ��ȯ �� = 0x");

	for (int i = 0;i < 16; i++) {
		printf("%02x", ipv6num.sin6_addr.u.Byte[i]);
	}
	printf("\n");

	// wsaaddresstostirng : ���� -> ���ڿ�
	char ipaddr[50];	// ���ڿ� �ּ� ���� ����
	DWORD ipaddrlen = sizeof(ipaddr);
	WSAAddressToString((SOCKADDR *)&ipv6num, sizeof(ipv6num), NULL, ipaddr, &ipaddrlen);	// �����ּ�, ����, null, ���ڿ� �ּҰ���, ����
	printf("IPv6�ּ� ��ȯ�� =%s\n", ipaddr);

	getchar();

	// ���� ����
	WSACleanup();
	return 0;

}