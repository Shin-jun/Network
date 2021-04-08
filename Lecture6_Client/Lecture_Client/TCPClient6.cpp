#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <WS2tcpip.h>	// wsa관련함수

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define SERVERIP "::1"	// IPv6 루프백주소
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

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char* buf, int len, int flags) {
	int received;		// 수신된 정보
	char* ptr = buf;	// 버퍼의 시작 위치
	int left = len;		// 읽을 데이터의 길이

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;	// 읽을 때 마다 하나씩 감소
		ptr += received;	// 읽을 위치값을 설정
	}

	return (len - left);

}

int main() {
	int retval;	// 소켓 성공시 처리되는 정보

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;


	// 1단계 : 소켓 생성 --> 클라이언트 소켓 생성
	SOCKET sock = socket(AF_INET6, SOCK_STREAM, 0);
	// AF_INET6 : IPv6, SOCK_STREAM : TCP
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// 2단계 : connect() --> 소켓 주소구조체, 서버와의 연결 시도
	SOCKADDR_IN6 serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));	// 소켓 주소 구조체의 초기화
	serveraddr.sin6_family = AF_INET6;
	int addrlen = sizeof(serveraddr);
	WSAStringToAddress((LPSTR)SERVERIP, AF_INET6, NULL, (SOCKADDR*)&serveraddr, &addrlen);
	serveraddr.sin6_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect");


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
		retval = send(sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send");
			break;
		}
		printf("[TCP클라이언트] %d바이트 보냈습니다.\n", retval);

		// 데이터 받기(4단계)
		retval = recvn(sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send");
			break;
		}
		else if (retval == 0)
			break;

		// 받은 데이터 출력
		buf[retval] = '\0';
		printf("[TCP 클라이언트] %d바이트 받았습니다.\n", retval);
		printf("[받은데이터] %s\n", buf);
	}

	// 소켓 닫기(5단계)
	closesocket(sock);
	// 윈속 종료
	WSACleanup();

	
	return 0;
}