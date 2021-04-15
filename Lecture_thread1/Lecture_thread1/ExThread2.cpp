#include <Windows.h>
#include <stdio.h>

int sum = 0;

// 스레드에 작업을 할 함수
DWORD WINAPI MyThread2(LPVOID arg) {
	
	
	int num = (int)arg;
	// 합계구하기
	for (int i = 1; i <= num; i++) sum += i;

	return 0;
}

int main(void) {
	int num = 100;

	// 스레드 생성
	HANDLE hThread = CreateThread(NULL, 0, MyThread2, (LPVOID)num, CREATE_SUSPENDED, NULL);
	// 함수 이름, 값, 실행여부

	if (hThread == NULL) return 0;
	printf("스레드 실행 전: 계산 결과 = %d\n", sum);

	// 스레드 실행(재실행)
	ResumeThread(hThread);	// CREATE_SUSPEND는 ResumThread함수를 호출되기 전까지 실행 안함

	WaitForSingleObject(hThread, INFINITE);	// 스레드가 다 실행될때까지 기다림
	printf("스레드 실행 후, 계산 결과 = %d\n", sum);
	CloseHandle(hThread);

	return 0;
}