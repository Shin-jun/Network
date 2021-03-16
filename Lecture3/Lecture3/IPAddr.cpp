#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdio.h>

int main_IPAddr() {
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ip주소 변환
	const char *ipv4test = "147.46.114.70";
	printf("IP주소 변환전 : =%s\n", ipv4test);
	
	// inet_addr() : 문자열 -> 숫자
	printf("ipv4주소 변환 후= 0x%x\n", inet_addr(ipv4test));

	// inet_ntoa() : 숫자 -> 문자열
	IN_ADDR ipv4num;
	ipv4num.s_addr = inet_addr(ipv4test);	// 문자열 -> 숫자
	printf("IP주소 다시 변환 = %s\n", inet_ntoa(ipv4num));
	printf("\n");

	// ipv6
	const char *ipv6test = "2001:0230:abcd:ffab:0023:eb00:ffff:1111";
	printf("IPv6주소 변환 전 = %s\n", ipv6test);

	// WSAStringtoAddress() : 원속 제공함수 문자열->숫자
	SOCKADDR_IN6 ipv6num;	// IPv6주소 구조체 선언
	int addrlen = sizeof(ipv6num);	// 주소체계의 크기
	WSAStringToAddress((LPSTR)ipv6test, AF_INET6, NULL, (SOCKADDR *)&ipv6num, &addrlen);	// 문자열 주소, 주소체계, null, 저장할 숫자, 저장할 숫자주소 길이
	printf("IPV6 주소 변환 후 = 0x");

	for (int i = 0;i < 16; i++) {
		printf("%02x", ipv6num.sin6_addr.u.Byte[i]);
	}
	printf("\n");

	// wsaaddresstostirng : 숫자 -> 문자열
	char ipaddr[50];	// 문자열 주소 저장 공간
	DWORD ipaddrlen = sizeof(ipaddr);
	WSAAddressToString((SOCKADDR *)&ipv6num, sizeof(ipv6num), NULL, ipaddr, &ipaddrlen);	// 숫자주소, 길이, null, 문자열 주소공간, 길이
	printf("IPv6주소 변환후 =%s\n", ipaddr);

	getchar();

	// 윈속 종료
	WSACleanup();
	return 0;

}