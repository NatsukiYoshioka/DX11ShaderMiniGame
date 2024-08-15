#include "pch.h"
#include"Json.h"
#include"Player.h"
#include "PlayerAccessor.h"

PlayerAccessor* PlayerAccessor::m_instance = nullptr;

PlayerAccessor::PlayerAccessor()
{
	auto json = Json::GetInstance();
	m_player = new Player(json->Widen(json->GetData()["Player"].at(0)).c_str(),
		Vector3(json->GetData()["PlayerPosition"].at(0),
			json->GetData()["PlayerPosition"].at(1),
			json->GetData()["PlayerPosition"].at(2)),
		float(json->GetData()["PlayerPosition"].at(3)));
}

PlayerAccessor::~PlayerAccessor()
{
	delete(m_player);
}

void PlayerAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new PlayerAccessor();
}

void PlayerAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}