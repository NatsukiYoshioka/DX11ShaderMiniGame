#include "pch.h"
#include<iostream>
#include <fstream>
#include<sstream>
#include "Json.h"

Json* Json::m_instance = nullptr;
const string Json::m_filePath = "Assets/Data/data.json";

Json::Json()
{
	ifstream ifs(m_filePath.c_str());
	if (!ifs.good())return;
	ifs >> m_json;
}

Json::~Json()
{

}

void Json::CreateInstance()
{
	if (m_instance)return;
	m_instance = new Json();
}

void Json::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}

wstring Json::Widen(const string& str)
{
	wostringstream wstm;
	const ctype<wchar_t>& ctfacet = use_facet<ctype<wchar_t>>(wstm.getloc());
	for (size_t i = 0; i < str.size(); ++i)
	{
		wstm << ctfacet.widen(str[i]);
	}
	return wstm.str();
}