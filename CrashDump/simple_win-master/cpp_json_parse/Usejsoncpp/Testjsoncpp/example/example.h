#pragma once
#include <string>
#include <json/json.h>
#include "../utility/utility.h"

/**
jsoncpp��ʹ�ò���
�����֪��json�������ݸ�ʽ����ô��Ҫʹ�õݹ������

jsoncpp��Ҫ�ŵ��ǣ��죬�����е������ⶼ�졣
jsoncpp��Ҫȱ���ǲ�֧��unicode��
���У����SimpleJson�������ӿڶ࣬�о��Ƚϸ��ӡ�
�����У�����ʹ��jsoncpp��
simplejson��һ�ѿӣ������������ࡣ
boost property_treeҪ����boost�⣬̫�Ӵ�

2012-12-12
*/
class CTestjsoncpp
{
public:
	bool test_Parse();
	bool test_writefile();
	bool test_readfile();

private:
	std::wstring ReleaseJsonStr();
	Json::Value ReleaseJsonValue();
};