// Testjsoncpp.cpp : Defines the entry point for the console application.
//

#include "example/example.h"
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <string>
#include <iostream>

/**
jsoncpp��ʹ�ò���
�����֪��json�������ݸ�ʽ����ô��Ҫʹ�õݹ������

jsoncpp��Ҫ�ŵ��ǣ��죬�����е������ⶼ�졣
jsoncpp��Ҫȱ���ǲ�֧��unicode��
���У����SimpleJson�������ӿڶ࣬�о��Ƚϸ��ӡ�
�����У�����ʹ��jsoncpp����֧��ֱ��unicode�����Ǵ����⡣
simplejson���ڴ������ӣ���������ҪС������
boost property_treeҪ����boost�⣬̫�Ӵ�

2012-12-12
*/

void TestFun()
{
	CTestjsoncpp testobj;
	testobj.test_Parse();
	testobj.test_writefile();
	testobj.test_readfile();
}


int _tmain(int argc, _TCHAR* argv[])
{
	TestFun();
	system("pause");
	return 0;
}

