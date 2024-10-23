#include "pch.h"
#include"BaseScene.h"
#include"TitleScene.h"
#include"GameScene.h"
#include"ResultScene.h"
#include "SceneManager.h"

SceneManager* SceneManager::m_instance = nullptr;
BaseScene* SceneManager::m_nowScene = nullptr;

//�R���X�g���N�^
SceneManager::SceneManager()
{
	m_nowScene = new TitleScene();
}

//�f�X�g���N�^
SceneManager::~SceneManager()
{

}

//�C���X�^���X����
void SceneManager::CreateInstance()
{
	if (m_instance)return;
	m_instance = new SceneManager();
}

//�C���X�^���X�j��
void SceneManager::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}

//�V�[���J�ڏ���
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

//�V�[���X�V
void SceneManager::Update()
{
	m_nowScene->Update();
}

//�V�[���`��
void SceneManager::Draw()
{
	m_nowScene->Draw();
}

//�I�t�X�N���[���`��
void SceneManager::DrawOffScreen()
{
	m_nowScene->DrawOffScreen();
}