#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include "WalkSound.h"

//����������SE������
WalkSound::WalkSound():
	m_volume(float(Json::GetInstance()->GetData()["WalkSoundVolume"]))
{
	auto audioEngine = DeviceAccessor::GetInstance()->GetAudioEngine();
	auto json = Json::GetInstance();

	//�T�E���h�̃��[�h
	SetCurrentDirectory(L"Assets/Sounds");
	m_sound = make_unique<SoundEffect>(audioEngine, json->Widen(json->GetData()["WalkSound"]).c_str());
	m_soundInstance = m_sound->CreateInstance();
	SetCurrentDirectory(L"../../");
}

//�f�[�^�j��
WalkSound::~WalkSound()
{

}

//�^�C�g���ł̃T�E���h������
void WalkSound::InitializeTitle()
{
	Initialize();
}

//�^�C�g���ł̃T�E���h�X�V(�����Ȃ�)
void WalkSound::UpdateTitle()
{

}

//�Q�[���V�[���ł̃T�E���h������
void WalkSound::Initialize()
{
	m_soundInstance->Stop();
	m_soundInstance->SetVolume(m_volume);
}

//�Q�[���V�[���ł̃T�E���h�X�V
void WalkSound::Update()
{
	auto animationState = PlayerAccessor::GetInstance()->GetPlayer()->GetNowAnimationState();
	//�v���C���[�������Ă�����T�E���h�Đ�
	if (animationState == Player::AnimationState::CrouchedWalk || animationState == Player::AnimationState::Walk)
	{
		m_soundInstance->Play(true);
	}
	else
	{
		m_soundInstance->Stop();
	}
}

//���U���g�ł̃T�E���h������
void WalkSound::InitializeResult()
{
	Initialize();
}

//���U���g�ł̃T�E���h�X�V(�����Ȃ�)
void WalkSound::UpdateResult()
{

}