#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include"FoundUI.h"
#include"UIBase.h"
#include"UIAccessor.h"
#include "FoundSE.h"

//�T�E���h�̏�����
FoundSE::FoundSE():
	m_pitch(0.f),
	m_volume(float(Json::GetInstance()->GetData()["FoundSEVolume"]))
{
	auto audioEngine = DeviceAccessor::GetInstance()->GetAudioEngine();
	auto json = Json::GetInstance();

	//�T�E���h�̃��[�h
	SetCurrentDirectory(L"Assets/Sounds");
	m_sound = make_unique<SoundEffect>(audioEngine, json->Widen(json->GetData()["FoundSE"]).c_str());
	m_soundInstance = m_sound->CreateInstance();
	SetCurrentDirectory(L"../../");
}

//�f�[�^�j��
FoundSE::~FoundSE()
{

}

//�^�C�g���ł̏�����
void FoundSE::InitializeTitle()
{
	Initialize();
}

//�^�C�g���ł̍X�V(�����Ȃ�)
void FoundSE::UpdateTitle()
{

}

//�Q�[���V�[���ł̏�����
void FoundSE::Initialize()
{
	m_soundInstance->Stop();
	m_soundInstance->SetVolume(m_volume);
}

//�Q�[���V�[���ł̍X�V
void FoundSE::Update()
{
	//�������Ă�����s�b�`���グ�Ȃ���Đ�
	if (PlayerAccessor::GetInstance()->GetPlayer()->GetBeFound() &&
		PlayerAccessor::GetInstance()->GetPlayer()->GetNowAnimationState() != Player::AnimationState::Die&&
		!PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())
	{
		for (int i = 0; i < UIAccessor::GetInstance()->GetUIs().size(); i++)
		{
			auto foundUI = dynamic_cast<FoundUI*>(UIAccessor::GetInstance()->GetUIs().at(i));
			if (foundUI)
			{
				m_soundInstance->SetPitch(m_pitch + 1.f * foundUI->GetTimeRatio());
			}
		}
		m_soundInstance->Play(true);
	}
	else
	{
		m_soundInstance->Stop(true);
	}
}

//���U���g�ł̏�����
void FoundSE::InitializeResult()
{
	Initialize();
}

//���U���g�ł̍X�V(�����Ȃ�)
void FoundSE::UpdateResult()
{

}