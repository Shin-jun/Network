#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

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
	int retval;		// 소켓에 정보를 저장할 변수

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성 --> 서버 소켓 생성(UDP)
	SOCKET listen_sock = socket(AF_INET, SOCK_DGRAM, 0);
	// AF_INET : IPv4, SOCK_DGRAM : UDP
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// 2단계 : bind() : 서버 지역 ip, 포트 설정
	SOCKADDR_IN serveraddr;		// 소켓 주소 구조체 선언
	ZeroMemory(&serveraddr, sizeof(serveraddr));	// 서버 주소 공간을 0으로 초기화
	serveraddr.sin_family = AF_INET;				// 주소 체계
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// ip주소 , hton --> host -> network, INADDR_ANY : 클라이언트 ip주소를 어떤것이든 수용
	serveraddr.sin_port = htons(SERVERPORT);

	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");


	// 통신을 위한 준비 작업
	SOCKADDR_IN clientaddr;		// 클라이언트 ip, 포트 (원격)
	int addrlen;		// 주소 길이
	char buf[BUFSIZE + 1];		// 응용 프로그램에서 송신, 수신 버퍼에 있는 값을 복사해서 넣을 공간

	while (1) {
		// 4단계 : accept()함수 --> 클라이언트의 connect함수와 연결==> UDP는 없음
		addrlen = sizeof(clientaddr);


		// 클라이언트와의 데이터 통신
		retval = recvfrom(listen_sock, buf, BUFSIZE, 0, (SOCKADDR*)&clientaddr, &addrlen);
		// recvfrom(소켓, 버퍼, 버퍼크기, 0, 주소, 주소길이)
		// 소켓정보, 수신버퍼에 있는 값을 복사해서 저장할 응용프로그램 공간, 크기, 상태정보(0)
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			continue;
		}


		// 받은 데이터 출력
		buf[retval] = '\0';
		printf("[UDP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

		
	}// 데이터 통신의 마무리


// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();

	getchar();
	return 0;


}