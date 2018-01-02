#pragma once


#include <windows.h>
#include <string>


/**
���ڻ�ȡ��ʱ����
ʹ��QueryPerformanceCounter������
*/
class CCostTime
{
public:
	CCostTime (const wchar_t* log_name);
	~CCostTime();
	void debug_log(const std::wstring log_msg);
private:
	void Start();
	void End();
	double CostTime();
private:
	_LARGE_INTEGER time_start_;	//��ʼʱ��
	_LARGE_INTEGER time_end_;	//����ʱ��
	std::wstring cost_name_;
};



