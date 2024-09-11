#include "pch.h"
#include"Json.h"
#include"Player.h"
#include "PlayerAccessor.h"

PlayerAccessor* PlayerAccessor::m_instance = nullptr;

//���f���̐���
PlayerAccessor::PlayerAccessor()
{
	auto json = Json::GetInstance();
	m_player = new Player(json->Widen(json->GetData()["Player"].at(0)).c_str());
}

//�f�[�^�j��
PlayerAccessor::~PlayerAccessor()
{
	delete(m_player);
}

//�C���X�^���X����
void PlayerAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new PlayerAccessor();
}

//�C���X�^���X�j��
void PlayerAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}