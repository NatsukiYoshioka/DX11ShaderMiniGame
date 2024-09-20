#include "pch.h"
#include"DeviceAccessor.h"
#include"Desk.h"
#include"DeskAccessor.h"
#include"DollHead.h"
#include"DollHeadAccessor.h"
#include "BaseScene.h"
#include"GameObjectManager.h"
#include "ResultScene.h"

extern void ExitGame() noexcept;

ResultScene::ResultScene()
{
	m_isChangeScene = false;
	auto gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager->InitializeResult();
}

ResultScene::~ResultScene()
{

}

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
}

void ResultScene::Draw()
{
	auto gameObjectManager = GameObjectManager::GetInstance();

	gameObjectManager->SetObjectShadowResource();
	gameObjectManager->SetCharacterShadowResource();

	gameObjectManager->DrawResult();
}

void ResultScene::DrawOffScreen()
{
	auto gameObjectManager = GameObjectManager::GetInstance();
	//‰e‚Ì•`‰æ
	gameObjectManager->ClearObjectShadow();
	gameObjectManager->SetObjectShadowRenderTarget();
	DeskAccessor::GetInstance()->GetDesk()->DrawShadow();

	gameObjectManager->ClearCharacterShadow();
	gameObjectManager->SetCharacterShadowRenderTarget();
	DollHeadAccessor::GetInstance()->GetDollHead()->DrawShadow();
}