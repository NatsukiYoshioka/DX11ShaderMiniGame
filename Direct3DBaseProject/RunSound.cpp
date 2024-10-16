#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include "RunSound.h"

//���������̑����̏�����
RunSound::RunSound():
	m_volume(float(Json::GetInstance()->GetData()["RunSoundVolume"]))
{
	auto audioEngine = DeviceAccessor::GetInstance()->GetAudioEngine();
	auto json = Json::GetInstance();

	//�T�E���h�̃��[�h
	SetCurrentDirectory(L"Assets/Sounds");
	m_sound = make_unique<SoundEffect>(audioEngine, json->Widen(json->GetData()["RunSound"]).c_str());
	m_soundInstance = m_sound->CreateInstance();
	SetCurrentDirectory(L"../../");
}

//�f�[�^�j��
RunSound::~RunSound()
{

}

//�^�C�g���ł̃T�E���h������
void RunSound::InitializeTitle()
{
	Initialize();
}

//�^�C�g���ł̃T�E���h�X�V(�����Ȃ�)
void RunSound::UpdateTitle()
{

}

//�Q�[���V�[���ł̃T�E���h������
void RunSound::Initialize()
{
	m_soundInstance->Stop();
	m_soundInstance->SetVolume(m_volume);
}

//�Q�[���V�[���ł̃T�E���h�X�V
void RunSound::Update()
{
	auto animationState = PlayerAccessor::GetInstance()->GetPlayer()->GetNowAnimationState();
	//�v���C���[�������Ă�����Đ�
	if (animationState == Player::AnimationState::Run)
	{
		m_soundInstance->Play(true);
	}
	else
	{
		m_soundInstance->Stop();
	}
}

//���U���g�ł̃T�E���h������
void RunSound::InitializeResult()
{
	Initialize();
}

//���U���g�ł̃T�E���h�X�V(�����Ȃ�)
void RunSound::UpdateResult()
{

}