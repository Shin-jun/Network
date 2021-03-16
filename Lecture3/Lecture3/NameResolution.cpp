#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>
#define TESTNAME "www.naver.com"

// 함수정의
void err_display(const char *msg) {
	printf("오류");
}

// 도메인 이름 -> ip주소 변환 함수
bool GetIPAddr(const char *name, IN_ADDR *addr) {
	// name : 도메인이름
	// addr : 주소체계
	HOSTENT *ptr = gethostbyname(name);
	if (ptr == NULL) {
		err_display("gethostbyname()");
		return false;
	}
	if (ptr->h_addrtype != AF_INET) {
		// h_addr type : 주소타입
		// AF_INET : IPv4
		return false;
	}

	// memcpy(대상, 소스) : 바이트 복사
	memcpy(addr, ptr->h_addr, ptr->h_length);
	// h->addr 주소, h->length 길이
	return true;
}

// IP주소 --> 도메인 주소로 변환
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
	// strncpy(대상, 소스, 길이)
	// h_name : 공식 도메인 이름
	return true;
}

int main() {
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	printf("도메인 이름 변환전 = %s\n", TESTNAME);

	// 도메인이름 -> IP주소
	IN_ADDR addr;	// ip주소 저장 구조체 선언
	if (GetIPAddr(TESTNAME, &addr)) {
		// GetIPAddr(도메인이름, 주소체계)
		printf("IP주소 변환후 = %s\n", inet_ntoa(addr));	// inet_ntoa : 숫자 주소를 문자열로 출력, 
		// 네트워크 바이트 정렬 주소-> 숫자로된 문자열로 출력

		// ip주소를 도메인 이름으로
		char name[256];	//도메인 이름을 저장할 공간
		if (GetDomainName(addr, name, sizeof(name))) {
			// addr : 주소 체계
			// name : 도메인을 이름 저장
			// sizeof : 크기
			printf("도메인 이름 변환후 = %s\n", name);
		}

	}
	getchar();
	WSACleanup();
	return 0;
}