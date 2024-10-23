#include "pch.h"
#include"DeviceAccessor.h"
#include"BaseScene.h"
#include"SceneManager.h"
#include"UIBase.h"
#include"Transition.h"
#include"UIAccessor.h"
#include"Enemy.h"
#include"EnemyAccessor.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include"GiftBox.h"
#include"GiftBoxAccessor.h"
#include"GameObjectManager.h"
#include "TitleScene.h"

extern void ExitGame() noexcept;

//タイトルシーン初期化
TitleScene::TitleScene():
	m_isStartGame(false)
{
	m_isChangeScene = false;
	auto gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager->InitializeTitle();
}

//データ破棄
TitleScene::~TitleScene()
{

}

//シーン更新
void TitleScene::Update()
{
	auto pad = DeviceAccessor::GetInstance()->GetGamePad()->GetState(0);
	auto key = DeviceAccessor::GetInstance()->GetKeyboard()->GetState();

	auto gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager->UpdateTitle();
	
	if (pad.IsViewPressed() || key.Escape)
	{
		ExitGame();
	}
	//Aボタンを押していたらシーン遷移
	for (int i = 0; i < UIAccessor::GetInstance()->GetUIs().size(); i++)
	{
		auto transition = dynamic_cast<Transition*>(UIAccessor::GetInstance()->GetUIs().at(i));
		if (transition && transition->GetIsFinishFadein())
		{
			if (EnemyAccessor::GetInstance()->GetEnemy()->GetNowAnimationState() == Enemy::AnimationState::PickUp)
			{
				m_isChangeScene = true;
			}
			if (key.Enter || pad.IsAPressed())
			{
				m_isStartGame = true;
			}
		}
		if (transition && transition->GetIsFinishFadeout())
		{
			SceneManager::ChangeScene(SceneManager::Scene::Game);
			break;
		}
	}
}

//シーン描画
void TitleScene::Draw()
{
	auto gameObjectManager = GameObjectManager::GetInstance();

	gameObjectManager->SetObjectShadowResource();

	gameObjectManager->DrawTitle();
}

//オフスクリーン描画
void TitleScene::DrawOffScreen()
{
	auto gameObjectManager = GameObjectManager::GetInstance();
	//影の描画
	gameObjectManager->ClearObjectShadow();
	gameObjectManager->SetObjectShadowRenderTarget();
	gameObjectManager->DrawShadow();
}