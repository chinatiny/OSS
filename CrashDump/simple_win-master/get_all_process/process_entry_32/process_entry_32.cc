//http://www.pediy.com/kssd/pediy04/kanxue036.htm
//�����ӣ�������ΪȨ�������޷���ȡ������Ϊexplorer.exe�ĸ�������

#include <windows.h> 
#include <tlhelp32.h> 
#include <psapi.h> 
#include <stdio.h> 
#include <iostream>

#pragma comment(lib, "psapi") 

void DisplayProcessInfo(DWORD pid) 
{ 
	::printf("PID = %8u ", pid); 
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid); 
	wchar_t FileName[MAX_PATH]; 
	if (hProcess) 
	{ 
		if (::GetModuleFileNameEx(hProcess, NULL, FileName, MAX_PATH)) 
		{ 
			 ::printf("%ws\n", FileName); 
		} 
		else 
		{ 
			::printf("�޷���ý�����\n");
		} 
		::CloseHandle(hProcess); 
	} 
	else 
	{ 
		::printf("��Ȩ���ʸý���\n");
	} 
} 

void main(void) 
{ 
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
	if (hProcessSnap == INVALID_HANDLE_VALUE) 
	{ 
		::printf("�޷��������̵Ŀ���\n");
		return; 
	} 

	PROCESSENTRY32 pe32      = {0}; 
	pe32.dwSize = sizeof(PROCESSENTRY32); 
	if (!Process32First(hProcessSnap, &pe32)) 
	{ 
		::printf("�޷���ý����б�\n");
		::CloseHandle(hProcessSnap);
		return; 
	} 

	do 
	{ 
		DisplayProcessInfo(pe32.th32ProcessID); 
		DisplayProcessInfo(pe32.th32ParentProcessID); 
		::printf("\n");
		::printf("\n");
	} while(::Process32Next(hProcessSnap, &pe32)); 
	::CloseHandle(hProcessSnap); 
	::system("pause");
}