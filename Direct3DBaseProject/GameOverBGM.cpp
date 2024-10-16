#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include"Transition.h"
#include"UIAccessor.h"
#include"BaseScene.h"
#include"SceneManager.h"
#include "GameOverBGM.h"

//�Q�[���I�[�o�[BGM������
GameOverBGM::GameOverBGM():
	m_volume(1.f)
{
	auto audioEngine = DeviceAccessor::GetInstance()->GetAudioEngine();
	auto json = Json::GetInstance();

	//�T�E���h�̃��[�h
	SetCurrentDirectory(L"Assets/Sounds");
	m_sound = make_unique<SoundEffect>(audioEngine, json->Widen(json->GetData()["GameOverBGM"]).c_str());
	m_soundInstance = m_sound->CreateInstance();
	SetCurrentDirectory(L"../../");
}

//�f�[�^�j��
GameOverBGM::~GameOverBGM()
{

}

//�^�C�g���ł̃T�E���h������
void GameOverBGM::InitializeTitle()
{
	Initialize();
}

//�^�C�g���ł̃T�E���h�X�V(�����Ȃ�)
void GameOverBGM::UpdateTitle()
{

}

//�Q�[���V�[���ł̃T�E���h������
void GameOverBGM::Initialize()
{
	m_soundInstance->Stop();
	m_soundInstance->SetVolume(m_volume);
	m_volume = 1.f;
}

//�Q�[���V�[���ł̃T�E���h�X�V(�����Ȃ�)
void GameOverBGM::Update()
{

}

//���U���g�ł̃T�E���h������
void GameOverBGM::InitializeResult()
{
	Initialize();
}

//���U���g�ł̃T�E���h�X�V
void GameOverBGM::UpdateResult()
{
	if (PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())return;
	auto isChangeScene = SceneManager::GetInstance()->GetNowScene()->GetIsChangeScene();
	for (int i = 0;i < UIAccessor::GetInstance()->GetUIs().size();i++)
	{
		auto transition = dynamic_cast<Transition*>(UIAccessor::GetInstance()->GetUIs().at(i));
		//�g�����W�V�����ɍ��킹�ăT�E���h�̃{�����[��������������
		if (transition && isChangeScene)
		{
			m_volume = 1.f - transition->GetAlpha();
		}
	}
	m_soundInstance->SetVolume(m_volume);
	m_soundInstance->Play(true);
}

