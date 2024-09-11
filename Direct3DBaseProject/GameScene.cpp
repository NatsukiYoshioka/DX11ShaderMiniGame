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
    //�G���_���瓖���蔻��Ɖe�e�N�X�`���̃Z�b�g
    gameObjectManager->SetHitCheckShaderResource();
    gameObjectManager->SetHitCheckCharacterShaderResource();
    gameObjectManager->HitCheck();

    gameObjectManager->SetObjectShadowResource();
    gameObjectManager->SetCharacterShadowResource();

    //�I�u�W�F�N�g�̕`��
	gameObjectManager->Draw();
}

void GameScene::DrawOffScreen()
{
    auto gameObjectManager = GameObjectManager::GetInstance();
    //�e�̕`��
    gameObjectManager->ClearObjectShadow();
    gameObjectManager->SetObjectShadowRenderTarget();
    gameObjectManager->DrawObjectShadow();

    gameObjectManager->ClearCharacterShadow();
    gameObjectManager->SetCharacterShadowRenderTarget();
    gameObjectManager->DrawCharacterShadow();

    //�G���_����̓����蔻��p�`��
    gameObjectManager->ClearHitCheckRenderTarget();
    gameObjectManager->SetHitCheckRenderTarget();
    gameObjectManager->DrawHitCheck();

    gameObjectManager->ClearHitCheckCharacterRenderTarget();
    gameObjectManager->SetHitCheckCharacterRenderTarget();
    gameObjectManager->DrawHitCheckCharacter();
}