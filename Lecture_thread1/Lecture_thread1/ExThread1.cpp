#include <Windows.h>
#include <stdio.h>

// 구조체 선언
struct Point3D {
	int x, y, z;
};

// 스레드 역할을 함수 정의 
DWORD WINAPI MyThread(LPVOID arg) {
	
	// 구조체 멤버에 접근하기 위한 부분

	Point3D* pt = (Point3D*)arg;	// &pt1, &pt2
	while (1) {
		printf("실행스레드 %d : %d, %d, %d\n", GetCurrentThreadId(), pt->x, pt->y, pt->z);
		Sleep(1000);	// 1초동안 정지
	}
	return 0;
}

int main() {

	Point3D pt1 = { 10, 20, 30 };

	// 쓰레드 생성 함수
	HANDLE hThread1 = CreateThread(NULL, 0, MyThread, &pt1, 0, NULL);
	// CreateThread(보안, 크기, 함수명, 전달될값, 생성여부, id);
	if (hThread1 == NULL) return 1;
	CloseHandle(hThread1);	// 스레드 닫기

	Point3D pt2 = { 40, 50, 60 };

	// 쓰레드 생성 함수
	HANDLE hThread2 = CreateThread(NULL, 0, MyThread, &pt2, 0, NULL);
	// CreateThread(보안, 크기, 함수명, 전달될값, 생성여부, id);
	if (hThread2 == NULL) return 1;
	CloseHandle(hThread2);	// 스레드 닫기

	while (1) {
		printf("메인스레드 %d\n", GetCurrentThreadId());
		Sleep(1);
	}

	return 0;
}