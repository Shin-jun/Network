#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define SERVERIP "127.0.0.1"
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
	const char* chatid = "클라이언트-신용준1";

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 1단계 : 소켓 생성 --> 클라이언트 소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	// AF_INET : IPv4, SOCK_STREAM : TCP
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// 2단계 : connect() --> 소켓 주소구조체
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));	// 소켓 주소 구조체의 초기화
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);	// 서버 연결 주소
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect");


	// 데이터 통신에 사용할 변수
	char msg[BUFSIZE + 1];
	char* buf;
	int len;

	sprintf(msg, "[%s]", chatid);
	buf = msg + strlen(msg);

	while(1){

		// 채팅메시지 출력
		printf("[%s]", chatid);
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;
		// 문자 정리
		len = strlen(msg);
		if(msg[len-1]=='\n')
			msg[len-1] = '\0';
		if (strlen(buf) == 0)
			break;

		// 데이터 보내기(3단계)
		// 고정 길이 보내기 (4byte)
		retval = send(sock, (char*)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send");
			break;
		}
		// 가변 길이 보내기
		retval = send(sock, msg, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send");
			break;
		}

		// 데이터받기
		// 고정 길이 받기
		retval = recvn(sock, (char*)&len, sizeof(int), 0);

		// 소켓정보, 수신버퍼에 있는 값을 복사해서 저장할 응용프로그램 공간, 크기, 상태정보(0)
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0) {
			break;
		}

		// 가변 길이 받기
		retval = recvn(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0) {
			break;
		}
		// 받은 데이터 출력
		buf[retval] = '\0';
		printf("%s\n", buf);
	}

	// 소켓 닫기(5단계)
	closesocket(sock);
	// 윈속 종료
	WSACleanup();

	return 0;
}