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

//ゲームシーン初期化
GameScene::GameScene()
{
    m_isChangeScene = false;
    auto gameObjectManager = GameObjectManager::GetInstance();
    gameObjectManager->Initialize();
}

//データ破棄
GameScene::~GameScene()
{

}

//シーン更新
void GameScene::Update()
{
    auto pad = DeviceAccessor::GetInstance()->GetGamePad()->GetState(0);
    auto key = DeviceAccessor::GetInstance()->GetKeyboard()->GetState();

	auto gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager->Update();

    //クリアしていたらシーン遷移
    if (PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())
    {
        m_isChangeScene = true;
    }

    //見つかったらシーン遷移
    for (int i = 0; i < UIAccessor::GetInstance()->GetUIs().size(); i++)
    {
        auto foundUI = dynamic_cast<FoundUI*>(UIAccessor::GetInstance()->GetUIs().at(i));
        if (foundUI && foundUI->GetIsFound())
        {
            m_isChangeScene = true;
        }
        auto transition = dynamic_cast<Transition*>(UIAccessor::GetInstance()->GetUIs().at(i));
        if (transition && m_isChangeScene)
        {
            transition->SetTransitionStart(true);
        }
        if (transition && transition->GetIsFinishFadeout() && m_isChangeScene)
        {
            transition->SetTransitionStart(false);
            SceneManager::GetInstance()->ChangeScene(SceneManager::Scene::Result);
        }
    }

    if (pad.IsViewPressed() || key.Escape)
    {
        ExitGame();
    }
}

//シーン描画
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

//オフスクリーン描画
void GameScene::DrawOffScreen()
{
    auto gameObjectManager = GameObjectManager::GetInstance();

    //プレイヤーの深度値描画
    PlayerAccessor::GetInstance()->GetPlayer()->DrawDepth();

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