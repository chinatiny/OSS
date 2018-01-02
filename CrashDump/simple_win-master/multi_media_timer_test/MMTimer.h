#pragma once

#include <Windows.h>
#include <Mmsystem.h>
#pragma comment(lib,"Winmm.lib")
#include <map>


/**
��ý�嶨ʱ��
cswuyg@gmail.com
2013.5.16����
*/

/**
@brief �ٽ����Զ���
*/
class CAutoLock
{
public:
	CAutoLock(CRITICAL_SECTION* pSection)
		: m_pSection(pSection)
	{
		::EnterCriticalSection(m_pSection);
	}
	~CAutoLock()
	{
		::LeaveCriticalSection(m_pSection);
	}
private:
	CRITICAL_SECTION* m_pSection;
};

/**
@brief ��ʱ���ص�
*/
class ITimerCallBack
{
public:
	virtual void TimerCallBack() = 0;
};
/**
@brief ��ý�嶨ʱ��
*/
class CMMTimer
{
public:
	struct UserTimerInfo
	{
		ITimerCallBack* pCallBack;
		UINT nUserTimerID;
	};
	typedef  std::map<UINT, UserTimerInfo> T_mapTimer;

	CMMTimer();
	virtual ~CMMTimer();

	int SetTimer(ITimerCallBack* pCallback, UINT nUserTimerID, DWORD uElapse, bool bOnce = false);
	void KillTimer(UINT nUserTimerID);
	bool HasTimer(UINT nUserTimerID);

private:
	void static CALLBACK HighTimerCallBack(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
	void SendHighTimerMsg(UINT nTimerID);

private:
	TIMECAPS m_timeCaps;            //< ʱ��Ƶ�������С����
	MMRESULT m_retTimeGetDevCaps;   //< timeGetDevCaps Ret 
	T_mapTimer m_mapTimeID;         //< TimerID & HWND 
	ITimerCallBack* m_pCallBack;    //<  

	CRITICAL_SECTION m_section;
};