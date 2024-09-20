#include "pch.h"
#include"Json.h"
#include"DollHead.h"
#include "DollHeadAccessor.h"

DollHeadAccessor* DollHeadAccessor::m_instance = nullptr;

DollHeadAccessor::DollHeadAccessor()
{
	auto json = Json::GetInstance();
	m_dollHead = new DollHead(
		json->Widen(json->GetData()["PlayerHeadModel"]).c_str(),
		Vector3(json->GetData()["PlayerHeadPos"].at(0),
			json->GetData()["PlayerHeadPos"].at(1),
			json->GetData()["PlayerHeadPos"].at(2)));
}

DollHeadAccessor::~DollHeadAccessor()
{
	delete(m_dollHead);
}

void DollHeadAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new DollHeadAccessor();
}

void DollHeadAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}