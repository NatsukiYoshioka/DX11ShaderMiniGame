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

//�Q�[���V�[��������
GameScene::GameScene()
{
    m_isChangeScene = false;
    auto gameObjectManager = GameObjectManager::GetInstance();
    gameObjectManager->Initialize();
}

//�f�[�^�j��
GameScene::~GameScene()
{

}

//�V�[���X�V
void GameScene::Update()
{
    auto pad = DeviceAccessor::GetInstance()->GetGamePad()->GetState(0);
    auto key = DeviceAccessor::GetInstance()->GetKeyboard()->GetState();

	auto gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager->Update();

    //�N���A���Ă�����V�[���J��
    if (PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())
    {
        m_isChangeScene = true;
    }

    //����������V�[���J��
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

//�V�[���`��
void GameScene::Draw()
{
	auto gameObjectManager = GameObjectManager::GetInstance();
    //�G���_���瓖���蔻��Ɖe�e�N�X�`���̃Z�b�g
    gameObjectManager->SetHitCheckShaderResource();
    gameObjectManager->SetHitCheckCharacterShaderResource();
    gameObjectManager->HitCheck();

    gameObjectManager->SetObjectShadowResource();

    //�I�u�W�F�N�g�̕`��
	gameObjectManager->Draw();
}

//�I�t�X�N���[���`��
void GameScene::DrawOffScreen()
{
    auto gameObjectManager = GameObjectManager::GetInstance();

    //�v���C���[�̐[�x�l�`��
    PlayerAccessor::GetInstance()->GetPlayer()->DrawDepth();

    //�e�̕`��
    gameObjectManager->ClearObjectShadow();
    gameObjectManager->SetObjectShadowRenderTarget();
    gameObjectManager->DrawShadow();

    //�G���_����̓����蔻��p�`��
    gameObjectManager->ClearHitCheckRenderTarget();
    gameObjectManager->SetHitCheckRenderTarget();
    gameObjectManager->DrawHitCheck();

    gameObjectManager->ClearHitCheckCharacterRenderTarget();
    gameObjectManager->SetHitCheckCharacterRenderTarget();
    gameObjectManager->DrawHitCheckCharacter();
}