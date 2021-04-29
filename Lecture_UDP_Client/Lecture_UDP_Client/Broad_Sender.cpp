#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#define SERVERIP "255.255.255.255"
#define SERVERPORT 9000
#define BUFSIZE 512

// 함수 정의 --> 종료
void err_quit(const char* msg) {
	exit(1);
}

// 소켓 함수 오류 출력 정의
void err_display(const char* msg) {
	printf("오류 메시지");
}


int main() {
	int retval;	// 소켓 성공시 처리되는 정보

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 1단계 : 소켓 생성 --> UDP로 변경
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	// AF_INET : IPv4, SOCK_DGRAM : UDP
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// 소켓옵션 ==> 브로드 캐스팅옵션 추가
	BOOL bEnable = TRUE;
	retval = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&bEnable, sizeof(bEnable));
	if (retval == SOCKET_ERROR) err_quit("setsockopt()");


	// 2단계 : 소켓 주소구조체 정보
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));	// 소켓 주소 구조체의 초기화
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);	// 서버 연결 주소
	serveraddr.sin_port = htons(SERVERPORT);


	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE + 1];	// 응용 프로그램에서 사용할 버퍼
	int len;	// 보내거나 받을 문자의 길이

	// 서버와의 통신
	while (1) {
		printf("\n[보낼 데이터]");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;

		// TCP 프로토콜의 데이터의 끝이 정해지지 않아서 서버와 클라이언트와 약속
		// \n을 만나면 문자를 제거 -> 맨끝을 0으로 체크
		len = strlen(buf);	// 버퍼의 길이 정보
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;

		// 데이터 보내기(3단계)
		retval = sendto(sock, buf, strlen(buf), 0, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) {
			err_display("send");
			continue;
		}
		printf("[UDP클라이언트] %d바이트 보냈습니다.\n", retval);

	}

	// 소켓 닫기(5단계)
	closesocket(sock);
	// 윈속 종료
	WSACleanup();

	getchar();
	return 0;
}