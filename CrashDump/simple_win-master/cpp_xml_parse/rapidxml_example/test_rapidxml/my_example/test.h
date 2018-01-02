#pragma once 
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <rapidxml_print.hpp>

const wchar_t* const const_xml_path = L"c:\\test_rapid_xml.xml";

/*
����ʹ��rapidxml
2012-12-14
*/
class CTest
{
public:
	void test_load_xml();
	void test_release_xml();

private:
	/**
	��Ҫ��utf8��ת��
	*/
	void AddXMLAttributeValue(rapidxml::xml_document<>& pDoc, rapidxml::xml_node<>* pNode, const std::wstring& strKey, const std::wstring& strValue);
	std::wstring GetXMLAttributeValue(rapidxml::xml_node<>* pNode, const std::wstring& strwKey);
};