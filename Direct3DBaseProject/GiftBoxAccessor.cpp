#include "pch.h"
#include"Json.h"
#include"GiftBox.h"
#include "GiftBoxAccessor.h"

GiftBoxAccessor* GiftBoxAccessor::m_instance = nullptr;

//���f���̐���
GiftBoxAccessor::GiftBoxAccessor()
{
	auto json = Json::GetInstance();
	m_giftBox = new GiftBox(json->Widen(json->GetData()["GiftBox"].at(0)).c_str(),
		Vector3(json->GetData()["GiftBox"].at(1).at(0),
			json->GetData()["GiftBox"].at(1).at(1),
			json->GetData()["GiftBox"].at(1).at(2)),
		float(json->GetData()["GiftBox"].at(1).at(3)));
}

//�f�[�^�j��
GiftBoxAccessor::~GiftBoxAccessor()
{

}

//�C���X�^���X����
void GiftBoxAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new GiftBoxAccessor();
}

//�C���X�^���X�j��
void GiftBoxAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}