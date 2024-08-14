#include "pch.h"
#include"GameObject.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include"Block.h"
#include"BlockAccessor.h"
#include"Desk.h"
#include"DeskAccessor.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include "GameObjectManager.h"

GameObjectManager* GameObjectManager::m_instance = nullptr;

//管理するオブジェクトの設定
GameObjectManager::GameObjectManager()
{
	CameraAccessor::CreateInstance();
	BlockAccessor::CreateInstance();
	DeskAccessor::CreateInstance();
	PlayerAccessor::CreateInstance();

	m_gameObjects.push_back(dynamic_cast<GameObject*>(CameraAccessor::GetInstance()->GetCamera()));
	for (int i = 0; i < BlockAccessor::GetInstance()->GetBlocks().size(); i++)
	{
		m_gameObjects.push_back(dynamic_cast<GameObject*>(BlockAccessor::GetInstance()->GetBlocks().at(i)));
	}
	m_gameObjects.push_back(dynamic_cast<GameObject*>(DeskAccessor::GetInstance()->GetDesk()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(PlayerAccessor::GetInstance()->GetPlayer()));
}

//データ破棄
GameObjectManager::~GameObjectManager()
{
	if (m_gameObjects.empty())return;
	m_gameObjects.clear();

	CameraAccessor::DestroyInstance();
	BlockAccessor::DestroyInstance();
	DeskAccessor::DestroyInstance();
	PlayerAccessor::DestroyInstance();
}

//インスタンス生成
void GameObjectManager::CreateInstance()
{
	if (m_instance)return;
	m_instance = new GameObjectManager();
}

//インスタンス破棄
void GameObjectManager::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}

//オブジェクトの更新
void GameObjectManager::Update()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->Update();
	}
}

//オブジェクトの描画
void GameObjectManager::Draw()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->Draw();
	}
}