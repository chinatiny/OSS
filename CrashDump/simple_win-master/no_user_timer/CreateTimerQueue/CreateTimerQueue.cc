// createtime.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
/**
����Ҫʹ����Ϣѭ���Ķ�ʱ����
Ʃ�磺��д�������ʱ������ã������ú��������ĳ�������ڿⱻ���غ��ĳ��ʱ�䶨ʱ������
�Ҳ���Ҫ����õ����������߳�ӵ����Ϣѭ����

2013.3.3
cswuyg@gmail.com
*/

VOID WINAPI TimerQueueCallBack(LPVOID pVoid, BOOLEAN bTimeOrWait);

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hTimeQueue = ::CreateTimerQueue();
	HANDLE hTimer = NULL;
	::CreateTimerQueueTimer(&hTimer, hTimeQueue, TimerQueueCallBack, NULL, 1000, 1000, WT_EXECUTEDEFAULT);
	Sleep(5000);
	DeleteTimerQueueEx(hTimeQueue, INVALID_HANDLE_VALUE);
	::wprintf(L"Game over \n");
	getchar();
	return 0;
}

VOID WINAPI TimerQueueCallBack(LPVOID param, BOOLEAN bTimeOrWait)
{
	::wprintf(L"%s \n", L"Hello");
	return;
}