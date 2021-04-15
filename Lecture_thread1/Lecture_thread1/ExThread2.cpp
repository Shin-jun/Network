#include <Windows.h>
#include <stdio.h>

int sum = 0;

// �����忡 �۾��� �� �Լ�
DWORD WINAPI MyThread2(LPVOID arg) {
	
	
	int num = (int)arg;
	// �հ豸�ϱ�
	for (int i = 1; i <= num; i++) sum += i;

	return 0;
}

int main(void) {
	int num = 100;

	// ������ ����
	HANDLE hThread = CreateThread(NULL, 0, MyThread2, (LPVOID)num, CREATE_SUSPENDED, NULL);
	// �Լ� �̸�, ��, ���࿩��

	if (hThread == NULL) return 0;
	printf("������ ���� ��: ��� ��� = %d\n", sum);

	// ������ ����(�����)
	ResumeThread(hThread);	// CREATE_SUSPEND�� ResumThread�Լ��� ȣ��Ǳ� ������ ���� ����

	WaitForSingleObject(hThread, INFINITE);	// �����尡 �� ����ɶ����� ��ٸ�
	printf("������ ���� ��, ��� ��� = %d\n", sum);
	CloseHandle(hThread);

	return 0;
}