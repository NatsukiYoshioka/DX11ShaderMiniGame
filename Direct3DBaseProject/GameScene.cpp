#include "pch.h"
#include "BaseScene.h"
#include"GameObjectManager.h"
#include "GameScene.h"

GameScene::GameScene()
{
    auto gameObjectManager = GameObjectManager::GetInstance();
    gameObjectManager->Initialize();
}

GameScene::~GameScene()
{

}

void GameScene::Update()
{
	auto gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager->Update();
}

void GameScene::Draw()
{
	auto gameObjectManager = GameObjectManager::GetInstance();
    //敵視点から当たり判定と影テクスチャのセット
    gameObjectManager->SetHitCheckShaderResource();
    gameObjectManager->SetHitCheckCharacterShaderResource();
    gameObjectManager->HitCheck();

    gameObjectManager->SetObjectShadowResource();
    gameObjectManager->SetCharacterShadowResource();

    //オブジェクトの描画
	gameObjectManager->Draw();
}

void GameScene::DrawOffScreen()
{
    auto gameObjectManager = GameObjectManager::GetInstance();
    //影の描画
    gameObjectManager->ClearObjectShadow();
    gameObjectManager->SetObjectShadowRenderTarget();
    gameObjectManager->DrawObjectShadow();

    gameObjectManager->ClearCharacterShadow();
    gameObjectManager->SetCharacterShadowRenderTarget();
    gameObjectManager->DrawCharacterShadow();

    //敵視点からの当たり判定用描画
    gameObjectManager->ClearHitCheckRenderTarget();
    gameObjectManager->SetHitCheckRenderTarget();
    gameObjectManager->DrawHitCheck();

    gameObjectManager->ClearHitCheckCharacterRenderTarget();
    gameObjectManager->SetHitCheckCharacterRenderTarget();
    gameObjectManager->DrawHitCheckCharacter();
}