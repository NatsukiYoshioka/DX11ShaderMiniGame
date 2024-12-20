#include "pch.h"
#include"Desk.h"
#include"Json.h"
#include "DeskAccessor.h"

DeskAccessor* DeskAccessor::m_instance = nullptr;

//モデルの生成
DeskAccessor::DeskAccessor()
{
	auto json = Json::GetInstance();
	m_desk = new Desk(json->Widen(json->GetData()["Desk"].at(0)).c_str(),
		Vector3(json->GetData()["Desk"].at(1).at(0),
			json->GetData()["Desk"].at(1).at(1),
			json->GetData()["Desk"].at(1).at(2)),
		float(json->GetData()["Desk"].at(1).at(3)));
}

//データ破棄
DeskAccessor::~DeskAccessor()
{
	delete(m_desk);
}

//インスタンス生成
void DeskAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new DeskAccessor();
}

//インスタンス破棄
void DeskAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}