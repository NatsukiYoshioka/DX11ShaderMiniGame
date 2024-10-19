#include "pch.h"
#include"DeviceAccessor.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include"UIBase.h"
#include"FoundUI.h"
#include"Transition.h"
#include"UIAccessor.h"
#include "BaseScene.h"
#include"GameObjectManager.h"
#include"SceneManager.h"
#include "GameScene.h"

extern void ExitGame() noexcept;

GameScene::GameScene()
{
    m_isChangeScene = false;
    auto gameObjectManager = GameObjectManager::GetInstance();
    gameObjectManager->Initialize();
}

GameScene::~GameScene()
{

}

void GameScene::Update()
{
    auto pad = DeviceAccessor::GetInstance()->GetGamePad()->GetState(0);
    auto key = DeviceAccessor::GetInstance()->GetKeyboard()->GetState();

	auto gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager->Update();

    if (PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())
    {
        m_isChangeScene = true;
    }

    for (int i = 0; i < UIAccessor::GetInstance()->GetUIs().size(); i++)
    {
        auto foundUI = dynamic_cast<FoundUI*>(UIAccessor::GetInstance()->GetUIs().at(i));
        if (foundUI && foundUI->GetIsFound())
        {
            m_isChangeScene = true;
        }
        auto transition = dynamic_cast<Transition*>(UIAccessor::GetInstance()->GetUIs().at(i));
        if (transition && transition->GetIsFinishFadeout())
        {
            SceneManager::GetInstance()->ChangeScene(SceneManager::Scene::Result);
        }
    }

    if (pad.IsViewPressed() || key.Escape)
    {
        ExitGame();
    }
}

void GameScene::Draw()
{
	auto gameObjectManager = GameObjectManager::GetInstance();
    //敵視点から当たり判定と影テクスチャのセット
    gameObjectManager->SetHitCheckShaderResource();
    gameObjectManager->SetHitCheckCharacterShaderResource();
    gameObjectManager->HitCheck();

    gameObjectManager->SetObjectShadowResource();

    //オブジェクトの描画
	gameObjectManager->Draw();
}

void GameScene::DrawOffScreen()
{
    auto gameObjectManager = GameObjectManager::GetInstance();
    //影の描画
    gameObjectManager->ClearObjectShadow();
    gameObjectManager->SetObjectShadowRenderTarget();
    gameObjectManager->DrawShadow();

    //敵視点からの当たり判定用描画
    gameObjectManager->ClearHitCheckRenderTarget();
    gameObjectManager->SetHitCheckRenderTarget();
    gameObjectManager->DrawHitCheck();

    gameObjectManager->ClearHitCheckCharacterRenderTarget();
    gameObjectManager->SetHitCheckCharacterRenderTarget();
    gameObjectManager->DrawHitCheckCharacter();
}