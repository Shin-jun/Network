#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32")
// 프로젝트 속성
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>

void err_display(const char *msg) {
	printf("오류");
}

int main_init()
{
	// 윈속 초기화 위한 구조체 선언
	WSADATA wsa;


	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)	// WSAStartup(버전, 윈속 정보 주소)
		return 1;

	// 메시지 박스(소유, 출력내용, 창제목, 버튼)
	MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);

	// 소켓 생성
	SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);	// socket(주소체계, 소켓타입, 프로토콜 타입)
	if (tcp_sock == INVALID_SOCKET)	// 소켓 생성 실패시
		err_display("socket");

	MessageBox(NULL, "소켓 성공", "알림", MB_OK);

	// 소켓 닫기
	closesocket(tcp_sock);	// 소켓 닫기

	WSACleanup();	// 윈속 종료
	return 0;
		
}