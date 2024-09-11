#include "pch.h"
#include"Json.h"
#include"GiftBox.h"
#include "GiftBoxAccessor.h"

GiftBoxAccessor* GiftBoxAccessor::m_instance = nullptr;

GiftBoxAccessor::GiftBoxAccessor()
{
	auto json = Json::GetInstance();
	m_giftBox = new GiftBox(json->Widen(json->GetData()["GiftBox"].at(0)).c_str(),
		Vector3(json->GetData()["GiftBox"].at(1).at(0),
			json->GetData()["GiftBox"].at(1).at(1),
			json->GetData()["GiftBox"].at(1).at(2)),
		float(json->GetData()["GiftBox"].at(1).at(3)));
}

GiftBoxAccessor::~GiftBoxAccessor()
{

}

void GiftBoxAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new GiftBoxAccessor();
}

void GiftBoxAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}