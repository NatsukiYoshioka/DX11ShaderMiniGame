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

//リザルトシーン初期化
ResultScene::ResultScene()
{
	m_isChangeScene = false;
	auto gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager->InitializeResult();
}

//データ破棄
ResultScene::~ResultScene()
{

}

//シーン更新
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
	//Aボタンを押したらシーン遷移
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

//シーン描画
void ResultScene::Draw()
{
	auto gameObjectManager = GameObjectManager::GetInstance();

	gameObjectManager->SetObjectShadowResource();

	gameObjectManager->DrawResult();
}

//オフスクリーン描画
void ResultScene::DrawOffScreen()
{
	auto gameObjectManager = GameObjectManager::GetInstance();
	//影の描画
	gameObjectManager->ClearObjectShadow();
	gameObjectManager->SetObjectShadowRenderTarget();
	gameObjectManager->DrawShadow();
}