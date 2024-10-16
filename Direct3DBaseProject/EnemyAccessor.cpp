#include "pch.h"
#include"Json.h"
#include"Enemy.h"
#include "EnemyAccessor.h"

EnemyAccessor* EnemyAccessor::m_instance = nullptr;

//モデルの生成
EnemyAccessor::EnemyAccessor()
{
	auto json = Json::GetInstance();
	m_enemy = new Enemy(json->Widen(json->GetData()["Enemy"].at(0)).c_str());
}

//データ破棄
EnemyAccessor::~EnemyAccessor()
{

}

//インスタンス生成
void EnemyAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new EnemyAccessor();
}

//インスタンス破棄
void EnemyAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}