#include "pch.h"
#include"Json.h"
#include"Enemy.h"
#include "EnemyAccessor.h"

EnemyAccessor* EnemyAccessor::m_instance = nullptr;

//���f���̐���
EnemyAccessor::EnemyAccessor()
{
	auto json = Json::GetInstance();
	m_enemy = new Enemy(json->Widen(json->GetData()["Enemy"].at(0)).c_str());
}

//�f�[�^�j��
EnemyAccessor::~EnemyAccessor()
{

}

//�C���X�^���X����
void EnemyAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new EnemyAccessor();
}

//�C���X�^���X�j��
void EnemyAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}