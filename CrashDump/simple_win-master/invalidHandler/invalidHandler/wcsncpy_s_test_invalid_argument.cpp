/*
A example about wcsncpy_s's invalid parameter.
2012-10-17
**/
#include "stdafx.h"
#include <iostream>
#include <windows.h>

static void InvalidParameterHandler(const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, uintptr_t)
{
	throw std::invalid_argument("��������");
}

void TestFunc()
{
	wchar_t* lpszDes = new wchar_t[ 20 ];
	wchar_t lpszSrc[] = L"123456";
	//��ΪĿ��bufferҪ�����ַ���������������Ŀ��buffer���Ȳ��������Դ�ַ������ȴ�1
	//���Դbuffer�����ַ����������ĳ���С�ڵ��ڲ���DestLength��
	//��ô���������SourLength����DestLength��Ҳ��û����ģ�
	//Ҳ����˵��������ȫ��飬�ڸ���Դ��Ŀ���length�����֮ǰ�����ȼ���ַ���������
	//ֻҪĿ��buffer����������Դ�ַ�������ô�ͺ��Ե�Դ�ַ����ĳ��Ȳ�����顣
	//�ַ�����ȫ��⣬������쳣����ôSEH\C++���쳣��׽���ƶ�û����׽��ֻ����ͨ��
	//_set_invalid_parameter_handler�����ú���ȥ��׽�������׽�����ǽ��̼���ġ�
	try
	{	
		wcsncpy_s( lpszDes, 5, lpszSrc, 4 ); 
	}
	catch ( std::invalid_argument& err)
	{
		std::cout << err.what() << std::endl;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	_set_invalid_parameter_handler( InvalidParameterHandler );
	TestFunc();
	system( "pause" );
	return 0;
}

