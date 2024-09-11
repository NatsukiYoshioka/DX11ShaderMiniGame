#include "pch.h"
#include"BaseScene.h"
#include"TitleScene.h"
#include"GameScene.h"
#include"ResultScene.h"
#include "SceneManager.h"

SceneManager* SceneManager::m_instance = nullptr;
BaseScene* SceneManager::m_nowScene = nullptr;

SceneManager::SceneManager()
{
	m_nowScene = new TitleScene();
}

SceneManager::~SceneManager()
{

}

void SceneManager::CreateInstance()
{
	if (m_instance)return;
	m_instance = new SceneManager();
}

void SceneManager::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}

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

void SceneManager::Update()
{
	m_nowScene->Update();
}

void SceneManager::Draw()
{
	m_nowScene->Draw();
}

void SceneManager::DrawOffScreen()
{
	m_nowScene->DrawOffScreen();
}