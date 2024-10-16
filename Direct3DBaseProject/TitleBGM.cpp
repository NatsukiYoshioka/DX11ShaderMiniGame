#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include "TitleBGM.h"

//�^�C�g��BGM�̏�����
TitleBGM::TitleBGM():
	m_volume(1.f)
{
	auto audioEngine = DeviceAccessor::GetInstance()->GetAudioEngine();
	auto json = Json::GetInstance();

	//�T�E���h�̃��[�h
	SetCurrentDirectory(L"Assets/Sounds");
	m_sound = make_unique<SoundEffect>(audioEngine, json->Widen(json->GetData()["TitleBGM"]).c_str());
	m_soundInstance = m_sound->CreateInstance();
	SetCurrentDirectory(L"../../");
}

//�f�[�^�j��
TitleBGM::~TitleBGM()
{
	m_soundInstance.reset();
	m_sound.reset();
}

//�^�C�g���ł̃T�E���h������
void TitleBGM::InitializeTitle()
{
	Initialize();
}

//�^�C�g���ł̃T�E���h�X�V
void TitleBGM::UpdateTitle()
{
	//�^�C�g���ł̃J�����̈ړ������ŃT�E���h�̃{�����[����ݒ�
	m_volume = CameraAccessor::GetInstance()->GetCamera()->GetTitlePosRatio();
	m_soundInstance->SetVolume(m_volume);
	m_soundInstance->Play(true);
}

//�Q�[���V�[���ł̃T�E���h������
void TitleBGM::Initialize()
{
	m_soundInstance->Stop();
	m_volume = 1.f;
}

//�Q�[���V�[���ł̃T�E���h�X�V(�����Ȃ�)
void TitleBGM::Update()
{

}

//���U���g�ł̃T�E���h������
void TitleBGM::InitializeResult()
{
	Initialize();
}

//���U���g�ł̃T�E���h�X�V
void TitleBGM::UpdateResult()
{

}