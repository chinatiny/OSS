#include "test.h"

/*
{
"contents": [{
"Test": "Test,Obj"
},
"HelloWorld,cswuyg",
"Good Good Studey,Day Day Up"],
"result": "no_way"
}
*/
std::wstring CTestJSON::GetJsonStr()
{
	JSONArray jsArray;
	JSONObject jsObj;
	jsObj[L"Test"] = new(std::nothrow)JSONValue(L"Test,Obj");
	jsArray.push_back(new(std::nothrow)JSONValue(jsObj));
	jsArray.push_back(new(std::nothrow)JSONValue(L"HelloWorld,cswuyg"));
	jsArray.push_back(new(std::nothrow)JSONValue(L"Good Good Studey,Day Day Up"));
	jsArray.push_back(new(std::nothrow)JSONValue((double)1988));

	JSONObject jsObjNew;
	jsObjNew[L"contents"] = new(std::nothrow)JSONValue(jsArray);
	jsObjNew[L"result"] = new(std::nothrow)JSONValue(L"no_way");

	JSONValue jsValue = jsObjNew;
	std::wstring strRet = jsValue.Stringify().c_str();
	return strRet;
}

void CTestJSON::parseJsonStr( const std::wstring& strJsonStr )
{
	JSONValue* jsInput = JSON::Parse(strJsonStr.c_str());
	if (jsInput == NULL || !jsInput->IsObject())
	{
		return;
	}

	JSONObject::const_iterator itResult = jsInput->AsObject().find(L"result");
	if (itResult != jsInput->AsObject().end())
	{
		std::wstring strResult = itResult->second->AsString();
		std::wcout << L"result" << L":" << strResult << std::endl;
	}

	JSONObject::const_iterator itLove = jsInput->AsObject().find(L"Love");
	if (itLove != jsInput->AsObject().end())
	{
		std::wstring strResult = itLove->second->AsString();
		std::wcout << L"Love" << L":" << strResult << std::endl;
	}

	JSONArray jsArray;
	JSONObject::const_iterator itContents = jsInput->AsObject().find(L"contents");
	if (itContents != jsInput->AsObject().end() && itContents->second != NULL && itContents->second->IsArray())
	{
		jsArray = itContents->second->AsArray();
	}

	std::wcout << "[" << std::endl;
	JSONArray::iterator it = jsArray.begin();
	JSONArray::iterator itEnd = jsArray.end();
	for (; it != itEnd; ++it)
	{
		JSONValue* jsValue = *it;
		if (jsValue->IsObject())
		{
			jsValue->AsObject();
			JSONObject::const_iterator itObj = jsValue->AsObject().begin();
			JSONObject::const_iterator itObjEnd = jsValue->AsObject().end();
			for (; itObj != itObjEnd; ++itObj)
			{
				std::wstring strValue = itObj->second->AsString();
				std::wcout << L"{" << itObj->first << L":" << strValue << L"}" << std::endl;
			}
		}
		else if (jsValue->IsString())
		{		
			std::wstring strValue = jsValue->AsString();
			std::wcout << strValue << std::endl;
		}
		else if (jsValue->IsNumber())
		{
			double dValue = jsValue->AsNumber();
			std::wcout << dValue << std::endl;
		}
		//...
	}
	std::wcout << "]" << std::endl;
}

std::wstring CTestJSON::AddJsonStr( const std::wstring& strJsonStr )
{
	/*
	SimpleJson ������Ƿǳ��鷳�����顣
	JSONValue����������ָ��Ĺ��ܣ����������������������JSON����
	��JSON��as....���������ص���const���͵����ã������array���ͣ���ô��JSONValue*��ǳ����
	��Parse�ķ���ֵʵ��delete֮��JSONValue�ֻ���deleteһ�Σ����ǳ��ֶ�������Ĵ���
	���Ա��뱣֤��Ҫôֻ��JSONValue����ȥִ��������Ҫôֻ��������delete Parse�ķ���ֵ��
	���ڲ�����˵�������߼�������鷳��������һ��JSONValue��ǳ������parse����ֵ��һ����json����
	Ȼ��JSONValue������ǳ������JSONValue*���ȶ�Parse�ķ���ֵ��������������delete�����������JSON����
	�����delete����Parse�ڲ�new��map�ڴ�û�б�������

	����취�����֣�
	1����Ҫ����JSONValue���󣬶��Ƕ���JSONValue���ã���Ϊ��Ҫ��JSONValue���ֵ�����Ա����õ�const_cast��
	2���ݹ鿽����JSONValue*����ַ�����ʽ��JSON����Ȼ����Parse֮�������JSONObj��
	��֤�¡��ɶ�����롣
	*/
	JSONValue* jsInput = JSON::Parse(strJsonStr.c_str());
	if (jsInput == NULL || !jsInput->IsObject())
	{
		return L"";
	}

	std::wstring strRet;
	JSONObject jsObjNew;
	JSONObject::const_iterator it = jsInput->AsObject().begin();
	JSONObject::const_iterator itEnd = jsInput->AsObject().end();
	for (; it != itEnd; ++it)
	{
		std::wstring strFirst = it->first.c_str();
		std::wstring strSecond = it->second->Stringify().c_str();
		JSONValue* pTemp = JSON::Parse(strSecond.c_str());
		jsObjNew[strFirst] = pTemp;
	}
	jsObjNew[L"Love"] = new(std::nothrow)JSONValue(L"is Happiness");
	JSONValue jsValueNew = jsObjNew;
	strRet = jsValueNew.Stringify();
	return strRet;
}