#include "pch.h"
#include"DeviceAccessor.h"
#include"BaseScene.h"
#include"SceneManager.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include"GiftBox.h"
#include"GiftBoxAccessor.h"
#include"GameObjectManager.h"
#include "TitleScene.h"

TitleScene::TitleScene()
{
	auto gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager->InitializeTitle();
}

TitleScene::~TitleScene()
{

}

void TitleScene::Update()
{
	auto key = DeviceAccessor::GetInstance()->GetKeyboard()->GetState();
	if (key.Enter)
	{
		SceneManager::ChangeScene(SceneManager::Scene::Game);
	}
	auto gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager->UpdateTitle();
}

void TitleScene::Draw()
{
	auto gameObjectManager = GameObjectManager::GetInstance();

	gameObjectManager->SetObjectShadowResource();
	gameObjectManager->SetCharacterShadowResource();

	gameObjectManager->DrawTitle();
}

void TitleScene::DrawOffScreen()
{
	auto gameObjectManager = GameObjectManager::GetInstance();
	//‰e‚Ì•`‰æ
	gameObjectManager->ClearObjectShadow();
	gameObjectManager->SetObjectShadowRenderTarget();
	GiftBoxAccessor::GetInstance()->GetGiftBox()->DrawShadow();

	gameObjectManager->ClearCharacterShadow();
	gameObjectManager->SetCharacterShadowRenderTarget();
	PlayerAccessor::GetInstance()->GetPlayer()->DrawShadow();
}