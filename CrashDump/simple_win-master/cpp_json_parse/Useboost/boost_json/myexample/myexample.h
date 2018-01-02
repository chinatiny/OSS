#pragma once
/*
2012-12-10
*/
//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/json_parser.hpp>
//#include <boost/foreach.hpp>
#include "J:\E\boost/property_tree/ptree.hpp"
#include "J:\E\boost\property_tree/json_parser.hpp"
#include "J:\E\boost/foreach.hpp"
/**
boost_1_52�ǵ�ǰ���°汾
���ԣ�http://sourceforge.net/projects/boost/files/boost/
��ͷ�����ӣ���������json�����Ӳ���д�ļ���\
��д�ļ��ģ�value������string����Ȼ����벻����
���Ҫ�Զ���key������Ҫʵ��path_of�࣬��ȻĬ��key��string����,boost�Ѿ�Ϊstringʵ����path_of������string_path.hpp�С�

�����ֱ��ʹ��read_json����·����ȡ�����ܻ����ַ���������⣬
��ʹ��unicode��ʽjson����ʱ�����ֱ��ʹ��·����ȡ��д�룬ʹ������ͦ�ҵģ�
���Խ������Լ���ȡ��stream������read_json��stream��ȡjson���ݣ��ܿ��ַ����룬
���߸����׵㣬��ȫʹ���Լ��Ķ�д�ļ�������json��ĺ���ֻ��string�򽻵�,��һ�������У���json��ֻ��string�򽻵�����Ҳ��һ�ֱȽϺõķֲ㡢��װ��
*/

/**
 ���ԵĴ��룬�Ͳ�����������
*/

class CTest
{
public:
	//save
	bool test_write_json_by_path();
	bool test_write_json_by_stream();
	bool test_save_by_custom();

	//read
	boost::property_tree::wptree test_read_json_by_path();
	boost::property_tree::wptree test_read_json_by_stream();
	boost::property_tree::wptree test_read_json_by_custom();
private:
	boost::property_tree::wptree Releaseptree();
	bool ShowJsonInfo(const boost::property_tree::wptree& ptree);
	boost::property_tree::wptree wstrToTree( const std::wstring& strJson );
private:

};