#include "pch.h"
#include"BaseScene.h"
#include"TitleScene.h"
#include"GameScene.h"
#include"ResultScene.h"
#include "SceneManager.h"

SceneManager* SceneManager::m_instance = nullptr;
BaseScene* SceneManager::m_nowScene = nullptr;

//コンストラクタ
SceneManager::SceneManager()
{
	m_nowScene = new TitleScene();
}

//デストラクタ
SceneManager::~SceneManager()
{

}

//インスタンス生成
void SceneManager::CreateInstance()
{
	if (m_instance)return;
	m_instance = new SceneManager();
}

//インスタンス破棄
void SceneManager::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}

//シーン遷移処理
void SceneManager::ChangeScene(Scene scene)
{
	if (m_nowScene != NULL)
	{
		delete m_nowScene;
	}

	switch (scene)
	{
	case Scene::Title:
		m_nowScene = new TitleScene();
		break;
	case Scene::Game:
		m_nowScene = new GameScene();
		break;
	case Scene::Result:
		m_nowScene = new ResultScene();
		break;
	}
}

//シーン更新
void SceneManager::Update()
{
	m_nowScene->Update();
}

//シーン描画
void SceneManager::Draw()
{
	m_nowScene->Draw();
}

//オフスクリーン描画
void SceneManager::DrawOffScreen()
{
	m_nowScene->DrawOffScreen();
}