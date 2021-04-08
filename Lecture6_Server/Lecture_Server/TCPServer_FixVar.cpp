#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define SERVERPORT 9000
#define BUFSIZE 512

// 고정+가변
// 함수 정의 --> 종료
void err_quit(const char* msg) {
	exit(1);
}

// 소켓 함수 오류 출력 정의
void err_display(const char* msg) {
	printf("오류 메시지");
}

// 사용자 정의 --> recvn
int recvn(SOCKET s, char* buf, int len, int flags) {
	int received; // 수신 데이터
	char* ptr = buf; // 버퍼의 시작위치
	int left = len; // 읽을 데이터

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
	int retval;		// 소켓에 정보를 저장할 변수

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성 --> 서버 소켓 생성
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	// AF_INET : IPv4, SOCK_STREAM : TCP
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

	// 3단계 : listen() : 서버 소켓의 상태를 "대기"상태로 변환
	retval = listen(listen_sock, SOMAXCONN);	// SOMAXCONN : 최대값
	if (retval == SOCKET_ERROR) err_quit("listen()");


	// 통신을 위한 준비 작업
	SOCKET client_sock;			// 연결될때 생성된 소켓 정보
	SOCKADDR_IN clientaddr;		// 클라이언트 ip, 포트 (원격)
	int addrlen;		// 주소 길이
	char buf[BUFSIZE + 1];		// 응용 프로그램에서 송신, 수신 버퍼에 있는 값을 복사해서 넣을 공간

	int len;	// 고정길이를 받을때 사용할 변수

	while (1) {
		// 4단계 : accept()함수 --> 클라이언트의 connect함수와 연결
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		printf("\n[TCP서버] 클라이언트 접속 : IP주소=%s, 포트번호=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 클라이언트와의 데이터 통신
		while (1) {
			// 5단계 : 수신
			
			// 고정 길이(보내기 1)
			retval = recvn(client_sock, (char*)&len, sizeof(int), 0);
			// 소켓정보, 수신버퍼에 있는 값을 복사해서 저장할 응용프로그램 공간, 크기, 상태정보(0)
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0) {
				break;
			}

			// 가변 길이(보내기 2)
			retval = recvn(client_sock, buf, len, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0) {
				break;
			}


			// 받은 데이터 출력
			buf[retval] = '\0';
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

			// 6단계 : 데이터 보내기(송신)
			retval = send(client_sock, buf, retval, 0);
			// 응용프로그램안에 buf정보를 송신 버퍼에 복사해서 전달
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}// 데이터 통신의 마무리

		// closesocket 7단계 : 소켓 종료
		closesocket(client_sock);
		printf("[TCP서버] 클라이언트 종료:IP주소 =%s, 포트번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();

	getchar();
	return 0;


}