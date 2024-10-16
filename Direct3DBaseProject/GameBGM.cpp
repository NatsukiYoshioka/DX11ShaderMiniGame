#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Transition.h"
#include"FoundUI.h"
#include"UIAccessor.h"
#include"FoundSE.h"
#include"SoundBase.h"
#include"SoundAccessor.h"
#include "GameBGM.h"

//�Q�[���V�[��BGM�̏�����
GameBGM::GameBGM():
	m_volume(0.f),
	m_initializeVolume(float(Json::GetInstance()->GetData()["GameBGMVolume"]))
{
	auto audioEngine = DeviceAccessor::GetInstance()->GetAudioEngine();
	auto json = Json::GetInstance();

	//�T�E���h�̃��[�h
	SetCurrentDirectory(L"Assets/Sounds");
	m_sound = make_unique<SoundEffect>(audioEngine, json->Widen(json->GetData()["GameBGM"]).c_str());
	m_soundInstance = m_sound->CreateInstance();
	SetCurrentDirectory(L"../../");
}

//�f�[�^�j��
GameBGM::~GameBGM()
{
	m_soundInstance.reset();
	m_sound.reset();
}

//�^�C�g���ł̃T�E���h������
void GameBGM::InitializeTitle()
{
	Initialize();
}

//�^�C�g���ł̃T�E���h�X�V(�����Ȃ�)
void GameBGM::UpdateTitle()
{
	
}

//�Q�[���V�[���ł̃T�E���h������
void GameBGM::Initialize()
{
	m_soundInstance->Stop();
	m_volume = m_initializeVolume;
}

//�Q�[���V�[���ł̃T�E���h�X�V
void GameBGM::Update()
{
	bool isFound = false;
	//���S�Ɍ����������ǂ������擾
	for (int i = 0;i < UIAccessor::GetInstance()->GetUIs().size();i++)
	{
		auto foundUI = dynamic_cast<FoundUI*>(UIAccessor::GetInstance()->GetUIs().at(i));
		if (foundUI && foundUI->GetIsFound())
		{
			isFound = true;
			break;
		}
	}
	for (int i = 0;i < UIAccessor::GetInstance()->GetUIs().size();i++)
	{
		auto transition = dynamic_cast<Transition*>(UIAccessor::GetInstance()->GetUIs().at(i));
		//����������g�����W�V�����̃t�F�[�h�C���ɍ��킹�ă{�����[����������
		if (transition && isFound)
		{
			m_volume = m_initializeVolume - transition->GetAlpha();
			if (m_volume <= 0.f)
			{
				m_volume = 0.f;
			}
			break;
		}
	}
	for (int i = 0;i < SoundAccessor::GetInstance()->GetSounds().size();i++)
	{
		auto foundSE = dynamic_cast<FoundSE*>(SoundAccessor::GetInstance()->GetSounds().at(i));
		//�������Ă���Ƃ���SE���Đ�����Ă���Ƃ���BGM�̃{�����[���𔼌�������
		if (foundSE)
		{
			if (foundSE->GetSoundState() == SoundState::PLAYING)
			{
				m_soundInstance->SetVolume(m_volume / 2);
			}
			else m_soundInstance->SetVolume(m_volume);

			break;
		}
	}
	m_soundInstance->Play(true);
}

//���U���g�ł̃T�E���h������
void GameBGM::InitializeResult()
{
	Initialize();
}

//���U���g�ł̍X�V(�����Ȃ�)
void GameBGM::UpdateResult()
{

}