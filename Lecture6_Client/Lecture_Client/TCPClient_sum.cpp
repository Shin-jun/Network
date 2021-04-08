#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 50

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
	char buf[BUFSIZE+1];	// 응용 프로그램에서 사용할 버퍼
	int len;
	struct Mydata {
		int n1;
		int n2;
	}data;

	int result;
	int dummy;

	while(1){

		printf("\n숫자 2개를 띄어쓰기로 입력 : ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;

		// 문자 정리
		len = strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;

		if (sscanf(buf, "%d%d%d", &data.n1, &data.n2, &dummy) != 2) {
			fprintf(stderr, "[오류]숫자 2개를 입력하세요!\n");
			continue;
		}

		// 데이터 보내기(3단계)
		// 가변 길이 보내기
		retval = send(sock, (char *)&data, sizeof(data), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send");
			break;
		}
		// 데이터 받기
		retval = recvn(sock, (char*)&result, sizeof(result), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send");
			break;
		}
		else if (retval == 9) {
			break;
		}
		
		// 받은데이터 출력
		printf("계산결과 %d + %d = %d\n", data.n1, data.n2, result);
	}

	// 소켓 닫기(5단계)
	closesocket(sock);
	// 윈속 종료
	WSACleanup();

	return 0;
}