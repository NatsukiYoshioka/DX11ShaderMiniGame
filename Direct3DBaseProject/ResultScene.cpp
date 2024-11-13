#include "pch.h"
#include"DeviceAccessor.h"
#include"Desk.h"
#include"DeskAccessor.h"
#include"DollHead.h"
#include"DollHeadAccessor.h"
#include"UIBase.h"
#include"Transition.h"
#include"UIAccessor.h"
#include "BaseScene.h"
#include"GameObjectManager.h"
#include"SceneManager.h"
#include "ResultScene.h"

extern void ExitGame() noexcept;

//���U���g�V�[��������
ResultScene::ResultScene()
{
	m_isChangeScene = false;
	auto gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager->InitializeResult();
}

//�f�[�^�j��
ResultScene::~ResultScene()
{

}

//�V�[���X�V
void ResultScene::Update()
{
	auto pad = DeviceAccessor::GetInstance()->GetGamePad()->GetState(0);
	auto key = DeviceAccessor::GetInstance()->GetKeyboard()->GetState();
	
	auto gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager->UpdateResult();

	if (pad.IsViewPressed() || key.Escape)
	{
		ExitGame();
	}
	//A�{�^������������V�[���J��
	for (int i = 0; i < UIAccessor::GetInstance()->GetUIs().size(); i++)
	{
		auto transition = dynamic_cast<Transition*>(UIAccessor::GetInstance()->GetUIs().at(i));
		if (transition && transition->GetIsFinishFadein() && (pad.IsAPressed() || key.Enter))
		{
			m_isChangeScene = true;
			transition->SetTransitionStart(true);
		}
		if (transition && transition->GetIsFinishFadeout())
		{
			transition->SetTransitionStart(false);
			SceneManager::ChangeScene(SceneManager::Scene::Title);
			break;
		}
	}
}

//�V�[���`��
void ResultScene::Draw()
{
	auto gameObjectManager = GameObjectManager::GetInstance();

	gameObjectManager->SetObjectShadowResource();

	gameObjectManager->DrawResult();
}

//�I�t�X�N���[���`��
void ResultScene::DrawOffScreen()
{
	auto gameObjectManager = GameObjectManager::GetInstance();
	//�e�̕`��
	gameObjectManager->ClearObjectShadow();
	gameObjectManager->SetObjectShadowRenderTarget();
	gameObjectManager->DrawShadow();
}