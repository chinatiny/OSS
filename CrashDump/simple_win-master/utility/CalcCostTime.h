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
	CCostTime();

	void Start();
	double CostTime();
#ifdef _DEBUG
	void debug_log(const std::wstring log_msg);
#endif

private:
	_LARGE_INTEGER time_start_;	//< ��ʼʱ��
	std::wstring cost_name_;
	double dCostTime;        //< ��ʱ second
};


