#include "pch.h"
#include"DeviceAccessor.h"
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

    if (pad.IsViewPressed() || key.Escape)
    {
        ExitGame();
    }

	auto gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager->Update();

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
            if (key.Enter || pad.IsBPressed())
            {
                SceneManager::GetInstance()->ChangeScene(SceneManager::Scene::Title);
            }
        }
    }
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