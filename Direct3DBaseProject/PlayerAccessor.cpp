#include "pch.h"
#include"Json.h"
#include"Player.h"
#include "PlayerAccessor.h"

PlayerAccessor* PlayerAccessor::m_instance = nullptr;

//モデルの生成
PlayerAccessor::PlayerAccessor()
{
	auto json = Json::GetInstance();
	m_player = new Player(json->Widen(json->GetData()["Player"].at(0)).c_str());
}

//データ破棄
PlayerAccessor::~PlayerAccessor()
{
	delete(m_player);
}

//インスタンス生成
void PlayerAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new PlayerAccessor();
}

//インスタンス破棄
void PlayerAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}