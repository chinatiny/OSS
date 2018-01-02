#include <windows.h>
#include <iostream>
#include <process.h>

/**
1���ɵȴ���ʱ���������ô�����ʱ�����߳��Լ��ȴ���Ҳ������������̵߳ȴ���
*/
static unsigned int _stdcall ThreadProc(void* argv)
{
	HANDLE hKernelTimer = static_cast<HANDLE>(argv);
	if(::WaitForSingleObject(hKernelTimer, INFINITE) != WAIT_OBJECT_0)
	{
		return -1;
	}
	std::wcout << "Waitable Timer arrive" << std::endl;
	return 0;
}

/**
�첽���̵��ã�����SetWaitableTimer�ĵ����̴߳��ڿ�����״̬ʱ�����ACP�����Żᱻͬһ���̵߳���
���Ա���ע�⵽�����߳�ʹ����SleepEx����ڶ���������TRUE�����ɵȴ���ʱ������ACP����ʱ������˯�߽�����
*/
void static _stdcall TimerACPRoutine(void* pvArg, DWORD dwTimerLowValue, DWORD dwTimerHighValue)
{
	UNREFERENCED_PARAMETER(dwTimerLowValue);
	UNREFERENCED_PARAMETER(dwTimerHighValue);
	int arg = *(static_cast< int* >(pvArg));
	std::wcout << arg << std::endl;
	//::SleepEx(10000,TRUE); //���õ����߳�˯��
}


/**���ÿɵȴ���ʱ���ĺ���*/
int UseKernelTimer()
{
	HANDLE hKernelTimer = ::CreateWaitableTimer(NULL, FALSE, NULL);
	if (!hKernelTimer)
	{
		return -1;
	}
	const int nTimerUnitsPerSecond = 10000000;
	LARGE_INTEGER li;
	li.QuadPart = -(2 * nTimerUnitsPerSecond);
	int arg = 10;
	if(!::SetWaitableTimer(hKernelTimer, &li, 2000, TimerACPRoutine, &arg, TRUE))
	{
		::CloseHandle(hKernelTimer);
		return -1;
	}
	::_beginthreadex(NULL, NULL, &ThreadProc, hKernelTimer, NULL, NULL);
	//if (::WaitForSingleObject(hKernelTimer, INFINITE) != WAIT_OBJECT_0)
	//{
	//	::CloseHandle(hKernelTimer);
	//	return -1;
	//}
	//::CloseHandle(hKernelTimer);
	//std::wcout << "KernelTimer Arrive" << std::endl;
	return 0;
}



int main()
{
	UseKernelTimer();
	::SleepEx(100000, TRUE);
	::system("pause");
	return 1;
}